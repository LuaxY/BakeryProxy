#ifndef PACKET_H
#define PACKET_H

#include "messages/imessage.h"
#include "io/binaryreader.h"
#include "utils/types.h"

class Packet
{
public:
    Packet();

    void serialize(IMessage* message, ByteArray& buffer);
    bool deserialize(ByteArray& buffer);

    IMessage* getMessage();

private:
    IMessage* message;

    ushort getMessageId(ushort header);
    ushort getMessageSizeType(ushort header);
    uint getMessageSize(ushort sizeType, BinaryReader& reader);
    ushort computeSizeType(uint size);
    ushort computeHeader(ushort id, ushort sizeType);
};

#endif // PACKET_H
