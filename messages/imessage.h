#ifndef IMESSAGE_H
#define IMESSAGE_H

#include <QByteArray>
#include <QString>
#include "utils/types.h"

class IMessage
{
public:
    IMessage(uint _unknownData, ushort _id, ByteArray _data);

    virtual QString getName() = 0;

    uint getUnknownData() const;
    ushort getId() const;
    ByteArray getData() const;
    void setData(ByteArray _data);

protected:
    uint unknownData;
    ushort id;
    ByteArray data;
};

#endif // IMESSAGE_H
