#include "imessage.h"


IMessage::IMessage(uint _unknownData, ushort _id, ByteArray _data) : unknownData(_unknownData), id(_id), data(_data)
{

}

uint IMessage::getUnknownData() const
{
    return unknownData;
}

ushort IMessage::getId() const
{
    return id;
}

ByteArray IMessage::getData() const
{
    return data;
}
