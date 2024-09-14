#include "web_server/receive_data.h"
#include "web_server/web_server.h"
#include "wifi_manager/wifi_manager.h"
#include "task_manager/task_manager.h"
#include "config/config.h"
#include <LittleFS.h>
#include <ArduinoJson.h>

using namespace task_master;

#define dest_config 1
#define dest_tasks 2
#define dest_networks 3

void web_server::config_write(){
  recieve_data(dest_config);
  config.init(); //restart config
  Serial.println("config updated");
}

void web_server::tasks_write(){
  recieve_data(dest_tasks);
  task_manager.init(); //restart task manager
  Serial.println("task list updated");
}

void web_server::networks_write(){
  recieve_data(dest_networks);
  w_manager.init(); //restart network manager
  Serial.println("network list updated");
}

void web_server::recieve_data(int dest){
  JsonDocument doc;
  HTTPUpload& upload = web_server::server.upload();
  deserializeJson(doc,upload.buf,upload.currentSize);
  File file;
  switch(dest){
    case dest_config:
      file = LittleFS.open("config.json", "w");
      break;
    case dest_tasks:
      file = LittleFS.open("tasks.json", "w");
      break;
    case dest_networks:
      file = LittleFS.open("wifi.json", "w");
      break;
    default:
      break;
  }
  serializeJson(doc,file);
  file.close();
  web_server::server.send(200,"text/plain","ok");
}
