#ifndef LOGGER_H
#define LOGGER_H

#include <QString>
#include <windows.h>

enum LogType {
    DEBUG,
    INFO,
    SUCCESS,
    FAIL,
    LOG_ERROR,
    WARNING,
    FATAL
};

class Logger
{
private:
    QString name;
    unsigned short color;
    unsigned short defaultConsoleColor;
    HANDLE hConsole;

    setColor(unsigned short color);

public:
    Logger(QString _name, unsigned short _color);

    void log(LogType type, QString message);
};

#endif // LOGGER_H
