#ifndef LOGGER_H
#define LOGGER_H

#include <string>
#include <deque>

#define LOGGER_BUFFER_SIZE (1024 * 10)

enum LogType {
    LOG_ERROR = 0x01,
    LOG_INFO = 0x02,
    LOG_DEBUG = 0x04,
    LOG_VERBOSE = 0xFF
};

class LoggerListener {
public:
    virtual void onLogMessage(const char *message, LogType mode)=0;
};

class Logger
{
private:

    static int mode;

    static std::deque<LoggerListener *> listeners;

public:

    static void write(LogType mode, std::string msg);

    static void write(LogType mode, const char *msg);

    static void debug(const char *format, ...);

    static void info(const char *format, ...);

    static void error(const char *format, ...);

    static void registerListener(LoggerListener *listener);
};

#endif