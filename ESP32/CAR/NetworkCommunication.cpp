#include "NetworkCommunication.h"
#include "Logger.h"

void NetworkCommunication::LogInfo(){
  Logger::Log(&LogValue, Logger::LOGTYPE_WIFI, Logger::LOGLEVEL_INFO);
}


void NetworkCommunication::LogError(){
  Logger::Log(&LogValue, Logger::LOGTYPE_WIFI, Logger::LOGLEVEL_CRITICAL);
}

void NetworkCommunication::LogWarning(){
  Logger::Log(&LogValue, Logger::LOGTYPE_WIFI, Logger::LOGLEVEL_WARNING);
}

void NetworkCommunication::Handle(){
  if(WifiStatus){
    client1 = server->available();
    if(client1){
      LastMessage = "";
      IPAddress ClientIp;
      ClientIp = client1.remoteIP();
      String ActualIp = IpToString(ClientIp);
      if(RemoteIp == "any"){
        RemoteIp = ActualIp;
      }else if(RemoteIp == ActualIp){
        LogValue = "Client connected from IP: " + ActualIp;
        LogInfo();

        LogValue = "FetchingMessage";
        LogInfo();

        while(client1.available()){
          char c = client1.read();
          int cn = c;
          LogValue = "";
          LogValue += "[READ]: Character: '" + String(c) + "'  ...  Character number: " + String(cn);
          LogInfo();
          LastMessage += c;
        }
        LogValue = "[Final string]:" + (String)"'" + LastMessage + "'";
        LogInfo();
      }else{
        LogValue = "Client tryed to connect from foreign IP (If IP change reset): " + ActualIp;
        LogWarning();
        client1.stop();
        if(client1.connect(ClientIp, *SEND_PORT)){
          client1.println("Refused.");
          client1.stop();
        }
      }
      
    }
  }
}


String NetworkCommunication::IpToString(IPAddress ip){
  return String(ip[0]) + "." + String(ip[1]) + "." + String(ip[2]) + "." + String(ip[3]);
}
