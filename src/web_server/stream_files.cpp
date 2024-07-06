#include "web_server/stream_files.h"
#include <LittleFS.h>

using namespace task_master;

void web_server::config_flash(){
  File config_file = LittleFS.open("/config.json", "r");
  web_server::server.streamFile(config_file,"application/json",200);
  config_file.close();  
}

void web_server::tasks_flash(){
  File tasks_file = LittleFS.open("/tasks.json", "r");
  web_server::server.streamFile(tasks_file,"application/json",200);
  tasks_file.close(); 
}

void web_server::networks_flash(){
  File networks_file = LittleFS.open("/wifi.json", "r");
  web_server::server.streamFile(networks_file,"application/json",200);
  networks_file.close(); 
}

void web_server::fetch_jquery(){
  File jquery_file = LittleFS.open("/portal/scripts/jquery-3.3.1.min.js", "r");
  web_server::server.streamFile(jquery_file,"text/plain",200);//pretty sure this the wrong way to do it...
  jquery_file.close(); 
}

void web_server::fetch_config_script(){
  File config_script_file = LittleFS.open("/portal/scripts/config_script.js", "r");
  web_server::server.streamFile(config_script_file,"text/plain",200);
  config_script_file.close(); 
}

void web_server::fetch_task_script(){
  File tasks_script_file = LittleFS.open("/portal/scripts/task_script.js", "r");
  web_server::server.streamFile(tasks_script_file,"text/plain",200);
  tasks_script_file.close(); 
}

void web_server::fetch_network_script(){
  File network_script_file = LittleFS.open("/portal/scripts/network_script.js", "r");
  web_server::server.streamFile(network_script_file,"text/plain",200);
  network_script_file.close(); 
}