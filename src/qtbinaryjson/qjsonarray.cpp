#include <qjsonobject.h>
#include <qjsonvalue.h>
#include <qjsonarray.h>
#include <qjson_p.h>
#include <qjsonvalue.h>

#include <qstringlist.h>

using namespace QtJson;

JsonArray::JsonArray()
    : d(0), a(0)
{
}

JsonArray::JsonArray(Data *data, Array *array)
    : d(data), a(array)
{
    d->ref.ref();
}

JsonArray::~JsonArray()
{
    if (d && !d->ref.deref())
        delete d;
}

JsonArray::JsonArray(const JsonArray &other)
{
    d = other.d;
    a = other.a;
    if (d)
        d->ref.ref();
}

JsonArray &JsonArray::operator =(const JsonArray &other)
{
    if (d != other.d) {
        if (d && !d->ref.deref())
            delete d;
        d = other.d;
        a = other.a;
        if (d)
            d->ref.ref();
    }

    return *this;
}

JsonArray JsonArray::fromStringList(const QStringList &list)
{
    JsonArray array;
    for (QStringList::const_iterator it = list.constBegin(); it != list.constEnd(); ++it)
        array.append(JsonValue(*it));
    return array;
}

JsonArray JsonArray::fromVariantList(const QVariantList &list)
{
    JsonArray array;
    for (QVariantList::const_iterator it = list.constBegin(); it != list.constEnd(); ++it)
        array.append(JsonValue::fromVariant(*it));
    return array;
}

QVariantList JsonArray::toVariantList() const
{
    QVariantList list;

    if (a) {
        for (int i = 0; i < (int)a->length; ++i)
            list.append(JsonValue(d, a, a->at(i)).toVariant());
    }
    return list;
}


int JsonArray::size() const
{
    if (!d)
        return 0;

    return (int)a->length;
}

bool JsonArray::isEmpty() const
{
    if (!d)
        return true;

    return a->length == 0;
}

JsonValue JsonArray::at(int i) const
{
    if (!a || i < 0 || i >= (int)a->length)
        return JsonValue(UndefinedValue);

    return JsonValue(d, a, a->at(i));
}

JsonValue JsonArray::first() const
{
    return at(0);
}

JsonValue JsonArray::last() const
{
    return at(a ? (a->length - 1) : 0);
}

void JsonArray::append(const JsonValue &value)
{
    insert(a ? a->length : 0, value);
}

void JsonArray::removeAt(int i)
{
    if (!a || i < 0 || i >= (int)a->length)
        return;

    detach();
    a->removeItems(i, 1);
    d->needsCompaction = true;
}

JsonValue JsonArray::takeAt(int i)
{
    if (!a || i < 0 || i >= (int)a->length)
        return JsonValue(UndefinedValue);

    detach();

    JsonValue v(d, a, a->at(i));
    v.detach();

    removeAt(i);

    return v;
}

void JsonArray::insert(int i, const JsonValue &value)
{
    Q_ASSERT (i >= 0 && i <= (int)(a ? a->length : 0));

    bool compressed;
    int valueSize = value.requiredStorage(&compressed);

    detach(valueSize + sizeof(offset) + sizeof(Value)); // offset for the new index entry

    if (!a->length)
        a->tableOffset = sizeof(Array);

    int valueOffset = a->reserveSpace(valueSize, i, 1);
    Value &v = (*a)[i];
    v.type = (value.t == UndefinedValue ? NullValue : value.t);
    v.latinOrIntValue = compressed;
    v.latinKey = false;
    v.val = value.valueToStore(valueOffset);
    if (valueSize)
        value.copyData((char *)a + valueOffset, compressed);
}

bool JsonArray::contains(const JsonValue &element) const
{
    for (int i = 0; i < size(); i++) {
        if (at(i) == element)
            return true;
    }
    return false;
}

//JsonValue &JsonArray::operator[](int i)
//{
//    Q_ASSERT(i >= 0 && i < a->length);
//    detach();

//}

JsonValue JsonArray::operator[](int i) const
{
    return at(i);
}

bool JsonArray::operator==(const JsonArray &other) const
{
    if (a == other.a)
        return true;

    if (!a)
        return other.a->length == 0;
    if (!other.a)
        return a->length == 0;
    if (a->length != other.a->length)
        return false;

    for (int i = 0; i < (int)a->length; ++i) {
        if (JsonValue(d, a, a->at(i)) != JsonValue(other.d, other.a, other.a->at(i)))
            return false;
    }
    return true;
}

bool JsonArray::operator!=(const JsonArray &other) const
{
    return !(*this == other);
}

void JsonArray::detach(uint reserve)
{
    if (!d) {
        d = new Data(reserve, ArrayValue);
        a = static_cast<Array *>(d->header->root());
        d->ref.ref();
        return;
    }
    Data *x = d->detach(a, reserve);
    x->ref.ref();
    if (!d->ref.deref())
        delete d;
    d = x;
    a = static_cast<Array *>(d->header->root());
}
