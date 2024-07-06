#include "web_server/pages.h"
#include <LittleFS.h>

using namespace task_master;

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