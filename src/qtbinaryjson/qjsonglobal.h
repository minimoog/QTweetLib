#ifndef QJSONGLOBAL_H
#define QJSONGLOBAL_H

#include <qglobal.h>
#include <qstring.h>

namespace QtJson
{
// ### FIXME
//    namespace Private {
        struct Data;
        struct Base;
        struct Object;
        struct Header;
        struct Array;
        struct Value;
//    };

    class JsonValue;
    class JsonObject;
    class JsonArray;
    class JsonDocument;
    class QJsonParser;

    enum ValueType {
        NullValue =  0x0,
        BooleanValue = 0x1,
        NumberValue = 0x2,
        StringValue = 0x3,
        ArrayValue = 0x4,
        ObjectValue = 0x5,
        UndefinedValue = 0x80
    };

}

#endif // QJSONGLOBAL_H
