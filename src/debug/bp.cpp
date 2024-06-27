#include "debug/bp.h"
#include <arduino.h>
  //my very simple, very dumb way of stepping through programs to debug them.
  //debugger? whats that? :P
  void bp(int i){
    Serial.print("bp "); Serial.println(i);
  }

  void bp(const char* text, long i){
    Serial.print(text); Serial.println(i);
  }
