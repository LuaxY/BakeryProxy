QT -= gui
QT += network

CONFIG += c++11 console
CONFIG -= app_bundle

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += main.cpp \
    utils\hexdump.cpp \
    network\bakeryclient.cpp \
    network\bakeryserver.cpp \
    utils\logger.cpp \
    network\bakery.cpp \
    messages\imessage.cpp \
    messages\bakerymessage.cpp \
    io\binaryreader.cpp \
    io\binarywriter.cpp \
    io\types\binary64.cpp \
    io\types\int64.cpp \
    io\types\uint64.cpp \
    utils\booleanbytewrapper.cpp \
    utils\byteorder.cpp \
    network/packet.cpp

HEADERS += \
    utils\hexdump.h \
    network\bakeryclient.h \
    network\bakeryserver.h \
    utils\logger.h \
    network\bakery.h \
    messages\imessage.h \
    messages\bakerymessage.h \
    io\binaryreader.h \
    io\binarywriter.h \
    io\types\binary64.h \
    io\types\int64.h \
    io\types\uint64.h \
    utils\booleanbytewrapper.h \
    utils\byteorder.h \
    utils\types.h \
    network/packet.h

RESOURCES += \
    resources.qrc
