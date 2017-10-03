#include "bakery.h"
#include "utils/hexdump.h"
#include "messages/bakerymessage.h"
#include "utils/types.h"
#include "packet.h"

#include <QFile>
#include <QSslKey>
#include <QDebug>
#include <QDataStream>

Bakery::Bakery(QObject* parent) : QObject(parent), fastForward(false)
{

}

void Bakery::start()
{
    socket = new QSslSocket(this);

    connect(socket, SIGNAL(stateChanged(QAbstractSocket::SocketState)),
            this, SLOT(socketStateChanged(QAbstractSocket::SocketState)));

    connect(socket, SIGNAL(encrypted()),
            this, SLOT(socketEncrypted()));

    connect(socket, SIGNAL(error(QAbstractSocket::SocketError)),
            this, SLOT(socketError(QAbstractSocket::SocketError)));

    connect(socket, SIGNAL(sslErrors(QList<QSslError>)),
            this, SLOT(sslErrors(QList<QSslError>)));

    connect(socket, SIGNAL(readyRead()),
            this, SLOT(socketReadyRead()));

    QFile privateKeyFile(":key/key.pem");

    if (!privateKeyFile.open(QFile::ReadOnly)) {
        logger->log(FATAL, "Unable to load key");
        return;
    }

    const QByteArray privateKey = privateKeyFile.readAll();
    privateKeyFile.close();

    QFile passPhraseFile(":key/passphrase.txt");

    if (!passPhraseFile.open(QFile::ReadOnly)) {
        logger->log(FATAL, "Unable to load passphrase");
        return;
    }

    const QByteArray passPhrase = passPhraseFile.readAll();
    passPhraseFile.close();

    QSslKey key(privateKey, QSsl::Rsa, QSsl::Pem, QSsl::PrivateKey, passPhrase);;

    if (key.isNull()) {
        logger->log(FATAL, "Key is null");
        return;
    }

    socket->setPrivateKey(key);
    socket->setLocalCertificate(":key/certificate.pem", QSsl::Pem);

    if (!socket->addCaCertificates(":key/ca.pem", QSsl::Pem, QRegExp::FixedString)) {
        logger->log(FATAL, "Unable to add CA certificate");
        return;
    }

    socket->setPeerVerifyMode(QSslSocket::VerifyNone);
    socket->ignoreSslErrors();
}

void Bakery::stop()
{
    socket->close();
}

void Bakery::send(IMessage* message)
{
    Packet packet;
    ByteArray buffer;

    packet.serialize(message, buffer);

    socket->write(buffer.data(), buffer.size());

    delete message;
}

void Bakery::send(QByteArray data)
{
     socket->write(data.data(), data.size());
}

void Bakery::socketStateChanged(QAbstractSocket::SocketState /*state*/)
{
    logger->log(DEBUG, "socketStateChanged");
    //qDebug() << state;
}

void Bakery::socketEncrypted()
{
    logger->log(DEBUG, "socketEncrypted");
}

void Bakery::socketError(QAbstractSocket::SocketError /*error*/)
{
    logger->log(DEBUG, "socketError");
    //qDebug() << error << socket->errorString();
}

void Bakery::socketReadyRead()
{
    QByteArray data = socket->readAll();

    /*QTime time = QTime::currentTime();
    QString timeFormated = time.toString("hh-mm-ss-zzz");
    QFile file(QString("dumps/dump-%1.bin").arg(timeFormated));
    file.open(QIODevice::WriteOnly);
    file.write(data);
    file.close();*/

    ByteArray readedByes(data.data(), data.data() + data.length());
    buffer.insert(buffer.end(), readedByes.begin(), readedByes.end());
    Packet packet;

    while (packet.deserialize(buffer)) {
        IMessage* message = packet.getMessage();
        logger->log(INFO, message->getName());
        lastMessageName = message->getName();
        onMessage(message);
    }
}

void Bakery::sslErrors(QList<QSslError> /*errors*/)
{
    logger->log(DEBUG, "sslErrors");
    //qDebug() << errors;
}
