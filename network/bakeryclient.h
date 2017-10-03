#ifndef BAKERYCLIENT_H
#define BAKERYCLIENT_H

#include "bakery.h"

class BakeryServer;

class BakeryClient : public Bakery
{
private:
    BakeryServer* server;

public:
    explicit BakeryClient(QObject* parent = nullptr);

    void start();
    void stop();
    void setServer(BakeryServer* _server);

protected:
    void onMessage(IMessage* message);
    void forward(QByteArray data);

private slots:
    void socketStateChanged(QAbstractSocket::SocketState state);
};

#endif // BAKERYCLIENT_H
