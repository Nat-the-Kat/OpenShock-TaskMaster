#include "web_server/receive_data.h"
#include "web_server/web_server.h"
#include "wifi_manager/wifi_manager.h"
#include "task_manager/task_manager.h"
#include "config/config.h"
#include <LittleFS.h>
#include <ArduinoJson.h>

using namespace task_master;

void web_server::config_flash(){
  JsonDocument doc;
  HTTPUpload& upload = web_server::server.upload();
  deserializeJson(doc,upload.buf,upload.currentSize);
  File config_file = LittleFS.open("config.json", "w");
  serializeJson(doc,config_file);
  config_file.close();
  conf.init(); //restart config
  web_server::server.send(200,"text/plain","ok");
  Serial.println("config updated");
}

void web_server::tasks_flash(){
  JsonDocument doc;
  HTTPUpload& upload = web_server::server.upload();
  deserializeJson(doc,upload.buf,upload.currentSize);
  File task_file = LittleFS.open("tasks.json", "w");
  serializeJson(doc,task_file);
  task_file.close();
  manager.init(); //restart task manager
  web_server::server.send(200,"text/plain","ok");
  Serial.println("task list updated");
}

void web_server::networks_flash(){
  JsonDocument doc;
  HTTPUpload& upload = web_server::server.upload();
  deserializeJson(doc,upload.buf,upload.currentSize);
  File network_file = LittleFS.open("wifi.json", "w");
  serializeJson(doc,network_file);
  network_file.close();
  w_manager.init(); //restart network manager
  web_server::server.send(200,"text/plain","ok");
  Serial.println("network list updated");
}
