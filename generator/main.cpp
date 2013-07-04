#include <QCoreApplication>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QtDebug>
#include <QFile>
#include <QStringList>
#include <QTimer>
#include "generator.h"

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);
    
    if (argc != 2)
        exit(EXIT_FAILURE);

    QFile file(argv[1]);
    file.open(QIODevice::ReadOnly | QIODevice::Text);

    QByteArray apiJson = file.readAll();

    QJsonDocument parsedJson = QJsonDocument::fromJson(apiJson);

    if (parsedJson.isArray()) {
        QJsonArray resource = parsedJson.array();

        foreach(const QJsonValue& value, resource) {
            if (value.isObject()) {
                QJsonObject details = value.toObject();

                Generator gen;
                gen.setClassName(details["class"].toString());
                gen.setUrl(details["url"].toString());

                if (details["method"].toString() == "GET")
                    gen.setHttpMethod(Generator::GET);
                else if (details["method"].toString() == "POST")
                    gen.setHttpMethod(Generator::POST);

                if (details["response"].toString() == "ListStatus")
                    gen.setResponseType(Generator::ListStatus);
                else if (details["response"].toString() == "Status")
                    gen.setResponseType(Generator::Status);

                if (details["required"].isObject()) {
                    QJsonObject required = details["required"].toObject();
                    QString paramName = required.keys().at(0);
                    QString paramType = required.value(paramName).toString();

                    if (paramType == "int")
                        gen.setRequiredParam(paramName, Generator::INT);
                    else if (paramType == "qint64")
                        gen.setRequiredParam(paramName, Generator::QINT64);
                    else if (paramType == "bool")
                        gen.setRequiredParam(paramName, Generator::BOOL);
                    else if (paramType == "string")
                        gen.setRequiredParam(paramName, Generator::STRING);
                }

                QJsonArray params = details["parameters"].toArray();

                for (QJsonArray::const_iterator it = params.constBegin(); it != params.constEnd(); ++it) {
                    QJsonObject param = (*it).toObject();

                    QString paramName = param.keys().at(0);
                    QString paramType = param.value(paramName).toString();

                    if (paramType == "int")
                        gen.addParam(paramName, Generator::INT);
                    else if (paramType == "qint64")
                        gen.addParam(paramName, Generator::QINT64);
                    else if (paramType == "bool")
                        gen.addParam(paramName, Generator::BOOL);
                    else if (paramType == "string")
                        gen.addParam(paramName, Generator::STRING);
                }

                gen.generate();
            }
        }
    }

    QTimer::singleShot(10, &app, SLOT(quit()));

    return app.exec();
}
