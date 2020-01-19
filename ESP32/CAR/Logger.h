#include <Arduino.h>


#pragma once


class Logger{
  public:
    static const bool ENABLE_LOGGER = 1;
    
    static void loggerror(String);
    static void loggwarning(String);
    static void logginfo(String);
    
    static void logwerror(String);
    static void logwwarning(String);
    static void logwinfo(String);

    static void logserror(String);
    static void logswarning(String);
    static void logsinfo(String);
    
    static void Log(String logInfo, const char logtype, const char LogLevel);
    
    static const char LOGTYPE_WIFI = 1, LOGTYPE_SENSOR = 2, LOGTYPE_GENERAL = 3;      
    static const char LOGLEVEL_WARNING = 1, LOGLEVEL_INFO = 2, LOGLEVEL_ERROR = 3;
};
