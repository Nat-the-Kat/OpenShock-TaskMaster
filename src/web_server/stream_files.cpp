#include "web_server/stream_files.h"
#include <LittleFS.h>

using namespace task_master;

void web_server::fetch_jquery(){
  File jquery_file = LittleFS.open("/portal/scripts/jquery-3.3.1.min.js", "r");
  web_server::server.streamFile(jquery_file,"text/javascript",200);//pretty sure this the wrong way to do it...
  jquery_file.close(); 
}

void web_server::fetch_config_script(){
  File config_script_file = LittleFS.open("/portal/scripts/config_script.js", "r");
  web_server::server.streamFile(config_script_file,"text/javascript",200);
  config_script_file.close(); 
}

void web_server::fetch_task_script(){
  File tasks_script_file = LittleFS.open("/portal/scripts/task_script.js", "r");
  web_server::server.streamFile(tasks_script_file,"text/javascript",200);
  tasks_script_file.close(); 
}

void web_server::fetch_network_script(){
  File network_script_file = LittleFS.open("/portal/scripts/network_script.js", "r");
  web_server::server.streamFile(network_script_file,"text/javascript",200);
  network_script_file.close(); 
}

void web_server::fetch_common_script(){
  File network_script_file = LittleFS.open("/portal/scripts/common.js", "r");
  web_server::server.streamFile(network_script_file,"text/javascript",200);
  network_script_file.close(); 
}

void web_server::fetch_css(){
  File network_script_file = LittleFS.open("/portal/i_hope_this_looks_decent.css", "r");
  web_server::server.streamFile(network_script_file,"text/css",200);
  network_script_file.close(); 
}