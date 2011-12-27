#ifndef QJSONOBJECT_H
#define QJSONOBJECT_H

#include <qjsonglobal.h>
#include <qvariant.h>

namespace QtJson {

class JsonObject
{
public:
    JsonObject();
    ~JsonObject();

    JsonObject(const JsonObject &other);
    JsonObject &operator =(const JsonObject &other);

    static JsonObject fromVariantMap(const QVariantMap &map);
    QVariantMap toVariantMap() const;

    QByteArray toJson() const;

    QStringList keys() const;
    int numKeys() const;
    bool isEmpty() const;

    // ### rather use an iterator?
//    QString keyAt(int);
//    QSonValue valueAt(int)

    JsonValue value(const QString & key) const;

    void insert(const QString &key, const JsonValue &value);
    void remove(const QString &key);
    JsonValue take(const QString &key);
    bool contains(const QString &key) const;

    bool operator==(const JsonObject &other) const;
    bool operator!=(const JsonObject &other) const;

    void detach(uint reserve = 0);

private:
    friend class Data;
    friend class JsonValue;
    friend class JsonDocument;
    friend class QJsonParser;
    JsonObject(Data *data, Object *object);

    Data *d;
    Object *o;
};

}

#endif // QJSONOBJECT_H
