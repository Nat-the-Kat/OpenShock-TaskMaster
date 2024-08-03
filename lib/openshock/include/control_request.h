#pragma once
#include "ArduinoJson.h"
#include <HTTPClient.h>
#include "control.h"
#include "config.h"

namespace openshock{
  void control_request(config conf, control c);
}

