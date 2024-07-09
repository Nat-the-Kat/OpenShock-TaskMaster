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

  bool frame_callback(repeating_timer* t);


  void setup() {
    rp2040.idleOtherCore();
    Serial.begin(115200);
    oled.init(20,21);
    oled.clear();
    oled.set_frame_callback(250,&frame_callback);

    while (!Serial);
    pinMode(LED_BUILTIN,OUTPUT);
    digitalWrite(LED_BUILTIN,LOW);

    if(!LittleFS.begin()){
      oled.cursor_pos(3,0);
      oled.write_string_8("Failed to mount file system, rebooting...");
      delay(2000);
      rp2040.reboot();
    }

    conf.init();
    manager.init();
    w_manager.init();

    if(w_manager.attempt_connection()){
      NTP.begin(conf.ntp_server.c_str());
      NTP.waitSet();
    }

    web_server::init();

    Serial.println("Ready to receive commands...");
    digitalWrite(LED_BUILTIN,HIGH);
    rp2040.resumeOtherCore();
  }

  void loop() {
    if(Serial.available() > 0) {
      parse_serial();
    }
    w_manager.check_connection();  
    //manager.check_tasks();
    web_server::server.handleClient();
  }

  void setup1(){

  }

  void loop1(){

  }

  bool frame_callback(repeating_timer* t){
    oled.load_font(font16);
    oled.cursor_pos(0, 0);
    tm temp;
    time_t now = time(nullptr); 
    localtime_r(&now, &temp); //this doesn't actually convert to local time, as this time library doesn't appear to have a way to set the timezone.... :(
    current_time = tod(temp) + timezone;
    oled.write_time_16(current_time, true);
    return true;
  }
