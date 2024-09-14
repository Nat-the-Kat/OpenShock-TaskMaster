#pragma once
#include "server.h"
#include "stream_files.h"
#include "pages.h"
#include "receive_data.h"


namespace task_master::web_server{
  void init();
  void not_found();
  void config_read();
  void tasks_read();
  void networks_read();
}
