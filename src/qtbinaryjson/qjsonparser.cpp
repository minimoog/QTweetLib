#include <qjsonparser_p.h>
#include <qjson_p.h>
#include <qvarlengtharray.h>
#include <qdebug.h>

//#define PARSER_DEBUG
#ifdef PARSER_DEBUG
static int indent = 0;
#define BEGIN qDebug() << QByteArray(4*indent++, ' ').constData() << "pos=" << current
#define END --indent
#define DEBUG qDebug() << QByteArray(4*indent, ' ').constData()
#else
#define BEGIN if (1) ; else qDebug()
#define END do {} while (0)
#define DEBUG if (1) ; else qDebug()
#endif


using namespace QtJson;

QJsonParser::QJsonParser(const char *json, int length)
    : json(json), data(0), dataLength(0), current(0)
{
    end = json + length;
}



/*

begin-array     = ws %x5B ws  ; [ left square bracket

begin-object    = ws %x7B ws  ; { left curly bracket

end-array       = ws %x5D ws  ; ] right square bracket

end-object      = ws %x7D ws  ; } right curly bracket

name-separator  = ws %x3A ws  ; : colon

value-separator = ws %x2C ws  ; , comma

Insignificant whitespace is allowed before or after any of the six
structural characters.

ws = *(
          %x20 /              ; Space
          %x09 /              ; Horizontal tab
          %x0A /              ; Line feed or New line
          %x0D                ; Carriage return
      )

*/

enum {
    Space = 0x20,
    Tab = 0x09,
    LineFeed = 0x0a,
    Return = 0x0d,
    BeginArray = 0x5b,
    BeginObject = 0x7b,
    EndArray = 0x5d,
    EndObject = 0x7d,
    NameSeparator = 0x3a,
    ValueSeparator = 0x2c,
    Quote = 0x22
};



bool QJsonParser::eatSpace()
{
    while (json < end) {
        if (*json > Space)
            break;
        if (*json != Space &&
            *json != Tab &&
            *json != LineFeed &&
            *json != Return)
            break;
        ++json;
    }
    return (json < end);
}

char QJsonParser::nextToken()
{
    if (!eatSpace())
        return 0;
    char token = *json++;
    switch (token) {
    case BeginArray:
    case BeginObject:
    case NameSeparator:
    case ValueSeparator:
        if (!eatSpace())
            return 0;
    case EndArray:
    case EndObject:
        eatSpace();
    case Quote:
        break;
    default:
        token = 0;
        break;
    }
    return token;
}

/*
    JSON-text = object / array
*/
QtJson::JsonDocument QJsonParser::parse()
{
#ifdef PARSER_DEBUG
    indent = 0;
    qDebug() << ">>>>> parser begin";
#endif
    // allocate some space
    dataLength = qMax(end - json, (ptrdiff_t) 256);
    data = (char *)malloc(dataLength);

    // fill in Header data
    Header *h = (Header *)data;
    h->tag = QBJS_Tag;
    h->version = 1;

    current = sizeof(Header);

    char token = nextToken();
    DEBUG << token;
    if (token == BeginArray) {
        if (!parseArray())
            goto error;
    } else if (token == BeginObject) {
        if (!parseObject())
            goto error;
    } else {
        goto error;
    }

    END;
    {
        Data *d = new Data(data, current);
        return JsonDocument(d);
    }

error:
#ifdef PARSER_DEBUG
    qDebug() << ">>>>> parser error";
#endif
    free(data);
    return JsonDocument();
}

/*
    object = begin-object [ member *( value-separator member ) ]
    end-object
*/

bool QJsonParser::parseObject()
{
    int objectOffset = reserveSpace(sizeof(Object));
    BEGIN << "parseObject pos=" << objectOffset << current << json;

    QVarLengthArray<uint> offsets;

    char token = nextToken();
    while (token == Quote) {
        int off = current - objectOffset;
        if (!parseMember(objectOffset))
            return false;
        offsets.append(off);
        token = nextToken();
        if (token != ValueSeparator)
            break;
        token = nextToken();
    }

    DEBUG << "end token=" << token;
    if (token != EndObject)
        return false;

    DEBUG << "numEntries" << offsets.size();
    int table = objectOffset;
    // finalize the object
    if (offsets.size()) {
        int tableSize = offsets.size()*sizeof(uint);
        table = reserveSpace(tableSize);
        memcpy(data + table, offsets.constData(), tableSize);
    }

    Object *o = (Object *)(data + objectOffset);
    o->tableOffset = table - objectOffset;
    o->size = current - objectOffset;
    o->is_object = true;
    o->length = offsets.size();

    DEBUG << "current=" << current;
    END;
    return true;
}

/*
    member = string name-separator value
*/
bool QJsonParser::parseMember(int baseOffset)
{
    int entryOffset = reserveSpace(sizeof(Entry));
    BEGIN << "parseMember pos=" << entryOffset;

    bool latin1;
    if (!parseString(&latin1))
        return false;
    char token = nextToken();
    if (token != NameSeparator)
        return false;
    Value val;
    if (!parseValue(&val, baseOffset))
        return false;

    // finalize the entry
    Entry *e = (Entry *)(data + entryOffset);
    e->value = val;
    e->value.latinKey = latin1;

    END;
    return true;
}

/*
    array = begin-array [ value *( value-separator value ) ] end-array
*/
bool QJsonParser::parseArray()
{
    BEGIN << "parseArray";
    int arrayOffset = reserveSpace(sizeof(Array));

    QVarLengthArray<Value> values;

    if (!eatSpace())
        return false;
    if (*json == EndArray) {
        nextToken();
    } else {
        while (1) {
            Value val;
            if (!parseValue(&val, arrayOffset))
                return false;
            values.append(val);
            char token = nextToken();
            if (token == EndArray)
                break;
            else if (token != ValueSeparator)
                return false;
        }
    }

    DEBUG << "size =" << values.size();
    int table = arrayOffset;
    // finalize the object
    if (values.size()) {
        int tableSize = values.size()*sizeof(Value);
        table = reserveSpace(tableSize);
        memcpy(data + table, values.constData(), tableSize);
    }

    Array *a = (Array *)(data + arrayOffset);
    a->tableOffset = table - arrayOffset;
    a->size = current - arrayOffset;
    a->is_object = false;
    a->length = values.size();

    DEBUG << "current=" << current;
    END;
    return true;
}

/*
value = false / null / true / object / array / number / string

*/

bool QJsonParser::parseValue(Value *val, int baseOffset)
{
    BEGIN << "parse Value" << json;
    val->int_val = 0;
    val->unused = 0;

    switch (*json++) {
    case 'n':
        if (end - json < 4)
            return false;
        if (*json++ == 'u' &&
            *json++ == 'l' &&
            *json++ == 'l') {
            val->type = NullValue;
            DEBUG << "value: null";
            END;
            return true;
        }
        return false;
    case 't':
        if (end - json < 4)
            return false;
        if (*json++ == 'r' &&
            *json++ == 'u' &&
            *json++ == 'e') {
            val->type = BooleanValue;
            val->val = true;
            DEBUG << "value: true";
            END;
            return true;
        }
        return false;
    case 'f':
        if (end - json < 5)
            return false;
        if (*json++ == 'a' &&
            *json++ == 'l' &&
            *json++ == 's' &&
            *json++ == 'e') {
            val->type = BooleanValue;
            val->val = false;
            DEBUG << "value: false";
            END;
            return true;
        }
        return false;
    case Quote: {
        val->type = StringValue;
        val->val = current - baseOffset;
        bool latin1;
        if (!parseString(&latin1))
            return false;
        val->latinOrIntValue = latin1;
        DEBUG << "value: string";
        END;
        return true;
    }
    case BeginArray:
        val->type = ArrayValue;
        val->val = current - baseOffset;
        if (!parseArray())
            return false;
        DEBUG << "value: array";
        END;
        return true;
    case BeginObject:
        val->type = ObjectValue;
        val->val = current - baseOffset;
        if (!parseObject())
            return false;
        DEBUG << "value: object";
        END;
        return true;
    default:
        --json;
        if (!parseNumber(val, baseOffset))
            return false;
        DEBUG << "value: number";
        END;
    }

    return true;
}





/*
        number = [ minus ] int [ frac ] [ exp ]
        decimal-point = %x2E       ; .
        digit1-9 = %x31-39         ; 1-9
        e = %x65 / %x45            ; e E
        exp = e [ minus / plus ] 1*DIGIT
        frac = decimal-point 1*DIGIT
        int = zero / ( digit1-9 *DIGIT )
        minus = %x2D               ; -
        plus = %x2B                ; +
        zero = %x30                ; 0

*/

bool QJsonParser::parseNumber(Value *val, int baseOffset)
{
    BEGIN << "parseNumber" << json;
    val->type = NumberValue;

    const char *start = json;
    bool isInt = true;

    // minus
    if (json < end && *json == '-')
        ++json;

    // int = zero / ( digit1-9 *DIGIT )
    if (json < end && *json == '0') {
        ++json;
    } else {
        while (json < end && *json >= '0' && *json <= '9')
            ++json;
    }

    // frac = decimal-point 1*DIGIT
    if (json < end && *json == '.') {
        isInt = false;
        ++json;
        while (json < end && *json >= '0' && *json <= '9')
            ++json;
    }

    // exp = e [ minus / plus ] 1*DIGIT
    if (json < end && (*json == 'e' || *json == 'E')) {
        isInt = false;
        ++json;
        if (json < end && (*json == '-' || *json == '+'))
            ++json;
        while (json < end && *json >= '0' && *json <= '9')
            ++json;
    }

    if (json >= end)
        return false;

    QByteArray number(start, json - start);
    DEBUG << "numberstring" << number;

    if (isInt) {
        bool ok;
        int n = number.toInt(&ok);
        if (ok && n < (1<<25) && n > -(1<<25)) {
            val->int_val = n;
            val->latinOrIntValue = true;
            END;
            return true;
        }
    }

    bool ok;
    union {
        char raw[sizeof(double)];
        double d;
    };
    d = number.toDouble(&ok);

    if (!ok)
        return false;

    int pos = reserveSpace(sizeof(double));
    memcpy(data + pos, raw, sizeof(double));
    val->val = pos - baseOffset;
    val->latinOrIntValue = false;

    END;
    return true;
}

/*

        string = quotation-mark *char quotation-mark

        char = unescaped /
               escape (
                   %x22 /          ; "    quotation mark  U+0022
                   %x5C /          ; \    reverse solidus U+005C
                   %x2F /          ; /    solidus         U+002F
                   %x62 /          ; b    backspace       U+0008
                   %x66 /          ; f    form feed       U+000C
                   %x6E /          ; n    line feed       U+000A
                   %x72 /          ; r    carriage return U+000D
                   %x74 /          ; t    tab             U+0009
                   %x75 4HEXDIG )  ; uXXXX                U+XXXX

        escape = %x5C              ; \

        quotation-mark = %x22      ; "

        unescaped = %x20-21 / %x23-5B / %x5D-10FFFF
 */
static inline bool addHexDigit(char digit, uint *result)
{
    *result <<= 4;
    if (digit >= '0' && digit <= '9')
        *result |= (digit - '0');
    else if (digit >= 'a' && digit <= 'f')
        *result |= (digit - 'a');
    else if (digit >= 'A' && digit <= 'F')
            *result |= (digit - 'A');
    else
        return false;
    return true;
}

static inline bool scanEscapeSequence(const char *&json, const char *end, uint *ch)
{
    ++json;
    if (json >= end)
        return false;

    DEBUG << "scan escape" << (char)*json;
    uint escaped = *json++;
    switch (escaped) {
    case '"':
        *ch = '"'; break;
    case '\\':
        *ch = '\\'; break;
    case '/':
        *ch = '/'; break;
    case 'b':
        *ch = 0x8; break;
    case 'f':
        *ch = 0xc; break;
    case 'n':
        *ch = 0xa; break;
    case 'r':
        *ch = 0xd; break;
    case 't':
        *ch = 0x9; break;
    case 'u': {
        *ch = 0;
        if (json > end - 4)
            return false;
        for (int i = 0; i < 4; ++i) {
            if (!addHexDigit(*json, ch))
                return false;
            ++json;
        }
        return true;
    }
    default:
        // ### this is not as strict as one could be, but allows for more Json files
        // to be parsed correctly.
        *ch = escaped;
        return true;
    }
    return true;
}

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

static inline bool scanUtf8Char(const char *&json, const char *end, uint *result)
{
    int need;
    uint min_uc;
    uint uc;
    uchar ch = *json++;
    if (ch < 128) {
        *result = ch;
        return true;
    } else if ((ch & 0xe0) == 0xc0) {
        uc = ch & 0x1f;
        need = 1;
        min_uc = 0x80;
    } else if ((ch & 0xf0) == 0xe0) {
        uc = ch & 0x0f;
        need = 2;
        min_uc = 0x800;
    } else if ((ch&0xf8) == 0xf0) {
        uc = ch & 0x07;
        need = 3;
        min_uc = 0x10000;
    } else {
        return false;
    }

    if (json >= end - need)
        return false;

    for (int i = 0; i < need; ++i) {
        ch = *json++;
        if ((ch&0xc0) != 0x80)
            return false;
        uc = (uc << 6) | (ch & 0x3f);
    }

    if (isUnicodeNonCharacter(uc) || uc >= 0x110000 ||
        (uc < min_uc) || (uc >= 0xd800 && uc <= 0xdfff))
        return false;

    *result = uc;
    return true;
}

bool QJsonParser::parseString(bool *latin1)
{
    *latin1 = true;

    const char *start = json;
    int outStart = current;

    // try to write out a latin1 string

    int stringPos = reserveSpace(2);
    BEGIN << "parse string stringPos=" << stringPos << json;
    while (json < end) {
        uint ch = 0;
        if (*json == '"')
            break;
        else if (*json == '\\') {
            if (!scanEscapeSequence(json, end, &ch))
                return false;
        } else {
            if (!scanUtf8Char(json, end, &ch))
                return false;
        }
        if (ch > 0xff) {
            *latin1 = false;
            break;
        }
        int pos = reserveSpace(1);
        DEBUG << "  " << ch << (char)ch;
        data[pos] = (uchar)ch;
    }
    ++json;
    DEBUG << "end of string";
    if (json >= end)
        return false;

    // no unicode string, we are done
    if (*latin1) {
        // write string length
        *(ushort *)(data + stringPos) = current - outStart - sizeof(ushort);
        reserveSpace((4 - current) & 3);
        END;
        return true;
    }

    *latin1 = false;
    DEBUG << "not latin";

    json = start;
    current = outStart + sizeof(int);

    while (json < end) {
        uint ch = 0;
        if (*json == '"')
            break;
        else if (*json == '\\') {
            if (!scanEscapeSequence(json, end, &ch))
                return false;
        } else {
            if (!scanUtf8Char(json, end, &ch))
                return false;
        }
        if (ch > 0xffff) {
            int pos = reserveSpace(4);
            *(ushort *)(data + pos) = QChar::highSurrogate(ch);
            *(ushort *)(data + pos + 2) = QChar::lowSurrogate(ch);
        } else {
            int pos = reserveSpace(2);
            *(ushort *)(data + pos) = (ushort)ch;
        }
    }
    ++json;

    if (json >= end)
        return false;

    // write string length
    *(int *)(data + stringPos) = (current - outStart - sizeof(int))/2;
    reserveSpace((4 - current) & 3);
    END;
    return true;
}

