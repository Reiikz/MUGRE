#include <Arduino.h>
#include <WiFi.h>
#include "Logger.h"
#include "SerialAsker.h"
#include "esp_wifi.h"

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
    
    NetworkCommunication(word* LISTENPORT, word* SYNCPORT, String RemoteIp);

    String IpToString(IPAddress);
    void Handle();
    String GetLastRecvMessage();
    unsigned long GetSecondSinceLastRecv();
    bool GetWiFiStatus();
    void send(String);
    unsigned long TimeSinceSync();
    String LastMessage;

  private:
    String RemoteIp;
    String LogValue;
    word* SYNC_PORT;
    bool WifiStatus;
    unsigned long LastMessageTime;
    unsigned long LastConnectionAttempt;
    unsigned long LastSync;
    byte ConnectionAttemptCount;
    WiFiServer* server;
    WiFiClient client1;
    IPAddress ClientIp;
    //WiFiClient client2;

  private:
    void LogError();
    void LogInfo();
    void LogWarning();
};
