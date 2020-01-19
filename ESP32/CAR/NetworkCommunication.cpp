#include "NetworkCommunication.h"
#include "Logger.h"

#include <WiFi.h>
#include "esp_wifi.h"

NetworkCommunication::NetworkCommunication(word* SYNC_PORT, WiFiServer* server):
  SYNC_PORT(SYNC_PORT), server(server), LastSync(0), RemoteIp("any")
{
  server->begin();
}

void NetworkCommunication::handle(){
    client1 = server->available();
    if(client1){
      String curr = "";
      ClientIp = client1.remoteIP();
      String ActualIp = IpToString(ClientIp);
      if(RemoteIp == "any"){
        RemoteIp = ActualIp;
      }else if(RemoteIp == ActualIp){
        Logger::logwinfo("Client connected from IP: " + ActualIp);
        Logger::logwinfo("FetchingMessage");

        while(client1.available()){
          char c = client1.read();
          int cn = c;
          Logger::logwinfo("[READ]: Character: '" + String(c) + "'  ...  Character number: " + String(cn));
          curr += c;
        }
        Logger::logwinfo( "[Final string]:" + (String)"'" + curr + "'");
        if(curr.equals("what ya doin")){
          client1.stop();
          if(client1.connect(ClientIp, *SYNC_PORT)){
            client1.println(LastMessage);
            client1.stop();
            LastSync = millis();
          }
        }else{
          LastMessage = curr;
        }
      }else{
        Logger::logwwarning("Client tryed to connect from foreign IP (If IP change reset): " + ActualIp);
        client1.stop();
        if(client1.connect(ClientIp, *SYNC_PORT)){
          client1.println("Refused.");
          client1.stop();
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
