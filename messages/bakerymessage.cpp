#include "bakerymessage.h"

#include <QDataStream>

BakeryMessage::BakeryMessage(uint _unknownData, ushort _id, ByteArray _data) : IMessage(_unknownData, _id, _data)
{

}

QString BakeryMessage::getName()
{
    switch (id) {
    case 0x3E81:
        return "BakeryRawDataMessage";
        break;
    case 0x3E82:
        return "BakeryHashKeyMessage";
        break;
    case 0x3E83:
        return "BakeryHelloConnectMessage";
        break;
    case 0x3E84:
        return "BakeryIdentificationMessage";
        break;
    case 0x3E85:
        return "BakeryIdentificationSuccessMessage";
        break;
    case 0x3E86:
        return "BakeryIdentificationFailedMessage";
        break;
    case 0x3E87:
        return "BakeryAddAccountMessage";
        break;
    case 0x3E88:
        return "BakeryRemoveAccountMessage";
        break;
    case 0x3E8B:
        return "BakeryStatisticsMessage";
        break;
    case 0x3E8C:
        return "BakeryRawDataMessageError";
        break;
    case 0x3E8D:
        return "BakeryDisconnectionMessage";
        break;
    case 0x3E8E:
        return "BakeryNotifyBotBannedMessage";
        break;
    case 0x3E8F:
        return "BakeryBotBannedMessage";
        break;
    case 183:
        return "BasicPongMessage";
        break;
    case 6372:
        return "CheckIntegrityMessage";
        break;
    }

    return QString("BakeryUnknownMessage (%1)").arg(id);
}
