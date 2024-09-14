#pragma once
#include <vector>
#include <ArduinoJson.h>
#include "wifi_manager/wifi_config.h"
#include <string>


#define max_retry_count 5



namespace task_master{
  class wifi_manager{
    public:
      void init();
      void reset();

      std::string write_to_string();

      //wifi network management functions
      void print_networks();    

      void check_connection();
      bool attempt_connection(); //try to connect to a known network, else fallback to the configuration network 

    private:
      uint8_t retry_count;
      std::vector <wifi_config> wifi_configs;
      
      void add_network(JsonObject object);
      bool read_from_stream(Stream &s);
      bool start_config_ap();
      void read_from_file(); //read wifi.json into ram
      void write_to_file(); //write config in ram to wifi.json


  };
}

extern task_master::wifi_manager w_manager;


