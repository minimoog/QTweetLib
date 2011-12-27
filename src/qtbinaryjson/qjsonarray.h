#ifndef QJSONARRAY_H
#define QJSONARRAY_H

#include <qjsonglobal.h>
#include <qvariant.h>
class QStringList;

namespace QtJson
{

class JsonArray
{
public:
    JsonArray();
    ~JsonArray();

    JsonArray(const JsonArray &other);
    JsonArray &operator =(const JsonArray &other);

    static JsonArray fromStringList(const QStringList &list);
    static JsonArray fromVariantList(const QVariantList &list);
    QVariantList toVariantList() const;

    int size() const;
    bool isEmpty() const;
    JsonValue at(int i) const;
    JsonValue first() const;
    JsonValue last() const;

    void append(const JsonValue &value);
    void removeAt(int i);
    JsonValue takeAt(int i);

    void insert(int i, const JsonValue &value);

    bool contains(const JsonValue &element) const;
//    JsonValue &operator[](int i);
    JsonValue operator[](int i) const;

    bool operator==(const JsonArray &other) const;
    bool operator!=(const JsonArray &other) const;

    void detach(uint reserve = 0);

private:
    friend class Data;
    friend class JsonValue;
    friend class JsonDocument;
    JsonArray(Data *data, Array *array);
    Data *d;
    Array *a;
};

}

#endif // QJSONARRAY_H
