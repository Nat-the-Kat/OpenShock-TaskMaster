#include <FreeRTOS.h>
#include <task.h>
#include <semphr.h>
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

  void update_time(void *param);
  void loop_1(void *param);
  void webserver_loop(void *param);

  void setup() {
    //interrupts
    Serial.begin(115200);

    //while (!Serial);
    pinMode(LED_BUILTIN,OUTPUT);
    digitalWrite(LED_BUILTIN,LOW);

    if(!LittleFS.begin()){
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

    xTaskCreateAffinitySet(webserver_loop, "webserver_loop", 4096, nullptr,1,0b1,nullptr);
  }

  //core0's only job
  void webserver_loop(void *param) {
    while(true){
      w_manager.check_connection();  
      web_server::server.handleClient();
      vTaskDelay(1);
    }
  }

  void loop(){
    vTaskDelete(nullptr); //bye-bye!!
  }

  

  void setup1(){
    oled.init(20,21);
    xTaskCreateAffinitySet(update_time, "update_time", 256, nullptr,1,0b10,nullptr);
    xTaskCreateAffinitySet(loop_1, "loop_1", 1024, nullptr,1,0b10,nullptr);
  }

  void loop1(){
    vTaskDelete(nullptr); //bye-bye!!
  }


  void loop_1(void *param){
    while(true){
    if(Serial.available() > 0) {
      parse_serial();
    }
    manager.check_tasks();
    vTaskDelay(100);
    }

  }

  void update_time(void *param){
    while(true){
      oled.load_font(font16);
      oled.cursor_pos(0, 0);
      tm temp;
      time_t now = time(nullptr); 
      localtime_r(&now, &temp); //this doesn't actually convert to local time, as this time library doesn't appear to have a way to set the timezone.... :(
      current_time = tod(temp) + timezone;
      oled.write_time_16(current_time, true);
      vTaskDelay(250);
    }
  }
