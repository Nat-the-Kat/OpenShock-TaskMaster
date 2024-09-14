#pragma once
#include <ArduinoJson.h>
#include <openshock.h>
#include <tod.h>
#include <string>

namespace task_master{
  class config_class{
    public:
      std::string ntp_server = "pool.ntp.org";
      openshock::config os_config;

      tod reset_time; //time that the override resets at
      int reset_day = 0;
      bool can_override = true; //is there a master override?
      int override_pin = 0; //gpio pin used to signal that the user wants to override the days tasks
      int num_overrides = 0; //number of times daily tasks can be overridden per week
      int overrides_left = 0; //how many overrides are left for the week
      int dow = 0;
      int message_time = 5;
      
      void print(); //print config info in ram to serial terminal
      void init();
      
      std::string write_to_string();

    private:
      std::string timezone_name;
      std::string timezone_rule; 
      void read_from_stream(Stream &s);
      void read_from_file(); //read config.json into ram
      void write_to_file(); //write config in ram to config.json
      
  };
}

extern task_master::config_class config;

  