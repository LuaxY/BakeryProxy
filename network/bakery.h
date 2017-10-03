#ifndef BAKERY_H
#define BAKERY_H

#include "utils/logger.h"
#include "messages/imessage.h"

#include <QObject>
#include <QSslSocket>
#include <QAbstractSocket>

class Bakery : public QObject
{
    Q_OBJECT

private:
    QString lastMessageName;
    ByteArray buffer;

    bool fastForward;

protected:
    QSslSocket* socket;
    Logger* logger;

public:
    Bakery(QObject* parent = nullptr);

    virtual void start();
    virtual void stop();
    virtual void send(IMessage* message);
    virtual void send(QByteArray data);

protected:
    virtual void onMessage(IMessage* message) = 0;
    virtual void forward(QByteArray data) = 0;

protected slots:
    virtual void socketStateChanged(QAbstractSocket::SocketState state);
    virtual void socketEncrypted();
    virtual void socketError(QAbstractSocket::SocketError error);
    virtual void socketReadyRead();
    virtual void sslErrors(QList<QSslError> errors);
};

#endif // BAKERY_H
