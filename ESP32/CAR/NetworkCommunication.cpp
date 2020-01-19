#include "NetworkCommunication.h"
#include "Logger.h"

NetworkCommunication::NetworkCommunication(word* LISTENPORT, word* SYNCPORT, String RemoteIp):
{
  
  
}

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
      String curr = "";
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
          curr += c;
        }
        LogValue = "[Final string]:" + (String)"'" + curr + "'";
        if(curr.equals("what ya doin")){
          client1.stop();
          if(client1.connect(ClientIp, *SYNC_PORT)){
            client1.println(LastMessage);
            client1.stop();
          }
        }else{
          LastMessage = curr;
        }
        LogInfo();
      }else{
        LogValue = "Client tryed to connect from foreign IP (If IP change reset): " + ActualIp;
        LogWarning();
        client1.stop();
        if(client1.connect(ClientIp, *SYNC_PORT)){
          client1.println("Refused.");
          client1.stop();
          LastSync = millis();
        }
      }
      
    }
  }
}

unsigned long NetworkCommunication::TimeSinceSync(){
  return millis() - LastSync;
}

void NetworkCommunication::send(String d){
  if(client1.connect(ClientIp, *SYNC_PORT)){
    client1.println(d);
    client1.stop();
  }
}

String NetworkCommunication::IpToString(IPAddress ip){
  return String(ip[0]) + "." + String(ip[1]) + "." + String(ip[2]) + "." + String(ip[3]);
}
