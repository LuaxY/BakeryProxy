#include "packet.h"
#include "messages/bakerymessage.h"
#include "io/binarywriter.h"
#include <QDebug>

Packet::Packet()
{

}

void Packet::serialize(IMessage* message, ByteArray& buffer)
{
    BinaryWriter writer(buffer);

    writer.writeUInt(message->getUnknownData());

    uint size = message->getData().size();
    ushort sizeType = computeSizeType(size);
    ushort header = computeHeader(message->getId(), sizeType);

    writer.writeUShort(header);

    switch (sizeType) {
    case 1:
        writer.writeByte(size);
        break;
    case 2:
        writer.writeShort(size);
        break;
    case 3:
        uint high = (size >> 16) & 255;
        uint low = size & 65535;
        writer.writeByte(high);
        writer.writeShort(low);
        break;
    }

    writer.writeBytes(message->getData(), false);
}

bool Packet::deserialize(ByteArray& buffer)
{
    BinaryReader reader(buffer);
    uint unknownData = 0; // account ou character id ?
    ushort header = 0;
    uint countReadedBytes = 0;

    if (reader.bytesAvailable() < sizeof(unknownData)) {
        return false;
    }

    unknownData = reader.readUInt();
    countReadedBytes += sizeof(unknownData);

    if (reader.bytesAvailable() < sizeof(header)) {
        return false;
    }

    header = reader.readUShort();
    countReadedBytes += sizeof(header);

    ushort id = getMessageId(header);
    ushort sizeType = getMessageSizeType(header);

    if (reader.bytesAvailable() < sizeType) {
        return false;
    }

    uint size = getMessageSize(sizeType, reader);
    countReadedBytes += sizeType;

    if (reader.bytesAvailable() < size) {
        return false;
    }

    ByteArray data = reader.readBytes(size);
    countReadedBytes += size;

    buffer.erase(buffer.begin(), buffer.begin() + countReadedBytes);

    message = new BakeryMessage(unknownData, id, data);

    return true;
}

IMessage *Packet::getMessage()
{
    return message;
}

ushort Packet::getMessageId(ushort header)
{
    return header >> 2;
}

ushort Packet::getMessageSizeType(ushort header)
{
    return static_cast<ushort>(header & 3);
}

uint Packet::getMessageSize(ushort sizeType, BinaryReader &reader)
{
    uint size = 0;

    switch (sizeType) {
    case 1:
        size = static_cast<uint>(reader.readByte());
        break;
    case 2:
        size = reader.readUShort();
        break;
    case 3:
        size = static_cast<uint>(
                    ((reader.readByte() & 255) << 16) +
                    ((reader.readByte() & 255) << 8) +
                     (reader.readByte() & 255));
        break;
    default:
        break;
    }

    return size;
}

ushort Packet::computeSizeType(uint size)
{
    if (size > 65535) return 3;
    if (size > 255)   return 2;
    if (size > 0)     return 1;
    return 0;
}

ushort Packet::computeHeader(ushort id, ushort sizeType)
{
    return id << 2 | sizeType;
}

