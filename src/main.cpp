#include <Arduino.h>
#include "command_parser.h"
#include <oled.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include "debug/bp.h"
#include "web_server/web_server.h"
#include "config/config.h"
#include "wifi_manager/wifi_manager.h"
#include "task_manager/task_manager.h"
#include <LittleFS.h>

using namespace task_master;

  //config* conf = new config;
  //task_manager* manager = new task_manager;
  //wifi_manager* w_manager = new wifi_manager;

  bool frame_callback(repeating_timer* t);

  bool wifi = true;

  void setup() {
    Serial.begin(115200);
    oled.init(20,21);
    oled.clear();
    oled.set_frame_callback(250,&frame_callback);
    //while (!Serial);
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

    int tries = 3;
    
    while(tries > 0){
      Serial.println("trying to connect...");
      if(w_manager.search_for_network()){
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

      NTP.begin(conf.ntp_server.c_str());
      NTP.waitSet();

    }
      web_server::init();

    Serial.println("Ready to receive commands...");
    digitalWrite(LED_BUILTIN,HIGH);
  }

  void loop() {
    if(Serial.available() > 0) {
      parse_serial();
    }
    if(wifi){ 
      /*if we have a wifi connection, check to make sure we are still connected, then if true, check tasks, other wise don't bother as we cannot act
      if a task is requesting a shock because we cannot send the request.
      */
      if(WiFi.status() != WL_CONNECTED){
        Serial.println("wifi disconnected, searching for known networks...");
        while(!w_manager.search_for_network());
        Serial.print("Connected! IP address: ");
        Serial.println(WiFi.localIP());
        NTP.begin(conf.ntp_server.c_str());
        NTP.waitSet();
      }else{
        //we have wifi, so check tasks
        manager.check_tasks();

      }
      
    }
    web_server::server.handleClient();
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
