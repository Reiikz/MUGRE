#include <Arduino.h>
#include <WiFi.h>
#include "esp_wifi.h"
#include "Logger.h"
#include "SerialAsker.h"


#pragma once

/*
 * 
 * This class serves as network communication basics, like listenning to a socket.
 * 
*/
class NetworkCommunication{
  
  public:
    
    NetworkCommunication(word* SYNC_PORT, WiFiServer* server);

    static String IpToString(IPAddress);
    void handle();
    void send(String);
    unsigned long TimeSinceSync();
    String LastMessage;

  private:
    
    word* SYNC_PORT;
    WiFiServer* server;
    unsigned long LastSync;
    String RemoteIp;
    
    WiFiClient client1;
    IPAddress ClientIp;
    //WiFiClient client2;
};
