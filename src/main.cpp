#include <Arduino.h>
#include <LittleFS.h>
#include <oled.h>
#include <WiFi.h>

#include "command_parser.h"
#include "config/config.h"
#include "task_manager/task_manager.h"
#include "web_server/web_server.h"
#include "wifi_manager/wifi_manager.h"

using namespace task_master;

  bool update_time(repeating_timer* t);

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

    config.init();
    w_manager.init();

    if(w_manager.attempt_connection()){
      NTP.begin(config.ntp_server.c_str());
      time_t now = time(nullptr);
      while (now < 8 * 3600 * 2) {
        delay(500);
        Serial.print(".");
        now = time(nullptr);
      }
    }
    update_time();
    task_manager.init(); //initialize this after setting the time, so any repeating tasks can be properly setup
    web_server::init();

    Serial.println("Ready to receive commands...");
    digitalWrite(LED_BUILTIN,HIGH);
  }

  void loop(){
    w_manager.check_connection();  
    web_server::server.handleClient();
    delay(5);
  }

  void setup1(){
    oled.init(20,21);
    oled.clear();
    oled.set_frame_callback(250, update_time);    
  }

  void loop1(){
    if(Serial.available() > 0) {
      parse_serial();
    }
    task_manager.check_tasks();
    delay(250);
  }

  bool update_time(repeating_timer* t){
    if(!oled.check_in_use()){ // if the oled isn't being written to, proceed.
      oled.load_font(font16);
      oled.cursor_pos(0, 0);
      update_time();
      oled.write_time_16(current_time, true);
    }
    return true;
  }
