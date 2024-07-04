#include "web_server/web_server.h"

using namespace task_master;

WebServer web_server::server(80);

void web_server::not_found(){
  web_server::server.send(404,"text/plain","whatever you are looking for doesn't exist!");
}

void web_server::index_page(){
  File index_file = LittleFS.open("/portal/index.html", "r");
  web_server::server.streamFile(index_file,"text/html",200);
  index_file.close();
}

void web_server::config_page(){
  File config_file = LittleFS.open("/portal/config.html", "r");
  web_server::server.streamFile(config_file,"text/html",200);
  config_file.close();
}

void web_server::networks_page(){
  File networks_file = LittleFS.open("/portal/networks.html", "r");
  web_server::server.streamFile(networks_file,"text/html",200);
  networks_file.close();
}

void web_server::tasks_page(){
  File tasks_file = LittleFS.open("/portal/tasks.html", "r");
  web_server::server.streamFile(tasks_file,"text/html",200);
  tasks_file.close();
}

void web_server::init(){
  
  web_server::server.on("/", web_server::index_page);
  web_server::server.on("/index.html", web_server::index_page);
  web_server::server.on("/config.html", web_server::config_page);
  web_server::server.on("/networks.html", web_server::networks_page);
  web_server::server.on("/tasks.html", web_server::tasks_page);

  web_server::server.on("/flash/config", web_server::config_flash);
  web_server::server.on("/flash/tasks", web_server::tasks_flash);
  web_server::server.on("/flash/networks", web_server::networks_flash);

  web_server::server.on("/ram/config", web_server::config_ram);
  web_server::server.on("/ram/tasks", web_server::tasks_ram);
  web_server::server.on("/ram/networks", web_server::networks_ram);

  web_server::server.on("/scripts/jquery-3.3.1.min.js",web_server::fetch_jquery);

  web_server::server.onNotFound(not_found);
  web_server::server.begin();
  Serial.println("server started!");
}

void web_server::config_ram(){

}

void web_server::config_flash(){
  File config_file = LittleFS.open("/config.json", "r");
  web_server::server.streamFile(config_file,"application/json",200);
  config_file.close();  
}

void web_server::tasks_ram(){

}

void web_server::tasks_flash(){
  File tasks_file = LittleFS.open("/tasks.json", "r");
  web_server::server.streamFile(tasks_file,"application/json",200);
  tasks_file.close(); 
}
    
void web_server::networks_ram(){

}

void web_server::networks_flash(){

}

void web_server::fetch_jquery(){
  File jquery_file = LittleFS.open("/scripts/jquery-3.3.1.min.js", "r");
  web_server::server.streamFile(jquery_file,"text/plain",200);//pretty sure this the wrong way to do it...
  jquery_file.close(); 
}