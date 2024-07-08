#pragma once
#include "server.h"

//this is for everything that requires transfering files
namespace task_master::web_server{
  void config_flash();
  void tasks_flash();
  void networks_flash();
  void fetch_jquery();
  void fetch_config_script();
  void fetch_task_script();
  void fetch_network_script();
  void fetch_common_script();
  void fetch_css();
}
