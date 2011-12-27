#ifndef QJSONPARSER_P_H
#define QJSONPARSER_P_H

#include <qjsondocument.h>

namespace QtJson {

class QJsonParser
{
public:
    QJsonParser(const char *json, int length);

    QtJson::JsonDocument parse();

private:
    inline bool eatSpace();
    inline char nextToken();

    bool parseObject();
    bool parseArray();
    bool parseMember(int baseOffset);
    bool parseString(bool *latin1);
    bool parseValue(Value *val, int baseOffset);
    bool parseNumber(Value *val, int baseOffset);
    const char *json;
    const char *end;

    char *data;
    int dataLength;
    int current;

    inline int reserveSpace(int space) {
        if (current + space >= dataLength) {
            dataLength = 2*dataLength + space;
            data = (char *)realloc(data, dataLength);
        }
        int pos = current;
        current += space;
        return pos;
    }
};

}
#endif
