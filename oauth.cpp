/* Copyright (c) 2010, Antonie Jovanoski
 *
 * All rights reserved.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library. If not, see <http://www.gnu.org/licenses/>.
 *
 * Contact e-mail: Antonie Jovanoski <minimoog77_at_gmail.com>
 */

#include <QDateTime>
#include <QtAlgorithms>
#include <QtDebug>
#include "oauth.h"

#define CONSUMER_KEY ""
#define CONSUMER_SECRET ""

/*!
    Generates time stamp
    \return time stamp in epoch time
*/
static QByteArray generateTimeStamp()
{
	//OAuth spec. 8 http://oauth.net/core/1.0/#nonce
	QDateTime current = QDateTime::currentDateTime();
	uint seconds = current.toTime_t();

	return QString("%1").arg(seconds).toUtf8();
}

/*!
    Generates random 16 length string
    \return random string
*/
static QByteArray generateNonce()
{
	//OAuth spec. 8 http://oauth.net/core/1.0/#nonce
	QByteArray chars("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789");
	int max = chars.size();

	int len = 16;

	QByteArray nonce;
	for(int i = 0; i < len; ++i){
		nonce.append( chars[qrand() % max] );
	}

	return nonce;
}

#ifdef Q_OS_WIN
/*!
    Constructor
    \param parent Parent QObject
*/
OAuth::OAuth(QObject *parent) : QObject(parent), m_libraryOpenssl("libeay32", parent)
{
	QDateTime current = QDateTime::currentDateTime();
	qsrand(current.toTime_t());

    //fetch function pointers
    HMAC_fp = (FPHMAC)m_libraryOpenssl.resolve("HMAC");

    if (!HMAC_fp) {
        qDebug() << "Error resolving symbol HMAC";
    }

    EVP_sha1_fp = (FPEVPSHA1)m_libraryOpenssl.resolve("EVP_sha1");

    if (!EVP_sha1_fp) {
        qDebug() << "Error resolving symbol EVP_sha1";
    }
}
#else
OAuth::OAuth(QObject *parent) : QObject(parent), m_libraryOpenssl("crypto", parent)
{
    QDateTime current = QDateTime::currentDateTime();
	qsrand(current.toTime_t());

    if (!m_libraryOpenssl.isLoaded())
        qDebug() << "Error loading libcrypto library";

    //fetch function pointers
    HMAC_fp = (FPHMAC)m_libraryOpenssl.resolve("HMAC");

    if (!HMAC_fp) {
        qDebug() << "Error resolving symbol HMAC";
    }

    EVP_sha1_fp = (FPEVPSHA1)m_libraryOpenssl.resolve("EVP_sha1");

    if (!EVP_sha1_fp) {
        qDebug() << "Error resolving symbol EVP_sha1";
    }
}
#endif

/*!
    Parses oauth_token and oauth_token_secret from response of the service provider
    and sets m_oauthToken and m_oauthTokenSecret accordingly

    \param response Response from service provider
 */
void OAuth::parseTokens(const QByteArray& response)
{
	//OAuth spec 5.3, 6.1.2, 6.3.2
	//use QUrl for parsing 
	QByteArray parseQuery("http://parse.com?");

	QUrl parseUrl = QUrl::fromEncoded(parseQuery + response);

	m_oauthToken = parseUrl.encodedQueryItemValue("oauth_token");
	m_oauthTokenSecret = parseUrl.encodedQueryItemValue("oauth_token_secret");
}

/*!
    Sets oauth token
    \param[in] token OAuth token
*/
void OAuth::setOAuthToken(const QByteArray& token)
{
	m_oauthToken = token;
}

/*!
    Sets OAauth token secret
    \param[in] tokenSecret OAuth token secret
*/
void OAuth::setOAuthTokenSecret(const QByteArray& tokenSecret)
{
	m_oauthTokenSecret = tokenSecret;
}

/*!
    Gets oauth_token
    \return OAuth token
*/
QByteArray OAuth::oauthToken() const
{
	return m_oauthToken;
}

/*!
    Gets oauth_token_secret
    \return OAuth token secret
*/
QByteArray OAuth::oauthTokenSecret() const
{
	return m_oauthTokenSecret;
}

void OAuth::clearTokens()
{
    m_oauthToken.clear();
    m_oauthTokenSecret.clear();
}

/*!
    Generates HMAC-SHA1 signature
    \param[in] signatureBase String
    \return HMAC-SHA1 signature
*/
QByteArray OAuth::generateSignatureHMACSHA1(const QByteArray& signatureBase)
{
	//OAuth spec. 9.2 http://oauth.net/core/1.0/#anchor16
	QByteArray key = QByteArray(CONSUMER_SECRET) + '&' + m_oauthTokenSecret;

	unsigned char digest[EVP_MAX_MD_SIZE];
	unsigned int digestLen = 0;

    HMAC_fp(EVP_sha1_fp(), 
		key.constData(), 
		key.size(), 
		(const unsigned char*)signatureBase.constData(), 
		signatureBase.size(), 
		digest, 
		&digestLen);

	QByteArray result((const char *)digest, digestLen);
	QByteArray resultBE64 = result.toBase64();
	QByteArray resultPE = resultBE64.toPercentEncoding();
	return resultPE;
}

/*!
    Generates OAuth signature base
    \param[in] url Url with encoded parameters
    \param[in] method Http method
    \param[in] timestamp timestamp
    \param[in] nonce random string
    \return signature base
*/
QByteArray OAuth::generateSignatureBase(const QUrl& url, HttpMethod method, const QByteArray& timestamp, const QByteArray& nonce)
{
	//OAuth spec. 9.1 http://oauth.net/core/1.0/#anchor14

	//OAuth spec. 9.1.1
	QList<QPair<QByteArray, QByteArray> > urlParameters = url.encodedQueryItems();
	QList<QByteArray> normParameters;

	QListIterator<QPair<QByteArray, QByteArray> > i(urlParameters);
	while(i.hasNext()){
		QPair<QByteArray, QByteArray> queryItem = i.next();
		QByteArray normItem = queryItem.first + '=' + queryItem.second;
		normParameters.append(normItem);
	}

	//consumer key
	normParameters.append(QByteArray("oauth_consumer_key=") + QByteArray(CONSUMER_KEY));

	//token
	if(!m_oauthToken.isEmpty()){
		normParameters.append(QByteArray("oauth_token=") + m_oauthToken);
	}

	//signature method, only HMAC_SHA1
	normParameters.append(QByteArray("oauth_signature_method=HMAC-SHA1"));
	//time stamp
	normParameters.append(QByteArray("oauth_timestamp=") + timestamp);
	//nonce
	normParameters.append(QByteArray("oauth_nonce=") + nonce);
	//version
	normParameters.append(QByteArray("oauth_version=1.0"));

	//OAuth spec. 9.1.1.1
	qSort(normParameters);

	//OAuth spec. 9.1.1.2
	QByteArray normString;
	QListIterator<QByteArray> j(normParameters);
	while(j.hasNext()){
		normString += j.next();
		normString += '&';
	}
	normString.chop(1);

	//OAuth spec. 9.1.2
	QString urlScheme = url.scheme();
	QString urlPath = url.path();
	QString urlHost = url.host();
	QByteArray normUrl = urlScheme.toUtf8() + "://" + urlHost.toUtf8() + urlPath.toUtf8();

	QByteArray httpm;

	switch (method)
	{
		case OAuth::GET:
			httpm = "GET";
			break;
		case OAuth::POST:
			httpm = "POST";
			break;
		case OAuth::DELETE:
			httpm = "DELETE";
			break;
		case OAuth::PUT:
			httpm = "PUT";
			break;
	}

	//OAuth spec. 9.1.3
	return httpm + '&' + normUrl.toPercentEncoding() + '&' + normString.toPercentEncoding();
}

/*!
    Generates Authorization Header
    \remarks If HttpMethod is POST put query items in url (QUrl::addEncodedQueryItem)
    \param[in] url Url with query items embedded
    \param[in] method Type of http method
 */
QByteArray OAuth::generateAuthorizationHeader( const QUrl& url, HttpMethod method )
{
	QByteArray timeStamp = generateTimeStamp();
	QByteArray nonce = generateNonce();

	QByteArray sigBase = generateSignatureBase(url, method, timeStamp, nonce);
	QByteArray signature = generateSignatureHMACSHA1(sigBase);

	QByteArray header;
	header += "OAuth ";
	header += "oauth_consumer_key=\"" + QByteArray(CONSUMER_KEY) + "\",";
	if(!m_oauthToken.isEmpty())
		header += "oauth_token=\"" + m_oauthToken + "\",";
	header += "oauth_signature_method=\"HMAC-SHA1\",";
	header += "oauth_signature=\"" + signature + "\",";
	header += "oauth_timestamp=\"" + timeStamp + "\",";
	header += "oauth_nonce=\"" + nonce + "\",";
	header += "oauth_version=\"1.0\"";

	return header;
}
