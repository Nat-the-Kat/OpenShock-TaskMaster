#pragma once
#include "server.h"
#include "stream_files.h"
#include "pages.h"


namespace task_master::web_server{
  void init();
  void not_found();
  void nulla();
  void config_ram();
  void tasks_ram();
  void networks_ram();

}
