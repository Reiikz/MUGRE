#include "NetworkCommunication.h"
#include "SerialAsker.h"

char WIFI_SSID[50] = { '\0' };
char WIFI_PASSWD[50]  = { '\0' };

word LISTEN_PORT  = 5500;
word SEND_PORT    = 5505;
word SYNC_PORT    = 5501;

NetworkCommunication* talker;

void setup(){
  Serial.begin(115200);
  talker = new NetworkCommunication(WIFI_SSID, sizeof(WIFI_SSID), WIFI_PASSWD, sizeof(WIFI_PASSWD), &LISTEN_PORT, &SEND_PORT, &SYNC_PORT);
}

void loop(){
  talker->Handle();
}
