#include "Logger.h"
#include <Arduino.h>

void Logger::loggerror(String m){
  Logger::Log(m, Logger::LOGTYPE_GENERAL, Logger::LOGLEVEL_INFO);
}

void Logger::loggwarning(String m){
  Logger::Log(m, Logger::LOGTYPE_GENERAL, Logger::LOGLEVEL_WARNING);
}

void Logger::logginfo(String m){
  Logger::Log(m, Logger::LOGTYPE_GENERAL, Logger::LOGLEVEL_INFO);}

void Logger::logwerror(String m){
  Logger::Log(m, Logger::LOGTYPE_WIFI, Logger::LOGLEVEL_ERROR);
}

void Logger::logwwarning(String m){
  Logger::Log(m, Logger::LOGTYPE_WIFI, Logger::LOGLEVEL_WARNING);
}

void Logger::logwinfo(String m){
  Logger::Log(m, Logger::LOGTYPE_WIFI, Logger::LOGLEVEL_INFO);
}

void Logger::logserror(String m){
  Logger::Log(m, Logger::LOGTYPE_SENSOR, Logger::LOGLEVEL_ERROR);
}

void Logger::logswarning(String m){
  Logger::Log(m, Logger::LOGTYPE_SENSOR, Logger::LOGLEVEL_WARNING);
}

void Logger::logsinfo(String m){
  Logger::Log(m, Logger::LOGTYPE_SENSOR, Logger::LOGLEVEL_INFO);
}

void Logger::Log(String logInfo, const char logType, const char logLevel){
  if(ENABLE_LOGGER){
    String type;
  switch(logType){
    case LOGTYPE_SENSOR:{
      type = "[SENSOR]:";
      break;
    }
    case LOGTYPE_WIFI:{
      type = "[WIFI]:";
      break;
    }
    case LOGTYPE_GENERAL:{
      type = "[GENERAL]:";
      break;
    }
    default:{
      type="[UNDEFINED]:";
      break;
    }
  }

  switch(logLevel){
    case LOGLEVEL_INFO:{
      type += " [INFO]: ";
      break;
    }
    case LOGLEVEL_WARNING:{
      type += " [WARNING]: ";
      break;
    }
    case LOGLEVEL_ERROR:{
      type += " [ERROR]: ";
      break;
    }
    default:{
      type +=" [WRONGLEVEL]: ";
      break;
    }
  }
  type = String("[MUGRE]: ") + type;
  type += logInfo;
  Serial.println(type);
  }  
}
