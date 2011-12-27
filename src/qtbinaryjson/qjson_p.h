#ifndef QJSON_P_H
#define QJSON_P_H

#include <qjsonglobal.h>
#include <qjsonobject.h>
#include <qjsonvalue.h>
#include <qjsonarray.h>
#include <qbasicatomic.h>
#include <qstring.h>

/*
  This defines a binary data structure for Json data. The data structure is optimised for fast reading and minimum allocations.
  The whole data structure can be mmap'ed and used directly.

  In most cases the binary structure is not as space efficient as a utf8 encoded text representation, but much faster to access.

  The size requirements are:

  String: 16 bytes header + 2*(string.length())

  Values: 4 bytes + size of data (size can be 0 for some data)
    bool: 0 bytes
    number: 8 bytes
    string: 4 + 2*length of string
    array: size of array
    object: size of object
  Array: 16 bytes + 4*length + size of Value data
  Object: 16 bytes + 12*length + size of Key Strings + size of Value data

  For an example such as

    {                                           // object: 12 + 5*12                = 72
         "firstName": "John",                   // key 4+12, value 8          = 24
         "lastName" : "Smith",                  // key 4+8, value 8         = 20
         "age"      : 25,                       // key 4+4, value 0             = 8
         "address"  :                           // key 4+8, object below = 160
         {                                      // object: 12 + 4*12
             "streetAddress": "21 2nd Street",  // key 4+16, value 16
             "city"         : "New York",       // key 4+4, value 12
             "state"        : "NY",             // key 4+8, value 4
             "postalCode"   : "10021"           // key 4+12, value 8
         },                                     // object total: 148
         "phoneNumber":                         // key: 4+12, value array below = 208
         [                                      // array: 12 + 2*4 + values below = 192
             {                                  // object 12 + 2*12
               "type"  : "home",                // key 4+4, value 8
               "number": "212 555-1234"         // key 4+8, value 16
             },                                 // object total: 84
             {                                  // object 12 + 2*12
               "type"  : "fax",                 // key 4+4, value 8
               "number": "646 555-4567"         // key 4+8, value 16
             }                                  // object total: 84
         ]                                      // array total: 248
    }                                           // great total:                       472 bytes

    The uncompressed text file used roughly 500 bytes, so we end up using about the same space
    as the text representation.
*/
namespace QtJson
{

typedef uint offset;
struct Array;
struct Object;
struct Value;
struct Entry;

const uint QBJS_Tag = ('q' << 24) | ('b' << 16) | ('j' << 8) | 's';

static inline int alignedSize(int size) { return (size + 3) & ~3; }

static inline bool useCompressed(const QString &s)
{
    if (s.length() >= 0x8000)
        return false;
    const ushort *uc = (const ushort *)s.constData();
    const ushort *e = uc + s.length();
    while (uc < e) {
        if (*uc > 0xff)
            return false;
        ++uc;
    }
    return true;
}

static inline int qStringSize(const QString &string, bool compress)
{
    int l = 2 + string.length();
    if (!compress)
        l *= 2;
    return alignedSize(l);
}

static inline void copyString(char *dest, const QString &str, bool compress)
{
    if (compress) {
        *((ushort *)dest) = str.length();
        uchar *d = (uchar *)dest + sizeof(ushort);
        const ushort *uc = (const ushort *)str.unicode();
        for (int i = 0; i < str.length(); ++i)
            *d++ = uc[i];
    } else {
        *((int *)dest) = str.length();
        memcpy(dest + sizeof(int), str.constData(), str.length()*sizeof(ushort));
    }
}


// returns INT_MAX if it can't compress it into 28 bits
static inline int compressedNumber(double d)
{
    // this relies on details of how ieee floats are represented
    const int exponent_off = 52;
    const quint64 fraction_mask = 0x000fffffffffffff;
    const quint64 exponent_mask = 0x7ff0000000000000;

    union {
        quint64 val;
        double dbl;
    };
    dbl = d;
    int exp = (int)((val & exponent_mask) >> exponent_off) - 1023;
    if (exp < 0 || exp > 25)
        return INT_MAX;

    quint64 non_int = val & (fraction_mask >> exp);
    if (non_int)
        return INT_MAX;

    bool neg = (val >> 63);
    val &= fraction_mask;
    val |= ((quint64)1 << 52);
    int res = (int)(val >> (52 - exp));
    return neg ? -res : res;
}

struct Latin1String;

struct String
{
    String(const ushort *utf16, int length)
        : length(length), utf16(utf16) {}
    int length;
    const ushort *utf16;

    inline bool equals(const ushort *s, int slen) const
    {
        if (slen != length)
            return false;
        int l = length;
        const ushort *a = utf16;
        const ushort *b = s;
        while (l-- && *a == *b)
            a++,b++;
        return (l == -1);
    }

    inline bool equals(const uchar *s, int slen) const
    {
        if (slen != length)
            return false;
        int l = length;
        const ushort *a = utf16;
        const uchar *b = s;
        while (l-- && *a == *b)
            a++,b++;
        return (l == -1);
    }

    bool operator ==(const QString &str) {
        return equals((const ushort *)str.constData(), str.length());
    }
    bool operator !=(const QString &str) {
        return !equals((const ushort *)str.constData(), str.length());
    }
    bool operator ==(const String &str) {
        return equals(str.utf16, str.length);
    }
    bool operator !=(const String &str) {
        return !equals(str.utf16, str.length);
    }
    inline bool operator ==(const Latin1String &str);
    inline bool operator !=(const Latin1String &str);
};

struct Latin1String
{
    Latin1String(const uchar *latin1, int length)
        : length(length), latin1(latin1) {}
    short length;
    const uchar *latin1;

    inline bool equals(const ushort *s, int slen) const
    {
        if (slen != length)
            return false;
        int l = length;
        const uchar *a = latin1;
        const ushort *b = s;
        while (l-- && *a == *b)
            a++,b++;
        return (l == -1);
    }

    inline bool equals(const uchar *s, int slen) const
    {
        if (slen != length)
            return false;
        int l = length;
        const uchar *a = latin1;
        const uchar *b = s;
        while (l-- && *a == *b)
            a++,b++;
        return (l == -1);
    }

    bool operator ==(const QString &str) {
        return equals((const ushort *)str.constData(), str.length());
    }
    bool operator !=(const QString &str) {
        return !equals((const ushort *)str.constData(), str.length());
    }
    bool operator ==(const String &str) {
        return equals(str.utf16, str.length);
    }
    bool operator !=(const String &str) {
        return !equals(str.utf16, str.length);
    }
    bool operator ==(const Latin1String &str) {
        return equals(str.latin1, str.length);
    }
    bool operator !=(const Latin1String &str) {
        return !equals(str.latin1, str.length);
    }
};

inline bool String::operator ==(const Latin1String &str) {
    return equals(str.latin1, str.length);
}
inline bool String::operator !=(const Latin1String &str) {
    return !equals(str.latin1, str.length);
}

struct Base
{
    uint size;
    uint length : 31;
    uint is_object : 1; // object or Array
    offset tableOffset;
    // content follows here

    bool isObject() const { return is_object; }
    bool isArray() const { return !is_object; }

    inline offset *table() const { return (offset *) (((char *) this) + tableOffset); }

    int reserveSpace(uint dataSize, int posInTable, uint numItems)
    {
        Q_ASSERT(posInTable >= 0 && posInTable <= (int)length);

        offset off = tableOffset;
        // move table to new position
        memmove((char *)(table() + posInTable + numItems) + dataSize, table() + posInTable, (length - posInTable)*sizeof(offset));
        memmove((char *)(table()) + dataSize, table(), posInTable*sizeof(offset));
        tableOffset += dataSize;
        for (int i = 0; i < (int)numItems; ++i)
            table()[posInTable + i] = off;
        length += numItems;
        size += dataSize + numItems * sizeof(offset);
        return off;
    }

    void removeItems(int pos, int numItems)
    {
        Q_ASSERT(pos >= 0 && pos <= (int)length);
        if (pos + numItems < (int)length)
            memmove(table() + pos, table() + pos + numItems, (length - pos - numItems)*sizeof(offset));
        length -= numItems;
    }
};

struct Object : public Base
{

    Entry *entryAt(int i) const {
        return reinterpret_cast<Entry *>(((char *)this) + table()[i]);
    }
    inline int indexOf(const QString &key);
};


struct Array : public Base
{
    inline Value at(int i) const;
    inline Value &operator [](int i);
};


struct Value
{
    // unfortunately some compilers can't handle mixed types in bitfields.
    // this works around the problem
    union {
        struct {
            int unused : 5;
            int int_val : 27;
        };
        struct {
            uint type : 3;
            uint latinOrIntValue : 1;
            uint latinKey : 1;
            uint val : 27;
        };
    };


    inline char *data(const Base *b) const { return ((char *)b) + val; }

    bool toBoolean() const;
    double toNumber(const Base *b) const;
    int toInt() const;
    QString toString(const Base *b) const;
    String asString(const Base *b) const;
    Latin1String asLatin1String(const Base *b) const;
    Base *objectOrArray(const Base *b) const;

    inline void setNull() {
        val = 0;
        latinOrIntValue = 0;
        latinKey = false;
        type = NullValue;
    }

    inline void setBool(bool b) {
        val = b;
        latinOrIntValue = false;
        latinKey = false;
        type = BooleanValue;
    }

    inline void setInt(int i) {
        Q_ASSERT(i < 1e27 && i > -1e27);
        int_val = i;
        latinOrIntValue = true;
        latinKey = false;
        type = NumberValue;
    }

    inline void setString(Base *b, offset off, const QString &str) {
        val = off;
        latinOrIntValue = false;
        latinKey = false;
        type = StringValue;
        memcpy(((char *)b) + off, str.constData(), str.length()*sizeof(ushort));
    }

    inline void setObject(Base *b, offset off, const Object *o) {
        val = off;
        latinOrIntValue = false;
        latinKey = false;
        type = StringValue;
        memcpy(((char *)b) + off, o, o->size);
    }
};

inline Value Array::at(int i) const
{
    return * (Value *) (((char *) this) + tableOffset + i*sizeof(Value));
}

inline Value &Array::operator [](int i)
{
    return * (Value *) (((char *) this) + tableOffset + i*sizeof(Value));
}


struct Entry {
    uint size;
    Value value;
    ushort *keyData() const { return (ushort *)((const char *)this + sizeof(Entry) + sizeof(int)); }
    uchar *latin1KeyData() const { return (uchar *)((const char *)this + sizeof(Entry) + sizeof(ushort)); }
    // key
    // value data follows key

    String shallowKey() const
    {
        Q_ASSERT(!value.latinKey);
        int length = *(int *) ((const char *)this + sizeof(Entry));
        return String(keyData(), length);
    }
    Latin1String shallowLatin1Key() const
    {
        Q_ASSERT(value.latinKey);
        int length = *(ushort *) ((const char *)this + sizeof(Entry));
        return Latin1String(latin1KeyData(), length);
    }
    QString key() const
    {
        if (value.latinKey) {
            int length = *(ushort *) ((const char *)this + sizeof(Entry));
            return QString::fromLatin1((const char *)latin1KeyData(), length);
        }
        int length = *(int *) ((const char *)this + sizeof(Entry));
        return QString((const QChar *)keyData(), length);
    }
};

inline int Object::indexOf(const QString &key)
{
    for (int i = 0; i < (int)length; ++i) {
        Entry *e = entryAt(i);
        if (e->value.latinKey) {
            if (e->shallowLatin1Key() == key)
                return i;
        } else {
            if (e->shallowKey() == key)
                return i;
        }
    }
    return -1;
}


struct Header {
    uint tag; // 'qbjs'
    uint version; // 1
    Base *root() { return (Base *)(this + 1); }
};


inline bool Value::toBoolean() const
{
    Q_ASSERT(type == BooleanValue);
    return val != 0;
}

inline double Value::toNumber(const Base *b) const
{
    Q_ASSERT(type == NumberValue);
    if (latinOrIntValue)
        return int_val;

    union {
        char raw[sizeof(double)];
        double d;
    };
    memcpy(raw, ((char *)b + val), sizeof(double));
    return d;
}

inline int Value::toInt() const
{
    Q_ASSERT(type == NumberValue && latinOrIntValue);
    return int_val;
}

inline QString Value::toString(const Base *b) const
{
    char *d = data(b);
    if (latinOrIntValue) {
        int l = *(ushort *)d;
        return QString::fromLatin1(d + sizeof(ushort), l);
    }
    int l = *(int *)d;
    const QChar *c = (const QChar *)(d + sizeof(int));
    return QString(c, l);
}

inline String Value::asString(const Base *b) const
{
    Q_ASSERT(type == StringValue && !latinOrIntValue);
    char *d = data(b);
    int l = *(int *)d;
    const ushort *c = (const ushort *)(d + sizeof(int));
    return String(c, l);
}

inline Latin1String Value::asLatin1String(const Base *b) const
{
    Q_ASSERT(type == StringValue && latinOrIntValue);
    char *d = data(b);
    ushort l = *(ushort *)d;
    const uchar *c = (const uchar *)(d + sizeof(ushort));
    return Latin1String(c, l);
}

inline Base *Value::objectOrArray(const Base *b) const
{
    Q_ASSERT(type == ArrayValue || type == ObjectValue);
    return reinterpret_cast<Base *>(data(b));
}

struct Data {
    inline Data(char *raw, int a)
        : alloc(a), needsCompaction(false), rawData(raw)
    { ref.store(0); }
    inline Data(int reserved, ValueType valueType)
        : needsCompaction(false), rawData(0)
    {
        ref.store(0);

        alloc = sizeof(Header) + sizeof(Base) + reserved + sizeof(offset);
        header = (Header *)malloc(alloc);
        header->tag = QBJS_Tag;
        header->version = 1;
        Base *b = header->root();
        b->size = sizeof(Base);
        b->is_object = (valueType == ObjectValue);
        b->tableOffset = sizeof(Base);
        b->length = 0;
    }
    inline ~Data()
    { free(rawData); }

    QBasicAtomicInt ref;
    int alloc;
    bool needsCompaction;
    union {
        char *rawData;
        Header *header;
    };

    uint offsetOf(const void *ptr) const { return ((char *)ptr - rawData); }

    Object *object(int offset) const { return reinterpret_cast<Object *>(rawData + offset); }
    Entry *entry(int offset) const { return reinterpret_cast<Entry *>(rawData + offset); }
    Value *value(int offset) const { return reinterpret_cast<Value *>(rawData + offset); }
    Array *array(int offset) const { return reinterpret_cast<Array *>(rawData + offset); }


    JsonObject toObject(Object *o) const
    {
        return JsonObject(const_cast<Data *>(this), o);
    }

    JsonArray toArray(Array *a) const
    {
        return JsonArray(const_cast<Data *>(this), a);
    }

    Data *detach(Base *b, int reserve = 0) {
        int size = sizeof(Header) + b->size + reserve;
        char *raw = (char *)malloc(size);
        memcpy(raw + sizeof(Header), b, b->size);
        Header *h = (Header *)raw;
        h->tag = QBJS_Tag;
        h->version = 1;
        return new Data(raw, size);
    }
};

}

#endif // QJSON_P_H
