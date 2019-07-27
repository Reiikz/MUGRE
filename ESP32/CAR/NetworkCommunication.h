#include <Arduino.h>
#include <WiFi.h>
#include "Logger.h"
#include "SerialAsker.h"

#pragma once

/*
 * 
 * This class serves as network communication basics, like listenning to a socket, 
 * initialization of the network connection, it does low level stuff
 * 
*/
class NetworkCommunication{

  public:
  
    static const byte MAX_CONNECTION_ATTEMPTS = 50;
    static const word CONNECTION_ATTEMPT_ESTIMATED_INTERVAL = 700;
  
  public:
    
    NetworkCommunication(char* WIFI_SSID, int ssid_size, char* WIFI_PASSWD, int pass_size, word* LISTENPORT, word* SENDPORT, word* SYNCPORT){   

      if(strlen(WIFI_SSID) == 0){
        SerialAsker::AskForCString("Enter WIFI SSID", WIFI_SSID, ssid_size);
      }
      
      if(strlen(WIFI_PASSWD) == 0){
        SerialAsker::AskForCString("Enter WIFI Password:", WIFI_PASSWD, pass_size); 
      }
      
      LastMessage = "";
      LastMessageTime = 0;
      SEND_PORT=SENDPORT;
      SYNC_PORT=SYNCPORT;
      ConnectionAttemptCount = 0;
      RemoteIp = "any";
      
      WiFi.setHostname("MUGRES-BODY");
      LogValue = "[SSID]: '" + String(WIFI_SSID) + "'";
      LogInfo();
      LogValue = "[PASSWORD]: '" + String(WIFI_PASSWD)  + "'";
      LogInfo();
      WiFi.begin(WIFI_SSID, WIFI_PASSWD);
      
      while(WiFi.status() != WL_CONNECTED && ConnectionAttemptCount < MAX_CONNECTION_ATTEMPTS){
        if(millis() - LastConnectionAttempt > CONNECTION_ATTEMPT_ESTIMATED_INTERVAL){
          ConnectionAttemptCount++;
          LogValue = "[CONNECTION ATTEMPT]: " + String(ConnectionAttemptCount);
          LogInfo();
          LastConnectionAttempt = millis();
          delay(CONNECTION_ATTEMPT_ESTIMATED_INTERVAL);
        }
      }
      
      if(ConnectionAttemptCount < MAX_CONNECTION_ATTEMPTS){
        WifiStatus = 1;
        IPAddress ip = WiFi.localIP();
        LocalIp = IpToString(ip);
        LogValue = "IP ADDRESS: " + LocalIp;
        LogInfo();
        server = new WiFiServer(*LISTENPORT);
        server->begin();
        LogValue = "Listenning on Port: " + String(*LISTENPORT);
        LogInfo();
      }else{
        WifiStatus = 0;
        LogValue = "Unable to connect wifi, reset to retry.";
        LogError();
      }
    }

    String IpToString(IPAddress);
    void Handle();
    String GetLastRecvMessage();
    unsigned long GetSecondSinceLastRecv();
    bool GetWiFiStatus();

  private:
    String LocalIp;
    String RemoteIp;
    String LogValue;
    word* SEND_PORT;
    word* SYNC_PORT;
    bool WifiStatus;
    String LastMessage;
    unsigned long LastMessageTime;
    unsigned long LastConnectionAttempt;
    byte ConnectionAttemptCount;
    WiFiServer* server;
    WiFiClient client1;
    //WiFiClient client2;

  private:
    void LogError();
    void LogInfo();
    void LogWarning();
};
