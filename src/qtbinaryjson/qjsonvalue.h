#ifndef QJSONVALUE_H
#define QJSONVALUE_H

#include <qjsonglobal.h>

class QVariant;

namespace QtJson {

class JsonValue {
public:
    JsonValue(ValueType = NullValue);
    JsonValue(bool b);
    JsonValue(double n);
    JsonValue(int n);
    JsonValue(const QString &s);
    JsonValue(const QLatin1String &s);
    JsonValue(const JsonArray &a);
    JsonValue(const JsonObject &o);

    ~JsonValue();

    JsonValue(const JsonValue &other);
    JsonValue &operator =(const JsonValue &other);

    static JsonValue fromVariant(const QVariant &variant);
    QVariant toVariant() const;

    ValueType type() const;
    inline bool isUndefined() const { return type() == UndefinedValue; }

    void setValue(bool);
    void setValue(double);
    void setValue(int);
    void setValue(const QString &);
    void setValue(const QLatin1String &);
    void setValue(const JsonArray &);
    void setValue(const JsonObject &);

    bool toBool() const;
    double toNumber() const;
    int toInt() const;
    QString toString() const;
    JsonArray toArray() const;
    JsonObject toObject() const;

    bool operator==(const JsonValue &other) const;
    bool operator!=(const JsonValue &other) const;

    void detach();

private:
    // avoid implicit conversions from char * to bool
    inline JsonValue(const void *) {}
    friend class Data;
    friend class Value;
    friend class JsonArray;
    friend class JsonObject;
    JsonValue(Data *d, Base *b, const Value& v);
    int requiredStorage(bool *compressed) const;
    uint valueToStore(uint offset) const;
    void copyData(char *dest, bool compressed) const;

    ValueType t;
    Data *d; // needed for Objects and Arrays
    union {
        char raw[sizeof(double)];
        bool b;
        double dbl;
        QStringData *stringData;
        Base *base;
        Object *object;
        Array *array;
    };
};

}
#endif // QJSONVALUE_H
