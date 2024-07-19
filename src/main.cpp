#include <Arduino.h>
#include "command_parser.h"
#include <oled.h>
#include <WiFi.h>
#include "web_server/web_server.h"
#include "config/config.h"
#include "wifi_manager/wifi_manager.h"
#include "task_manager/task_manager.h"
#include <LittleFS.h>

using namespace task_master;

  bool update_time(repeating_timer* t);
  bool wait = true;

  void setup() {
    Serial.begin(115200);
    #ifdef wait_for_serial
    while (!Serial);
    #endif
    pinMode(LED_BUILTIN,OUTPUT);
    digitalWrite(LED_BUILTIN,LOW);

    if(!LittleFS.begin()){
      rp2040.reboot();
    }

    conf.init();
    w_manager.init();

    if(w_manager.attempt_connection()){
      NTP.begin(conf.ntp_server.c_str());
      NTP.waitSet();
    }
    manager.init();   //initialize this after setting the time, so any repeating tasks can be properly setup
    web_server::init();

    Serial.println("Ready to receive commands...");
    digitalWrite(LED_BUILTIN,HIGH);
    wait = false;
  }

  void loop(){
    w_manager.check_connection();  
    web_server::server.handleClient();
    delay(5);
  }

  void setup1(){
    oled.init(20,21);
    oled.set_frame_callback(250,update_time);
    while(wait);
  }

  void loop1(){
    if(Serial.available() > 0) {
      parse_serial();
    }
    manager.check_tasks();
    delay(250);
  }

  bool update_time(repeating_timer* t){
    if(!oled.check_in_use()){ // if the oled isn't being written to, proceed.
      oled.load_font(font16);
      oled.cursor_pos(0, 0);
      tm temp;
      time_t now = time(nullptr); 
      localtime_r(&now, &temp); //this doesn't actually convert to local time, as this time library doesn't appear to have a way to set the timezone.... :(
      current_time = tod(temp) + timezone;
      oled.write_time_16(current_time, true);
    }
    return true;
  }
