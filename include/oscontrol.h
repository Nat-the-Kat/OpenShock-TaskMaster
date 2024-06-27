#pragma once
#include "config/config.h"
#include <HTTPClient.h>
#include "task_manager/control.h"

namespace task_master{
  void control_request(config* conf, control c);
}

