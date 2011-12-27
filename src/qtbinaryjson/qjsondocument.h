#ifndef QJSONDOCUMENT_H
#define QJSONDOCUMENT_H

#include <qjsonglobal.h>
#include <qvariant.h>

namespace QtJson {

class JsonDocument
{
public:
    JsonDocument();
    JsonDocument(const JsonObject &object);
    JsonDocument(const JsonArray &array);
    ~JsonDocument();

    JsonDocument(const JsonDocument &other);
    JsonDocument &operator =(const JsonDocument &other);

    static JsonDocument fromBinaryData(const QByteArray &data);
    QByteArray toBinaryData() const;

    static JsonDocument fromVariant(const QVariant &v);
    QVariant toVariant() const;

    QByteArray toJson() const;
    static JsonDocument fromJson(const QByteArray &json);

    bool isEmpty() const;

    ValueType type() const;

    JsonObject object() const;
    JsonArray array() const;

    void setObject(const JsonObject &object);
    void setArray(const JsonArray &array);

    bool operator==(const JsonDocument &other) const;
    bool operator!=(const JsonDocument &other) const;

private:
    friend class Data;
    friend class JsonValue;
    friend class QJsonParser;
    JsonDocument(Data *data);

    Data *d;
};

}

#endif // QJSONDOCUMENT_H
