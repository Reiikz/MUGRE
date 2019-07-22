#include "NetworkCommunication.h"

//char* WIFI_SSID = ":'v':";
char* WIFI_SSID = "Ceibal-2.4GHz";
char* WIFI_PASSWD = "";
//char* WIFI_PASSWD = "thedragonsarekawaii";

word LISTEN_PORT  = 5500;
word SEND_PORT    = 5505;
word SYNC_PORT    = 5501;

NetworkCommunication* talker;

void setup(){
  Serial.begin(115200);
  talker = new NetworkCommunication(WIFI_SSID, WIFI_PASSWD, &LISTEN_PORT, &SEND_PORT, &SYNC_PORT);
}

void loop(){
  talker->Handle();
}
