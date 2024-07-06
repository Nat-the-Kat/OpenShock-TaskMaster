#pragma once
#include <tod.h>
#include <openshock.h>
#include <oled.h>
#include <pico/stdlib.h>

namespace task_master{
  class task{
    public:
      uint8_t gpio = 0;  //gpio pin used to call off the task
      uint8_t type = 0;
      std::string name = "task_name";

      bool can_warn = false; //does this task give a warning?
      tod warn_time;
      openshock::control warning;

      bool can_punish = false; //is there a punishment assinged to this task?
      tod punish_time;
      openshock::control punish;

      bool can_reward = false;
      std::string reward_message = "reward_message"; //message to display on screen if the task is completed on time.

      bool active = true;  //does the task still need to be done today?

      virtual void print();
      virtual JsonDocument to_json();
      virtual void check();
      

      task();
      task(JsonObject object);

  };
}
