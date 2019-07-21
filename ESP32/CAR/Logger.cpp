#include "Logger.h"
#include <Arduino.h>

void Logger::Log(String* logInfo, const char logType, const char logLevel){
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
    case LOGTYPE_WIFI:{
      type += " [WARNING]: ";
      break;
    }
    case LOGTYPE_GENERAL:{
      type += " [ERROR]: ";
      break;
    }
    default:{
      type +=" [WRONGLEVEL]: ";
      break;
    }
  }
  type += *logInfo;
  Serial.println(type);
  }  
}
