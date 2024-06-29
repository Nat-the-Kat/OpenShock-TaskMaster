#pragma once
#include <ArduinoJson.h>

namespace openshock{
  class control{
    public:
      int intensity = 0;
      std::string type = "type";
      int duration = 0;
      std::string message = "message";


      void print();
      JsonDocument to_json();
      control();
      control(JsonObject object);

  };
}

