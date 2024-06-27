#pragma once
#include <ArduinoJson.h>

namespace task_master{
  class control{
    public:
      int strength = 0;
      std::string type = "type";
      int dur = 0;
      std::string message = "message";


      void print();
      JsonDocument to_json();
      control();
      control(JsonObject object);

  };
}

