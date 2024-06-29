#include <Arduino.h>
#include "command_parser.h"
#include <oled.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include "debug/bp.h"

using namespace task_master;

  config* conf = new config;
  task_manager* manager = new task_manager;
  wifi_manager* w_manager = new wifi_manager;

  bool wifi = true;

  bool frame_callback(repeating_timer* t);


  void setup() {
    Serial.begin(115200);
    oled.init(20,21);
    oled.clear();
    oled.set_frame_callback(250,&frame_callback);
    while (!Serial);

    if(!LittleFS.begin()){
      oled.cursor_pos(3,0);
      oled.write_string_8("Failed to mount file system, rebooting...");
      delay(2000);
      rp2040.reboot();
    }

    conf->init();
    manager->init();
    w_manager->init();

    int tries = 3;
    
    while(tries > 0){
      Serial.println("trying to connect...");
      if(w_manager->search_for_network()){
        break;
      }
      tries--;
    }

    if(tries == 0){
      Serial.println("no known wifi networks have been found, please modify network config!!!");
      wifi = false;
    }

    if(wifi){
      Serial.print("Connected! IP address: ");
      Serial.println(WiFi.localIP());

      NTP.begin(conf->ntp_server.c_str());
      NTP.waitSet();
    }

    Serial.println("Ready to receive commands...");
    
  }

  void loop() {
    if (Serial.available() > 0) {
      parse_serial(manager, conf, w_manager);
    }
    if(wifi){
      if(WiFi.status() != WL_CONNECTED){
        Serial.println("wifi disconnected, searching for known networks...");
        while(!w_manager->search_for_network());
        Serial.print("Connected! IP address: ");
        Serial.println(WiFi.localIP());
        NTP.begin(conf->ntp_server.c_str());
        NTP.waitSet();
      }
      manager->check_tasks(conf);
    }

  }

  void setup1(){

  }

  void loop1() {
    //while(wait); //pause the core until the config is loaded
    //delay(100);
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
