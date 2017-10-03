#include <QCoreApplication>
#include "network/bakeryclient.h"
#include "network/bakeryserver.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    if (!QSslSocket::supportsSsl()) {
        qDebug() << "This system does not support OpenSSL.";
        return -1;
    }

    BakeryClient bakeryClient;
    BakeryServer bakeryServer;

    bakeryClient.setServer(&bakeryServer);
    bakeryServer.setClient(&bakeryClient);

    //bakeryClient.start();
    bakeryServer.start();

    return a.exec();
}
