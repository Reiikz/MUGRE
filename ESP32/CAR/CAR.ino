#include "esp_wifi.h"
#include <WiFi.h>

#include "NetworkCommunication.h"

#include "Steer.h"
#include "Wheels.h"

#include "SerialAsker.h"
#include "Logger.h"

static const byte MAX_CONNECTION_ATTEMPTS = 50;
static const word CONNECTION_ATTEMPT_ESTIMATED_INTERVAL = 700;

const int ssid_size = 30;
const int pass_size = 30;
char SSID[ssid_size] =  ":'v':"; // { '\0' };
char PASS[pass_size] =  "thedragonsarekawaii"; //{ '\0' };

word LISTEN_PORT  = 5500;
word SYNC_PORT    = 5505;

#define MOTOR_ENABLE  (byte)  13
#define MOTOR_LEFT    (byte)  12
#define MOTOR_RIGHT   (byte)  14
#define STEER_SERVO   (byte)  4

NetworkCommunication* talker;
Wheels* wheels;
Steer* steering;

void setup(){
  Serial.begin(115200);


  /*
   * -------------------------------------------------------------------------------------------------------------------------------------------------
   */
      if(strlen(SSID) == 0){
        SerialAsker::AskForCString("Enter WIFI SSID", SSID, ssid_size);
      }
      
      if(strlen(PASS) == 0){
        SerialAsker::AskForCString("Enter WIFI Password:", PASS, pass_size); 
      }
      int ConnectionAttemptCount = 0;
      
      //WiFi.setHostname("SensorController");
      Logger::logwinfo(String("[SSID]: '") + String(SSID) + String("'"));
      Logger::logwinfo((String("[PASSWORD]: '") + String(PASS)  + String("'")));
      
      //WiFi.mode(WIFI_STA);
      //esp_wifi_set_ps (WIFI_PS_NONE);
      WiFi.begin(SSID, PASS);
      unsigned long LastConnectionAttempt = 0;
      while(WiFi.status() != WL_CONNECTED && ConnectionAttemptCount < MAX_CONNECTION_ATTEMPTS){
        if(millis() - LastConnectionAttempt > CONNECTION_ATTEMPT_ESTIMATED_INTERVAL){
          ConnectionAttemptCount++;
          Logger::logwinfo(("[CONNECTION ATTEMPT]: " + String(ConnectionAttemptCount)));
          LastConnectionAttempt = millis();
          delay(CONNECTION_ATTEMPT_ESTIMATED_INTERVAL);
        }
      }
      
      if(ConnectionAttemptCount < MAX_CONNECTION_ATTEMPTS){
        IPAddress ip = WiFi.localIP();
        Logger::logwinfo( (String("IP ADDRESS: ") + String(ip[0]) + "." + String(ip[1]) + "." + String(ip[2]) + "." + String(ip[3])) );
      }else{
        while(1){
         Logger::logwerror(String("Unable to connect wifi, reset to retry."));
         delay(CONNECTION_ATTEMPT_ESTIMATED_INTERVAL);
        }
      }
  /*
   * -------------------------------------------------------------------------------------------------------------------------------------------------
   */
  
  
  talker = new NetworkCommunication(&SYNC_PORT, new WiFiServer(LISTEN_PORT));
  wheels = new Wheels(talker, MOTOR_ENABLE, MOTOR_LEFT, MOTOR_RIGHT);
  steering = new Steer(talker, STEER_SERVO);
}


void loop(){
  talker->handle();
  wheels->handle();
  steering->handle();
}
