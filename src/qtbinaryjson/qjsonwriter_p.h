#include <qjsonglobal.h>

namespace QtJson
{

class QJsonWriter
{
public:
    static void objectToJson(const Object *o, QByteArray &json, int indent);
    static void arrayToJson(const Array *a, QByteArray &json, int indent);
};

}
