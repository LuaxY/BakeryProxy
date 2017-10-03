#include "bakeryclient.h"
#include <QFile>
#include <QSslKey>
#include "utils/hexdump.h"
#include "bakeryserver.h"

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
