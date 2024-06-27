#pragma once
#include <ArduinoJson.h>

namespace task_master{
  class wifi_config{
    public:
      std::string ssid = "ssid";
      std::string password = "password";
      
      void print();
      JsonDocument to_json();
      
      wifi_config();
      wifi_config(JsonObject object);
      wifi_config(const char* s, const char* p);
  };
}


