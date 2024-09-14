#include "web_server/web_server.h"
#include "wifi_manager/wifi_manager.h"
#include "task_manager/task_manager.h"
#include "config/config.h"
#include <WebServer.h>

using namespace task_master;

WebServer web_server::server(80);


void web_server::not_found(){
  web_server::server.send(404, "text/plain", "whatever you are looking for doesn't exist!");
}

void web_server::init(){
  
  web_server::server.on("/", web_server::index_page);
  web_server::server.on("/index.html", web_server::index_page);
  web_server::server.on("/config.html", web_server::config_page);
  web_server::server.on("/networks.html", web_server::networks_page);
  web_server::server.on("/tasks.html", web_server::tasks_page);

  web_server::server.on("/data/config", HTTP_POST, web_server::config_write);
  web_server::server.on("/data/tasks", HTTP_POST, web_server::tasks_write);
  web_server::server.on("/data/networks", HTTP_POST, web_server::networks_write);

  web_server::server.on("/data/config", HTTP_GET, web_server::config_read);
  web_server::server.on("/data/tasks", HTTP_GET, web_server::tasks_read);
  web_server::server.on("/data/networks", HTTP_GET, web_server::networks_read);

  web_server::server.on("/scripts/jquery-3.3.1.min.js", web_server::fetch_jquery);
  web_server::server.on("/scripts/config_script.js", web_server::fetch_config_script);
  web_server::server.on("/scripts/task_script.js", web_server::fetch_task_script);
  web_server::server.on("/scripts/network_script.js", web_server::fetch_network_script);
  web_server::server.on("/scripts/common.js", web_server::fetch_common_script);
  web_server::server.on("/scripts/tz_text.js", web_server::fetch_tz_text);

  web_server::server.on("/i_hope_this_looks_decent.css", web_server::fetch_css);
  
  web_server::server.onNotFound(not_found);
  web_server::server.begin();
  Serial.println("server started!");
}

void web_server::config_read(){
  web_server::server.send(200, "application/json", config.write_to_string().c_str());
}

void web_server::tasks_read(){
  web_server::server.send(200, "application/json", task_manager.write_to_string().c_str());
}
   
void web_server::networks_read(){
  web_server::server.send(200, "application/json", w_manager.write_to_string().c_str());
}

