#include <qjsondocument.h>
#include <qjsonobject.h>
#include <qjsonvalue.h>
#include <qjsonarray.h>
#include <qjson_p.h>
#include <qjsonwriter_p.h>
#include <qjsonparser_p.h>
#include <qstringlist.h>

using namespace QtJson;


JsonDocument::JsonDocument()
    : d(0)
{
}

JsonDocument::JsonDocument(const JsonObject &object)
    : d(object.d)
{
    if (!d)
        d = new Data(0, ObjectValue);
    d->ref.ref();
}

JsonDocument::JsonDocument(const JsonArray &array)
    : d(array.d)
{
    if (!d)
        d = new Data(0, ArrayValue);
    d->ref.ref();
}

JsonDocument::JsonDocument(Data *data)
    : d(data)
{
    Q_ASSERT(d);
    d->ref.ref();
}


JsonDocument::~JsonDocument()
{
    if (d && !d->ref.deref())
        delete d;
}

JsonDocument::JsonDocument(const JsonDocument &other)
{
    d = other.d;
    if (d)
        d->ref.ref();
}

JsonDocument &JsonDocument::operator =(const JsonDocument &other)
{
    if (d != other.d) {
        if (d && !d->ref.deref())
            delete d;
        d = other.d;
        if (d)
            d->ref.ref();
    }

    return *this;
}

JsonDocument JsonDocument::fromBinaryData(const QByteArray &data)
{
    Header *h = (Header *) data.constData();

    if (data.size() < (int)(sizeof(Header) + sizeof(Base)) ||
        h->tag != QBJS_Tag || h->version != 1 ||
        sizeof(Header) + h->root()->size > (uint)data.size())
        return JsonDocument();

    char *raw = (char *)malloc(data.size());
    memcpy(raw, data.constData(), data.size());
    Data *d = new Data(raw, data.size());

    return JsonDocument(d);
}

JsonDocument JsonDocument::fromVariant(const QVariant &v)
{
    JsonDocument doc;
    if (v.type() == QVariant::Map) {
        doc.setObject(JsonObject::fromVariantMap(v.toMap()));
    } else if (v.type() == QVariant::List) {
        doc.setArray(JsonArray::fromVariantList(v.toList()));
    } else if (v.type() == QVariant::StringList) {
        doc.setArray(JsonArray::fromStringList(v.toStringList()));
    }
    return doc;
}

QVariant JsonDocument::toVariant() const
{
    if (!d)
        return QVariant();

    if (d->header->root()->isArray())
        return JsonArray(d, static_cast<Array *>(d->header->root())).toVariantList();
    else
        return JsonObject(d, static_cast<Object *>(d->header->root())).toVariantMap();
}

QByteArray JsonDocument::toJson() const
{
    if (!d)
        return QByteArray();

    QByteArray json;

    if (d->header->root()->isArray())
        QJsonWriter::arrayToJson(static_cast<Array *>(d->header->root()), json, 0);
    else
        QJsonWriter::objectToJson(static_cast<Object *>(d->header->root()), json, 0);

    return json;
}

JsonDocument JsonDocument::fromJson(const QByteArray &json)
{
    QJsonParser parser(json.constData(), json.length());
    return parser.parse();
}

bool JsonDocument::isEmpty() const
{
    if (!d)
        return true;

    return false;
}

QByteArray JsonDocument::toBinaryData() const
{
    if (!d || !d->rawData)
        return QByteArray();

    return QByteArray(d->rawData, d->header->root()->size + sizeof(Header));
}

ValueType JsonDocument::type() const
{
    if (!d)
        return NullValue;

    Header *h = (Header *)d->rawData;
    return h->root()->isArray() ? ArrayValue : ObjectValue;
}

JsonObject JsonDocument::object() const
{
    if (d) {
        Base *b = d->header->root();
        if (b->isObject())
            return JsonObject(d, static_cast<Object *>(b));
    }
    return JsonObject();
}

JsonArray JsonDocument::array() const
{
    if (d) {
        Base *b = d->header->root();
        if (b->isArray())
            return JsonArray(d, static_cast<Array *>(b));
    }
    return JsonArray();
}

void JsonDocument::setObject(const JsonObject &object)
{
    if (d && !d->ref.deref())
        delete d;

    d = object.d;
    if (!d)
        d = new Data(0, ObjectValue);
    d->ref.ref();
}

void JsonDocument::setArray(const JsonArray &array)
{
    if (d && !d->ref.deref())
        delete d;

    d = array.d;
    if (!d)
        d = new Data(0, ArrayValue);
    d->ref.ref();
}


bool JsonDocument::operator==(const JsonDocument &other) const
{
    if (d == other.d)
        return true;

    if (!d || !other.d)
        return false;
    if (d->header->root()->isArray() != other.d->header->root()->isArray())
        return false;

    if (d->header->root()->isArray())
        return JsonObject(d, static_cast<Object *>(d->header->root()))
                == JsonObject(other.d, static_cast<Object *>(other.d->header->root()));
    else
        return JsonArray(d, static_cast<Array *>(d->header->root()))
                == JsonArray(other.d, static_cast<Array *>(other.d->header->root()));
}

bool JsonDocument::operator!=(const JsonDocument &other) const
{
    return !(*this == other);
}
