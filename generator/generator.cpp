#include "generator.h"
#include <QFile>
#include <QTextStream>

const char * paramString[] = {
    "int",
    "qint64",
    "bool",
    "const QString&" };

const char * paramComparison[] = {
    " != 0",
    " != 0",
    "",
    ".isEmpty()",
};

const char * defaultValueString[] = {
    "0",
    "0",
    "false",
    "QString()"
};

Generator::Generator(QObject *parent) :
    QObject(parent)
{
    m_requiredParam.type = NONE;
}

void Generator::addParam(const QString &name, ParamType type)
{
    Parameter parameter;
    parameter.name = name;
    parameter.type = type;

    m_params.push_back(parameter);
}

void Generator::setRequiredParam(const QString &name, ParamType type)
{
    m_requiredParam.name = name;
    m_requiredParam.type = type;
}

void Generator::generateHeaderFile()
{
    QString headerFileName = "qtweet" + m_className.toLower() + ".h";
    QFile headerFile(headerFileName);

    if (headerFile.open(QIODevice::ReadWrite | QIODevice::Truncate)) {
        QTextStream out(&headerFile);

        out << "//-----------------Generated file---------------\n\n\n";

        QString headerGuard = "QTWEET" + m_className.toUpper() + "_H";

        out << "#ifndef " << headerGuard << "\n";
        out << "#define " << headerGuard << "\n";
        out << "\n";
        out << "#include \"qtweetnetbase.h\"\n";
        out << "\n";

        out << "class QTWEETLIBSHARED_EXPORT " << "QTweet" << m_className << " : public QTweetNetBase\n";
        out << "{\n";
        out << "    Q_OBJECT\n";
        out << "public:\n";
        out << "    QTweet" << m_className << "(QObject *parent = 0);\n";
        out << "    QTweet" << m_className << "(OAuthTwitter *oauthTwitter, QObject *parent = 0);\n";
        out << "    void fetch(";

        QString methodParams;
        QTextStream paramsStream(&methodParams);

        if (m_requiredParam.type != NONE) {
            paramsStream << paramString[m_requiredParam.type] << " " << m_requiredParam.name << ", ";
        }

        foreach (const Parameter& parameter, m_params) {
            paramsStream << paramString[parameter.type] << " " << parameter.name;
            paramsStream << " = " << defaultValueString[parameter.type] << ", ";
        }

        methodParams.chop(2);

        out << methodParams;
        out << ");\n\n";
        out << "signals:\n";

        if (m_responseType == ListStatus)
            out << "    void statusList(const QList<QTweetStatus>& statuses);\n";
        else if (m_responseType == Status)
            out << "    void status(const QTweetStatus& status);\n";

        out << "\n";

        out << "protected slots:\n";
        out << "    void parseJsonFinished(const QJsonDocument &jsonDoc);\n";
        out << "};";
        out << "\n";
        out << "#endif\n";
    }
}

void Generator::generateCppFile()
{
    QString cppFileName = "qtweet" + m_className.toLower() + ".cpp";
    QFile cppFile(cppFileName);

    if (cppFile.open(QIODevice::ReadWrite | QIODevice::Truncate)) {
        QTextStream out(&cppFile);

        out << "#include \"" << "qtweet" << m_className.toLower() << ".h\"\n";
        out << "#include <QNetworkRequest>\n";
        out << "#include <QNetworkReply>\n";
        out << "#include <QUrlQuery>\n";
        out << "#include \"qtweetstatus.h\"\n";
        out << "#include \"qtweetconvert.h\"\n";
        out << "#include <QJsonDocument>\n";
        out << "#include <QJsonArray>\n";
        out << "#include <QJsonObject>\n\n";

        QString className = "QTweet" + m_className;

        out << className << "::" << className << "(QObject *parent) : QTweetNetBase(parent)\n";
        out << "{\n";
        out << "}\n";
        out << "\n";

        out << className << "::" << className << "(OAuthTwitter *oauthTwitter, QObject *parent) : QTweetNetBase(oauthTwitter, parent)\n";
        out << "{\n";
        out << "}\n";
        out << "\n";

        out << "void " << className << "::" << "fetch(";

        QString methodParams;
        QTextStream paramsStream(&methodParams);

        if (m_requiredParam.type != NONE)
            paramsStream << paramString[m_requiredParam.type] << " " << m_requiredParam.name << ", ";

        foreach (const Parameter& parameter, m_params) {
            paramsStream << paramString[parameter.type] << " " << parameter.name << ", ";
        }

        methodParams.chop(2);

        out << methodParams << ")\n";
        out << "{\n";

        out << "    if (!isAuthenticationEnabled()) {\n";
        out << "        qCritical(\"Needs authentication to be enabled\");\n";
        out << "        return;\n";
        out << "    }\n";
        out << "\n";

        if (m_requiredParam.type != NONE && m_url.contains("%")) {
            out << "    QString urlString = QString(\"" << m_url << "\").arg(" << m_requiredParam.name << ");\n";
            out << "    QUrl url(urlString);\n";
            out << "    QUrlQuery urlQuery;\n";
            out << "\n";
        } else {
            out << "    QUrl url(\"" << m_url << "\");\n";
            out << "    QUrlQuery urlQuery;\n";
            out << "\n";

            if (m_requiredParam.type != NONE) {
                out << "    urlQuery.addQueryItem(\"" << m_requiredParam.name << "\", ";

                if (m_requiredParam.type == INT || m_requiredParam.type == QINT64) {
                    out << "QString::number(" << m_requiredParam.name << ")";
                } else if (m_requiredParam.type == BOOL) {
                    out << "\"true\"";
                } else if (m_requiredParam.type == STRING) {
                    out << m_requiredParam.name;
                }

                out << ");\n";
                out << "\n";
            }
        }

        foreach (const Parameter& parameter, m_params) {
            out << "    if (";

            if (parameter.type == STRING)
                out << "!";

            out << parameter.name;
            out << paramComparison[parameter.type] << ")\n";

            out << "        urlQuery.addQueryItem(\"" << parameter.name << "\", ";

            if (parameter.type == INT || parameter.type == QINT64) {
                out << "QString::number(" << parameter.name << ")";
            } else if (parameter.type == BOOL) {
                out << "\"true\"";
            } else if (parameter.type == STRING) {
                out << parameter.name;
            }

            out << ");\n";
            out << "\n";
        }

        out << "    url.setQuery(urlQuery);\n";
        out << "\n";
        out << "    QNetworkRequest req(url);\n";
        out << "\n";

        if (m_method == GET) {
            out << "    QByteArray oauthHeader = oauthTwitter()->generateAuthorizationHeader(url, OAuth::GET);\n";
            out << "    req.setRawHeader(AUTH_HEADER, oauthHeader);\n";
            out << "\n";
            out << "    QNetworkReply *reply = oauthTwitter()->networkAccessManager()->get(req);\n";
            out << "    connect(reply, SIGNAL(finished()), this, SLOT(reply()));\n";
        } else if (m_method == POST) {
            // ### TODO:
        }

        out << "}\n";
        out << "\n";

        out << "void QTweet" << m_className << "::parseJsonFinished(const QJsonDocument &jsonDoc)\n";
        out << "{\n";

        if (m_responseType == ListStatus) {
            out << "    if (jsonDoc.isArray()) {\n";
            out << "        QList<QTweetStatus> statuses = QTweetConvert::jsonArrayToStatusList(jsonDoc.array());\n\n";
            out << "        emit statusList(statuses);\n";
            out << "    }\n";
        } else if (m_responseType == Status) {
            out << "    if (jsonDoc.isObject()) {\n";
            out << "        QTweetStatus parsedStatus = QTweetConvert::jsonObjectToStatus(jsonDoc.object());\n";
            out << "\n";
            out << "        emit status(parsedStatus);\n";
            out << "    }\n";
        }

        out << "}\n";
    }
}
