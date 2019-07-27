#include <Arduino.h>

#pragma once


class SerialAsker{
  public:
  
    static String AskForString(const char*);
    static void AskForCString(const char* message, char buff[], int size_, bool lineJump);
    static void AskForCString(const char* message, char buff[], int size_);
  
};
