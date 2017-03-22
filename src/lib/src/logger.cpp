#include <iostream>
#include <string>
#include <stdio.h>
#include <stdarg.h>
#include "logger.h"

int Logger::mode = LOG_VERBOSE;
std::deque<LoggerListener *> Logger::listeners;

void Logger::write(LogType mode, const char *msg) {
    for (unsigned int index = 0; index < listeners.size(); index++) {
        listeners.at(index)->onLogMessage(msg, mode);
    }
}

void Logger::debug(const char *format, ...) {
    char dest[LOGGER_BUFFER_SIZE];
    va_list argptr;
    va_start(argptr, format);
    vsprintf(dest, format, argptr);
    va_end(argptr);

    Logger::write(LOG_DEBUG, dest);
}

void Logger::info(const char *format, ...) {
    char dest[LOGGER_BUFFER_SIZE];
    va_list argptr;
    va_start(argptr, format);
    vsprintf(dest, format, argptr);
    va_end(argptr);

    Logger::write(LOG_INFO, dest);
}

void Logger::error(const char *format, ...) {
    char dest[LOGGER_BUFFER_SIZE];
    va_list argptr;
    va_start(argptr, format);
    vsprintf(dest, format, argptr);
    va_end(argptr);

    Logger::write(LOG_ERROR, dest);
}

void Logger::registerListener(LoggerListener *listener) {
    for (unsigned int index = 0; index < listeners.size(); index++) {
        if (listeners[index] == listener) {
            return;
        }
    }

    listeners.push_back(listener);
}