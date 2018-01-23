#include "bakeryclient.h"
#include <QFile>
#include <QSslKey>
#include "utils/hexdump.h"
#include "bakeryserver.h"
#include "io/binarywriter.h"
#include "messages/bakerymessage.h"

BakeryClient::BakeryClient(QObject* parent) : Bakery(parent)
{
    logger = new Logger("SRV", 0xA);
}

void BakeryClient::start()
{
    Bakery::start();

    socket->connectToHostEncrypted("bakery.mufibot.net", 6555);

    logger->log(INFO, "Client started !");
}

void BakeryClient::stop()
{
    Bakery::stop();

    logger->log(INFO, "Client stoped !");
}

void BakeryClient::setServer(BakeryServer* _server)
{
    server = _server;
}

void BakeryClient::onMessage(IMessage* message)
{
    QString messageName = message->getName();

    if (messageName == "BakeryIdentificationSuccessMessage") {
        ByteArray hack;
        BinaryWriter writer(hack);

        // date and more ?
        std::vector<ubyte> date = {0x42,0x76,0xED,0x4C,0xD9,0xF1,0x80,0x00,0x00,0x00,0x00,0x77,0x00};

        writer.writeUTF("MufiCrack");
        writer.writeByte(0x02); // Role ? user 0x01 | admin 0x02
        writer.writeUBytes(date, false);

        message->setData(hack);
    }

    if (
            messageName == "CheckIntegrityMessage" ||
            messageName == "BakeryHashKeyMessage" ||
            messageName == "ByteCodeTesterMessage" ||
            messageName == "BakeryNotifyBotBannedMessage" ||
            messageName == "BakeryBotBannedMessage"
        ) {
        ByteArray payload = message->getData();
        hexdump(payload.data(), payload.size());
    }

    /*if (messageName == "BakeryRawDataMessageError") {
        ByteArray payload = message->getData();
        hexdump(payload.data(), payload.size());

        ByteArray hack;
        BinaryWriter writer(hack);
        writer.writeUTF("amUgc3VpcyB1biBib3QgPDM=");

        message = new BakeryMessage(message->getUnknownData(), 6372, hack);
    }*/

    server->send(message);
}

void BakeryClient::forward(QByteArray data)
{
    server->send(data);
}

void BakeryClient::socketStateChanged(QAbstractSocket::SocketState state)
{
    Bakery::socketStateChanged(state);

    if (state == QAbstractSocket::ClosingState) {
        server->stop();
    }
}
