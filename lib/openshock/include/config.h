#pragma once
#include <ArduinoJson.h>

namespace openshock{
  class config{
    public:
      std::string server = "api.openshock.app";
      std::string api_key = "api_key";
      std::string shocker = "shocker";

      config();
      config(JsonObject object);
      void print();
      JsonDocument to_json();

  
  };
}