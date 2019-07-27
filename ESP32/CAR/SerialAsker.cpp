#include <Arduino.h>
#include "SerialAsker.h"

String SerialAsker::AskForString(const char* message){
  Serial.println(message);
  while(! Serial.available());
  String in = Serial.readString();
  Serial.println(in);
  return in;
}


void SerialAsker::AskForCString(const char* message, char buff[], int size_){
  SerialAsker::AskForCString(message, buff, size_, false);
}

void SerialAsker::AskForCString(const char* message, char buff[], int size_, bool lineJump){
  while(Serial.available()){
    Serial.read();
  }
  Serial.println(message);
  while(! Serial.available());
  char c;
  for(int i = 0; Serial.available() ; i++){
    c = Serial.read();
    if(!lineJump && c == '\n'){
      continue;
    }
    buff[i] = c;
  }
  Serial.println(buff);
}
