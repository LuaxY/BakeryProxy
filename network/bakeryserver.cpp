#include "bakeryserver.h"
#include <QFile>
#include <QSslKey>
#include "utils/hexdump.h"
#include "bakeryclient.h"

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

    if (!socket->setSocketDescriptor(descriptor)) {
        logger->log(LOG_ERROR, "Unable to set socket descriptor");
        return;
    }

    socket->startServerEncryption();
}

void BakeryServer::onMessage(IMessage* message)
{
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

