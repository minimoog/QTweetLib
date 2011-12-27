#include <qjsonobject.h>
#include <qjsonvalue.h>
#include <qjsonarray.h>
#include <qjson_p.h>
#include <qjsonarray.h>

#include <qvariant.h>
#include <qstringlist.h>
#include <qdebug.h>

using namespace QtJson;

static const Base emptyArray = { sizeof(Base), 0, false, 0 };
static const Base emptyObject = { sizeof(Base), 0, true, 0 };

JsonValue::JsonValue(ValueType type)
    : t(type), d(0), dbl(0.)
{
}

JsonValue::JsonValue(Data *data, Base *base, const Value &v)
    : d(0)
{
    t = (ValueType)v.type;
    switch (t) {
    case UndefinedValue:
    case NullValue:
        dbl = 0;
        break;
    case BooleanValue:
        b = v.val;
        break;
    case NumberValue:
        dbl = v.toNumber(base);
        break;
    case StringValue: {
        QString s = v.toString(base);
        stringData = *reinterpret_cast<QStringData **>(&s);
        stringData->ref.ref();
        break;
    }
    case ArrayValue:
    case ObjectValue:
        d = data;
        this->base = v.objectOrArray(base);
        break;
    }
    if (d)
        d->ref.ref();
}

JsonValue::JsonValue(bool b)
    : t(BooleanValue), d(0)
{
    this->b = b;
}

JsonValue::JsonValue(double n)
    : t(NumberValue), d(0)
{
    this->dbl = n;
}

JsonValue::JsonValue(int n)
    : t(NumberValue), d(0)
{
    this->dbl = n;
}

JsonValue::JsonValue(const QString &s)
    : t(StringValue), d(0)
{
    stringData = *(QStringData **)(&s);
    stringData->ref.ref();
}

JsonValue::JsonValue(const QLatin1String &s)
    : t(StringValue), d(0)
{
    // ### FIXME: Avoid creating the temp QString below
    QString str(s);
    stringData = *(QStringData **)(&str);
    stringData->ref.ref();
}

JsonValue::JsonValue(const JsonArray &a)
    : t(ArrayValue), d(a.d)
{
    array = a.a;
    if (d)
        d->ref.ref();
}

JsonValue::JsonValue(const JsonObject &o)
    : t(ObjectValue), d(o.d)
{
    object = o.o;
    if (d)
        d->ref.ref();
}


JsonValue::~JsonValue()
{
    if (t == StringValue && stringData && !stringData->ref.deref())
        free(stringData);

    if (d && !d->ref.deref())
        delete d;
}

JsonValue::JsonValue(const JsonValue &other)
{
    t = other.t;
    d = other.d;
    memcpy(raw, other.raw, sizeof(double));
    if (d)
        d->ref.ref();

    if (t == StringValue && stringData)
        stringData->ref.ref();
}

JsonValue &JsonValue::operator =(const JsonValue &other)
{
    if (t == StringValue && stringData && !stringData->ref.deref())
        free(stringData);

    t = other.t;
    dbl = other.dbl;

    if (d != other.d) {

        if (d && !d->ref.deref())
            delete d;
        d = other.d;
        if (d)
            d->ref.ref();

    }

    if (t == StringValue && stringData)
        stringData->ref.ref();

    return *this;
}

JsonValue JsonValue::fromVariant(const QVariant &variant)
{
    switch (variant.type()) {
    case QVariant::Bool:
        return JsonValue(variant.toBool());
    case QVariant::Int:
        return JsonValue(variant.toInt());
    case QVariant::Double:
    case QVariant::LongLong:
    case QVariant::ULongLong:
    case QVariant::UInt:
        return JsonValue(variant.toDouble());
    case QVariant::String:
        return JsonValue(variant.toString());
    case QVariant::StringList:
        return JsonValue(JsonArray::fromStringList(variant.toStringList()));
    case QVariant::List:
        return JsonValue(JsonArray::fromVariantList(variant.toList()));
    case QVariant::Map:
        return JsonValue(JsonObject::fromVariantMap(variant.toMap()));
    default:
        break;
    }
    QString string = variant.toString();
    if (string.isEmpty())
        return JsonValue();
    return JsonValue(string);
}

QVariant JsonValue::toVariant() const
{
    switch (t) {
    case BooleanValue:
        return b;
    case NumberValue:
        return dbl;
    case StringValue:
        return toString();
    case ArrayValue:
        return JsonArray(d, array).toVariantList();
    case ObjectValue:
        return JsonObject(d, object).toVariantMap();
    case NullValue:
    case UndefinedValue:
        break;
    }
    return QVariant();
}


ValueType JsonValue::type() const
{
    return t;
 }

void JsonValue::setValue(bool b)
{
    *this = JsonValue(b);
}

void JsonValue::setValue(double d)
{
    *this = JsonValue(d);
}

void JsonValue::setValue(int i)
{
    *this = JsonValue(i);
}

void JsonValue::setValue(const QString &s)
{
    *this = JsonValue(s);
}

void JsonValue::setValue(const QLatin1String &s)
{
    *this = JsonValue(s);
}

void JsonValue::setValue(const JsonArray &a)
{
    *this = JsonValue(a);
}

void JsonValue::setValue(const JsonObject &o)
{
    *this = JsonValue(o);
}

bool JsonValue::toBool() const
{
    if (t != BooleanValue)
        return false;
    return b;
}

double JsonValue::toNumber() const
{
    if (t != NumberValue)
        return 0;
    return dbl;
}

int JsonValue::toInt() const
{
    if (t != NumberValue)
        return 0;
    return (int)dbl;
}

QString JsonValue::toString() const
{
    if (t != StringValue)
        return QString();
    stringData->ref.ref(); // the constructor below doesn't add a ref.
    return QString(*(const QConstStringData<1> *)stringData);
}

JsonArray JsonValue::toArray() const
{
    if (!d || t != ArrayValue)
        return JsonArray();

    return JsonArray(d, array);
}

JsonObject JsonValue::toObject() const
{
    if (!d || t != ObjectValue)
        return JsonObject();

    return JsonObject(d, object);
}

bool JsonValue::operator==(const JsonValue &other) const
{
    if (t != other.t)
        return false;

    switch (t) {
    case UndefinedValue:
    case NullValue:
        break;
    case BooleanValue:
        return b == other.b;
    case NumberValue:
        return dbl == other.dbl;
    case StringValue:
        return toString() == other.toString();
    case ArrayValue:
        return JsonArray(d, array) == JsonArray(other.d, other.array);
    case ObjectValue:
        return JsonObject(d, object) == JsonObject(other.d, other.object);
    }
    return true;
}

bool JsonValue::operator!=(const JsonValue &other) const
{
    return !(*this == other);
}

void JsonValue::detach()
{
    if (!d)
        return;

    Data *x = d->detach(object);
    x->ref.ref();
    if (!d->ref.deref())
        delete d;
    d = x;
    object = static_cast<Object *>(d->header->root());
}

int JsonValue::requiredStorage(bool *compressed) const
{
    *compressed = false;
    switch (t) {
    case NumberValue:
        if (compressedNumber(dbl) != INT_MAX) {
            *compressed = true;
            return 0;
        }
        return sizeof(double);
    case StringValue: {
        QString s = toString();
        *compressed = useCompressed(s);
        return qStringSize(s, *compressed);
    }
    case ArrayValue:
    case ObjectValue:
        return base ? base->size : sizeof(Base);
    case UndefinedValue:
    case NullValue:
    case BooleanValue:
        break;
    }
    return 0;
}

uint JsonValue::valueToStore(uint offset) const
{
    switch (t) {
    case UndefinedValue:
    case NullValue:
        break;
    case BooleanValue:
        return b;
    case NumberValue: {
        int c = compressedNumber(dbl);
        if (c != INT_MAX)
            return c;
    }
    case StringValue:
    case ArrayValue:
    case ObjectValue:
        return offset;
    }
    return 0;
}

void JsonValue::copyData(char *dest, bool compressed) const
{
    switch (t) {
    case NumberValue:
        if (!compressed)
            memcpy(dest, raw, sizeof(dbl));
        break;
    case StringValue: {
        QString str = toString();
        copyString(dest, str, compressed);
        break;
    }
    case ArrayValue:
    case ObjectValue: {
        const Base *b = base;
        if (!b)
            b = (t == ArrayValue ? &emptyArray : &emptyObject);
        memcpy(dest, b, b->size);
        break;
    }
    default:
        break;
    }
}
