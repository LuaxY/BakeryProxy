#ifndef BAKERYSERVER_H
#define BAKERYSERVER_H

#include "bakery.h"

#include <QtNetwork/QTcpServer>

class BakeryClient;

class BakeryServer : public Bakery, public QTcpServer
{
private:
    BakeryClient* client;

public:
    explicit BakeryServer(QObject *parent = nullptr);

    void start();
    void stop();
    void setClient(BakeryClient* _client);

protected:
    void incomingConnection(int descriptor);
    void onMessage(IMessage* message);
    void forward(QByteArray data);

private slots:
    void socketStateChanged(QAbstractSocket::SocketState state);
};

#endif // BAKERYSERVER_H
