#ifndef BAKERYMESSAGE_H
#define BAKERYMESSAGE_H

#include "imessage.h"

#include <QString>

class BakeryMessage : public IMessage
{
public:
    BakeryMessage(uint _unknownData, ushort _id, ByteArray _data);

    QString getName();
};

#endif // BAKERYMESSAGE_H
