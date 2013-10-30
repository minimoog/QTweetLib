#ifndef GENERATOR_H
#define GENERATOR_H

#include <QObject>
#include <QList>

class Generator : public QObject
{
    Q_OBJECT
public:
    enum HttpMethod {
        GET,
        POST
    };

    enum ParamType {
        INT = 0,
        QINT64 = 1,
        BOOL = 2 ,
        STRING = 3,
        NONE
    };

    enum ResponseType {
        ListStatus,
        Status,
        ListDM
    };

    explicit Generator(QObject *parent = 0);

    void setClassName(const QString& className) { m_className = className; }
    void setUrl(const QString& url) { m_url = url; }
    void setHttpMethod(HttpMethod method) { m_method = method; }
    void setResponseType(ResponseType type) { m_responseType = type; }
    void addParam(const QString& name, ParamType type);
    void setRequiredParam(const QString& name, ParamType type);

    void generate()
    {
        generateHeaderFile();
        generateCppFile();
    }

private:
    struct Parameter
    {
        QString name;
        Generator::ParamType type;
    };

    void generateHeaderFile();
    void generateCppFile();

    QString m_className;
    QString m_url;
    HttpMethod m_method;
    ResponseType m_responseType;
    QList<Parameter> m_params;
    Parameter m_requiredParam;
};

#endif // GENERATOR_H
