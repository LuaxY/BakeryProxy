#include "bakeryserver.h"
#include <QFile>
#include <QSslKey>
#include "utils/hexdump.h"
#include "bakeryclient.h"
#include "io/binarywriter.h"
#include "io/binaryreader.h"
#include "messages/bakerymessage.h"

BakeryServer::BakeryServer(QObject* parent) : Bakery(parent), QTcpServer(parent)
{
    logger = new Logger("CLI", 0xB);
}

void BakeryServer::start()
{
    Bakery::start();

    socket->setSocketOption(QAbstractSocket::KeepAliveOption, true);

    if (!listen(QHostAddress::Any, 6555)) {
        logger->log(FATAL, "Unable to listen");
        return;
    }

    logger->log(INFO, "Server started !");
}

void BakeryServer::stop()
{
    Bakery::stop();

    logger->log(INFO, "Server stoped !");
}

void BakeryServer::setClient(BakeryClient* _client)
{
    client = _client;
}

void BakeryServer::incomingConnection(int descriptor)
{

    logger->log(DEBUG, "incomingConnection");

    if (socket->state() != QAbstractSocket::UnconnectedState) {
        return;
    }

    Bakery::start();

    if (!socket->setSocketDescriptor(descriptor)) {
        logger->log(LOG_ERROR, "Unable to set socket descriptor");
        return;
    }

    socket->startServerEncryption();
}

void BakeryServer::onMessage(IMessage* message)
{
    QString messageName = message->getName();

    if (messageName == "BakeryRawDataMessage") {
        /*ByteArray payload = message->getData();
        //hexdump(payload.data(), 16);

        //QFile dump1("dumps/dump1.bin");
        //dump1.open(QFile::WriteOnly);
        //dump1.write(payload.data(), payload.size());
        //dump1.close();

        BinaryReader reader(payload);
        uint size = reader.readVarInt();
        reader.readBytes(size);
        reader.readUTF();
        std::string md5 = reader.readUTF();
        reader.readUShort();

        ByteArray hack;
        BinaryWriter writer(hack);

        // Fake ByteCode
        QFile byteCodeFile("bytecode/ByteCodeTester.swf");

        if (!byteCodeFile.open(QFile::ReadOnly)) {
            logger->log(FATAL, "Unable byte code");
            return;
        }

        const QByteArray byteCodeQBA = byteCodeFile.readAll();
        byteCodeFile.close();

        QCryptographicHash hash(QCryptographicHash::Md5);
        hash.addData(byteCodeQBA);

        std::vector<ubyte> byteCode(byteCodeQBA.begin(), byteCodeQBA.end());

        writer.writeVarInt(byteCode.size());
        writer.writeUBytes(byteCode, false);
        writer.writeUTF(hash.result().toHex().toStdString());
        writer.writeUTFBytes(md5.c_str(), 0x20);
        writer.writeUShort(0);

        message->setData(hack);

        //hexdump(hack.data(), 16);

        //QFile dump2("dumps/dump2.bin");
        //dump2.open(QFile::WriteOnly);
        //dump2.write(hack.data(), hack.size());
        //dump2.close();*/

        // Don't forward RDM, just send (bad) hack
        ByteArray hack;
        BinaryWriter writer(hack);
        writer.writeUTF("amUgc3VpcyB1biBib3QgPDM=");
        send(new BakeryMessage(message->getUnknownData(), 6372, hack));
        return;
    }

    if (
            messageName == "BakeryAddAccountMessage" ||
            messageName == "BakeryNotifyBotBannedMessage" ||
            messageName == "BakeryBotBannedMessage"
        ) {
        ByteArray payload = message->getData();
        hexdump(payload.data(), payload.size());
    }

    client->send(message);
}

void BakeryServer::forward(QByteArray data)
{
    client->send(data);
}

void BakeryServer::socketStateChanged(QAbstractSocket::SocketState state)
{
    Bakery::socketStateChanged(state);

    if (state == QAbstractSocket::ConnectedState) {
        client->start();
    }

    if (state == QAbstractSocket::ClosingState) {
        client->stop();
    }
}

