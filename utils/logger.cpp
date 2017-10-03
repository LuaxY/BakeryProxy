#include "logger.h"
#include <QTime>
#include <iostream>

Logger::Logger(QString _name, unsigned short _color) : name(_name), color(_color)
{
    hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO defaultConsoleInfo;
    GetConsoleScreenBufferInfo(hConsole, &defaultConsoleInfo);
    defaultConsoleColor = defaultConsoleInfo.wAttributes;
}

Logger::setColor(unsigned short color)
{
    SetConsoleTextAttribute(hConsole, color);
}

void Logger::log(LogType type, QString message)
{
    QString typeString;
    unsigned short typeColor = defaultConsoleColor;

    switch (type) {
    case DEBUG:
        typeString = "DEBUG";
        typeColor = 0xB;
        break;
    case INFO:
        typeString = "INFO";
        typeColor = 0x9;
        break;
    case SUCCESS:
        typeString = "SUCCESS";
        typeColor = 0xA;
        break;
    case FAIL:
        typeString = "FAIL";
        typeColor = 0x8;
        break;
    case LOG_ERROR:
        typeString = "ERROR";
        typeColor = 0xC;
        break;
    case WARNING:
        typeString = "WARNING";
        typeColor = 0xE;
        break;
    case FATAL:
        typeString = "FATAL";
        typeColor = 0x4;
        break;
    default:
        break;
    }

    if (type == DEBUG) {
       return;
    }

    QTime time = QTime::currentTime();
    QString timeFormated = time.toString("hh:mm:ss.zzz");

    setColor(defaultConsoleColor);
    std::cout << QString("[%1][").arg(timeFormated).toStdString();
    setColor(typeColor);
    std::cout << typeString.toStdString();
    setColor(defaultConsoleColor);
    std::cout << "][";
    setColor(color);
    std::cout << name.toStdString();
    setColor(defaultConsoleColor);
    std::cout << QString("] %1").arg(message).toStdString() << std::endl;
    setColor(defaultConsoleColor);

}
