#include <Arduino.h>


#pragma once


class Logger{
  public:
    static const bool ENABLE_LOGGER = 1;
    
    static void Log(String* logInfo);
    static void Log(String* logInfo, const char logType);
    static void Log(String* logInfo, const char logtype, const char LogLevel);
    static const char LOGTYPE_WIFI = 1, LOGTYPE_SENSOR = 2, LOGTYPE_GENERAL = 3;      
    static const char LOGLEVEL_WARNING = 1, LOGLEVEL_INFO = 2, LOGLEVEL_CRITICAL = 3;
};
