#pragma once
#include <vector>
#include <ArduinoJson.h>
#include "wifi_manager/wifi_config.h"
#include <string>

namespace task_master{
  class wifi_manager{
    public:
      void init();
      void read_from_file(); //read wifi.json into ram
      void write_to_file(); //write config in ram to wifi.json

      //wifi management functions
      void add_network(JsonObject object);
      void add_networks();
      void delete_network(const char* ssid);
      void clear_networks();
      void print_networks();
      bool search_for_network();

      std::string write_to_string();
    private:
      std::vector <wifi_config> wifi_configs;
      void read_from_stream(Stream &s);
      
  };
}
extern task_master::wifi_manager w_manager;


