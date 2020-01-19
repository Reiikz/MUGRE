#include "NetworkCommunication.h"
#include "SerialAsker.h"
#include "Wheels.h"
#include "Steer.h"

char WIFI_SSID[50] = ":'v':";//{ '\0' };
char WIFI_PASSWD[50]  = "thedragonsarekawaii";//{ '\0' };

word LISTEN_PORT  = 5500;
word SYNC_PORT    = 5505;

#define MOTOR_ENABLE  (byte)  13
#define MOTOR_LEFT    (byte)  12
#define MOTOR_RIGHT   (byte)  14
#define STEER_SERVO   (byte)  4

NetworkCommunication* talker_;
Wheels* wheels;
Steer* steering;

void setup(){
  Serial.begin(115200);


  if(strlen(WIFI_SSID) == 0){
    SerialAsker::AskForCString("Enter WIFI SSID", WIFI_SSID, ssid_size);
  }
  
  if(strlen(WIFI_PASSWD) == 0){
    SerialAsker::AskForCString("Enter WIFI Password:", WIFI_PASSWD, pass_size); 
  }
  
  LastMessage = "";
  LastMessageTime = 0;
  LastSync = 0;
  SYNC_PORT=SYNCPORT;
  ConnectionAttemptCount = 0;
  RemoteIp = "any";
  
  esp_wifi_set_ps (WIFI_PS_NONE);
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
  
  
  NetworkCommunication talker (WIFI_SSID, sizeof(WIFI_SSID), WIFI_PASSWD, sizeof(WIFI_PASSWD), &LISTEN_PORT, &SYNC_PORT);
  talker_ = &talker;
  wheels = new Wheels(talker_, MOTOR_ENABLE, MOTOR_LEFT, MOTOR_RIGHT);
  steering = new Steer(talker_, STEER_SERVO);
}

void loop(){
  talker_->Handle();
  wheels->handle();
  steering->handle();
}
