#pragma once
#include <vector>
#include <ArduinoJson.h>
#include "wifi_manager/wifi_config.h"
#include <string>


#define max_retry_count 5



namespace task_master{
  class wifi_manager_class{
    public:
      void init();
      void print(); 

      std::string write_to_string();

      void check_connection();
      bool attempt_connection(); //try to connect to a known network, else fallback to the configuration network 

    private:
      uint8_t retry_count;
      std::vector <wifi_config> wifi_configs;
      
      void add_network(JsonObject object);
      bool start_config_ap();
      void read_from_file(); //read wifi.json into ram

  };
}

extern task_master::wifi_manager_class wifi_manager;


