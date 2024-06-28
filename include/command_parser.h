#pragma once
#include "config/config.h"
#include "wifi_manager/wifi_manager.h"
#include "task_manager/task_manager.h"
#include "helpers/filesystem.h"
#include <pico/stdlib.h>

namespace task_master {
  void parse_serial(task_manager* manager, config* conf, wifi_manager* w_manager);

  void help(const char* string);
}

