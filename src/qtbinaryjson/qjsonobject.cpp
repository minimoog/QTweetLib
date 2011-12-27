#include <qjsonobject.h>
#include <qjsonvalue.h>
#include <qjsonarray.h>
#include <qjson_p.h>
#include <qjsonwriter_p.h>
#include <qstringlist.h>
#include <qdebug.h>

using namespace QtJson;

JsonObject::JsonObject()
    : d(0), o(0)
{
}

JsonObject::JsonObject(Data *data, Object *object)
    : d(data), o(object)
{
    Q_ASSERT(d);
    Q_ASSERT(o);
    d->ref.ref();
}


JsonObject::~JsonObject()
{
    if (d && !d->ref.deref())
        delete d;
}

JsonObject::JsonObject(const JsonObject &other)
{
    d = other.d;
    o = other.o;
    if (d)
        d->ref.ref();
}

JsonObject &JsonObject::operator =(const JsonObject &other)
{
    if (d != other.d) {
        if (d && !d->ref.deref())
            delete d;
        d = other.d;
        o = other.o;
        if (d)
            d->ref.ref();
    }

    return *this;
}

JsonObject JsonObject::fromVariantMap(const QVariantMap &map)
{
    // ### this is implemented the trivial way, not the most efficient way

    JsonObject object;
    for (QVariantMap::const_iterator it = map.constBegin(); it != map.constEnd(); ++it)
        object.insert(it.key(), JsonValue::fromVariant(it.value()));
    return object;
}

QVariantMap JsonObject::toVariantMap() const
{
    QVariantMap map;
    for (uint i = 0; i < o->length; ++i) {
        Entry *e = o->entryAt(i);
        map.insert(e->key(), JsonValue(d, o, e->value).toVariant());
    }
    return map;
}

QByteArray JsonObject::toJson() const
{
    QByteArray json;
    QJsonWriter::objectToJson(o, json, 0);
    return json;
}


QStringList JsonObject::keys() const
{
    if (!d)
        return QStringList();

    QStringList keys;

    for (uint i = 0; i < o->length; ++i) {
        Entry *e = o->entryAt(i);
        keys.append(e->key());
    }

    return keys;
}

int JsonObject::numKeys() const
{
    if (!d)
        return 0;

    return o->length;
}

bool JsonObject::isEmpty() const
{
    if (!d)
        return true;

    return o->length == 0;
}

JsonValue JsonObject::value(const QString &key) const
{
    if (!d)
        return JsonValue();

    for (uint i = 0; i < o->length; ++i) {
        Entry *e = o->entryAt(i);
        if (e->value.latinKey) {
            if (e->shallowLatin1Key() == key)
                return JsonValue(d, o, e->value);
        } else {
            if (e->shallowKey() == key)
                return JsonValue(d, o, e->value);
        }
    }
    return JsonValue(UndefinedValue);
}

void JsonObject::insert(const QString &key, const JsonValue &value)
{
    if (value.t == UndefinedValue) {
        remove(key);
        return;
    }

    bool latinOrIntValue;
    int valueSize = value.requiredStorage(&latinOrIntValue);

    bool latinKey = useCompressed(key);
    int valueOffset = sizeof(Entry) + qStringSize(key, latinKey);
    int requiredSize = valueOffset + valueSize;

    detach(requiredSize + sizeof(offset)); // offset for the new index entry

    if (!o->length)
        o->tableOffset = sizeof(Object);

    // ### this is crude!
    remove(key);

    int pos = o->length;
    o->reserveSpace(requiredSize, pos, 1);
    Entry *e = o->entryAt(pos);
    e->size = requiredSize;
    e->value.type = value.t;
    e->value.latinKey = latinKey;
    e->value.latinOrIntValue = latinOrIntValue;
    e->value.val = value.valueToStore((char *)e - (char *)o + valueOffset);
    copyString((char *)(e + 1), key, latinKey);
    if (valueSize)
        value.copyData((char *)e + valueOffset, latinOrIntValue);
}

void JsonObject::remove(const QString &key)
{
    if (!d)
        return;

    int index = o->indexOf(key);
    if (index < 0)
        return;

    o->removeItems(index, 1);
    d->needsCompaction = true;
}

JsonValue JsonObject::take(const QString &key)
{
    if (!o)
        return JsonValue(UndefinedValue);

    int index = o->indexOf(key);
    if (index < 0)
        return JsonValue(UndefinedValue);

    Entry *e = o->entryAt(index);
    o->removeItems(index, 1);
    d->needsCompaction = true;
    return JsonValue(d, o, e->value);
}

bool JsonObject::contains(const QString &key) const
{
    if (!o)
        return false;

    return o->indexOf(key) >= 0;
}

bool JsonObject::operator==(const JsonObject &other) const
{
    if (o == other.o)
        return true;

    if (!o)
        return other.o->length == 0;
    if (!other.o)
        return o->length == 0;
    if (o->length != other.o->length)
        return false;

    for (uint i = 0; i < o->length; ++i) {
        Entry *e = o->entryAt(i);
        JsonValue v(d, o, e->value);
        if (other.value(e->key()) != v)
            return false;
    }

    return true;
}

bool JsonObject::operator!=(const JsonObject &other) const
{
    return !(*this == other);
}

void JsonObject::detach(uint reserve)
{
    if (!d) {
        d = new Data(reserve, ObjectValue);
        o = static_cast<Object *>(d->header->root());
        d->ref.ref();
        return;
    }
    Data *x = d->detach(o, reserve);
    x->ref.ref();
    if (!d->ref.deref())
        delete d;
    d = x;
    o = static_cast<Object *>(d->header->root());
}
