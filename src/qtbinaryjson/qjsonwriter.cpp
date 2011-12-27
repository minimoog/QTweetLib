#include <qjsonwriter_p.h>
#include <qjson_p.h>

using namespace QtJson;

static void objectContentToJson(const Object *o, QByteArray &json, int indent);
static void arrayContentToJson(const Array *a, QByteArray &json, int indent);

// some code from qutfcodec.cpp, inlined here for performance reasons
// to allow fast escaping of strings
static inline bool isUnicodeNonCharacter(uint ucs4)
{
    // Unicode has a couple of "non-characters" that one can use internally,
    // but are not allowed to be used for text interchange.
    //
    // Those are the last two entries each Unicode Plane (U+FFFE, U+FFFF,
    // U+1FFFE, U+1FFFF, etc.) as well as the entries between U+FDD0 and
    // U+FDEF (inclusive)

    return (ucs4 & 0xfffe) == 0xfffe
            || (ucs4 - 0xfdd0U) < 16;
}

static inline uchar hexdig(uint u)
{
    return (u < 0xa ? '0' + u : 'a' + u - 0xa);
}

static QByteArray escapedString(const QString &s)
{
    const uchar replacement = '?';
    QByteArray ba(s.length(), Qt::Uninitialized);

    uchar *cursor = (uchar *)ba.data();
    const uchar *ba_end = cursor + ba.length();

    const QChar *ch = (const QChar *)s.constData();
    const QChar *end = ch + s.length();

    int surrogate_high = -1;

    while (ch < end) {
        if (cursor >= ba_end - 6) {
            // ensure we have enough space
            int pos = cursor - (uchar *)ba.constData();
            ba.resize(ba.size()*2);
            cursor = (uchar *)ba.data() + pos;
            ba_end = (const uchar *)ba.constData() + ba.length();
        }

        uint u = ch->unicode();
        if (surrogate_high >= 0) {
            if (ch->isLowSurrogate()) {
                u = QChar::surrogateToUcs4(surrogate_high, u);
                surrogate_high = -1;
            } else {
                // high surrogate without low
                *cursor = replacement;
                ++ch;
                surrogate_high = -1;
                continue;
            }
        } else if (ch->isLowSurrogate()) {
            // low surrogate without high
            *cursor = replacement;
            ++ch;
            continue;
        } else if (ch->isHighSurrogate()) {
            surrogate_high = u;
            ++ch;
            continue;
        }

        if (u < 0x80) {
            if (u < 0x20 || u == 0x22 || u == 0x5c) {
                *cursor++ = '\\';
                switch (u) {
                case 0x22:
                    *cursor++ = '"';
                    break;
                case 0x5c:
                    *cursor++ = '\\';
                    break;
                case 0x8:
                    *cursor++ = 'b';
                    break;
                case 0xc:
                    *cursor++ = 'f';
                    break;
                case 0xa:
                    *cursor++ = 'n';
                    break;
                case 0xd:
                    *cursor++ = 'r';
                    break;
                case 0x9:
                    *cursor++ = 't';
                    break;
                default:
                    *cursor++ = 'u';
                    *cursor++ = '0';
                    *cursor++ = '0';
                    *cursor++ = hexdig(u>>4);
                    *cursor++ = hexdig(u & 0xf);
               }
            } else {
                *cursor++ = (uchar)u;
            }
        } else {
            if (u < 0x0800) {
                *cursor++ = 0xc0 | ((uchar) (u >> 6));
            } else {
                // is it one of the Unicode non-characters?
                if (isUnicodeNonCharacter(u)) {
                    *cursor++ = replacement;
                    ++ch;
                    continue;
                }

                if (u > 0xffff) {
                    *cursor++ = 0xf0 | ((uchar) (u >> 18));
                    *cursor++ = 0x80 | (((uchar) (u >> 12)) & 0x3f);
                } else {
                    *cursor++ = 0xe0 | (((uchar) (u >> 12)) & 0x3f);
                }
                *cursor++ = 0x80 | (((uchar) (u >> 6)) & 0x3f);
            }
            *cursor++ = 0x80 | ((uchar) (u&0x3f));
        }
        ++ch;
    }

    ba.resize(cursor - (const uchar *)ba.constData());
    return ba;
}

static void valueToJson(const Base *b, const Value &v, QByteArray &json, int indent)
{
    ValueType type = (ValueType)v.type;
    switch (type) {
    case BooleanValue:
        json += v.val ? "true" : "false";
        break;
    case NumberValue:
        json += QByteArray::number(v.toNumber(b));
        break;
    case StringValue:
        json += '"';
        json += escapedString(v.toString(b));
        json += '"';
        break;
    case ArrayValue:
        json += "[\n";
        arrayContentToJson(static_cast<Array *>(v.objectOrArray(b)), json, indent + 1);
        json += QByteArray(4*indent, ' ');
        json += "]";
        break;
    case ObjectValue:
        json += "{\n";
        objectContentToJson(static_cast<Object *>(v.objectOrArray(b)), json, indent + 1);
        json += QByteArray(4*indent, ' ');
        json += "}";
        break;
    case NullValue:
    default:
        json += "null";
    }
}

static void arrayContentToJson(const Array *a, QByteArray &json, int indent)
{
    if (!a || !a->length)
        return;

    QByteArray indentString(4*indent, ' ');

    uint i = 0;
    while (1) {
        json += indentString;
        valueToJson(a, a->at(i), json, indent);

        if (++i == a->length) {
            json += '\n';
            break;
        }

        json += ",\n";
    }
}


static void objectContentToJson(const Object *o, QByteArray &json, int indent)
{
    if (!o || !o->length)
        return;

    QByteArray indentString(4*indent, ' ');

    uint i = 0;
    while (1) {
        Entry *e = o->entryAt(i);
        json += indentString;
        json += '"';
        json += escapedString(e->key());
        json += "\": ";
        valueToJson(o, e->value, json, indent);

        if (++i == o->length) {
            json += '\n';
            break;
        }

        json += ",\n";
    }
}

void QJsonWriter::objectToJson(const Object *o, QByteArray &json, int indent)
{
    json.reserve(json.size() + (o ? o->size : 16));
    json += "{\n";
    objectContentToJson(o, json, indent + 1);
    json += QByteArray(4*indent, ' ');
    json += "}\n";
}

void QJsonWriter::arrayToJson(const Array *a, QByteArray &json, int indent)
{
    json.reserve(json.size() + (a ? a->size : 16));
    json += "[\n";
    arrayContentToJson(a, json, indent + 1);
    json += QByteArray(4*indent, ' ');
    json += "]\n";
}
