#include "task_manager/task.h"
#include "config/config.h"


using namespace task_master;
  void task::print(){
    char buffer[256];
    sprintf(buffer, "task name: %s]\ntype: %d\ngpio: %d\nactive: %d\ncan_warn: %d\ncan_punish: %d\ncan_reward: %d", name.c_str(), type, gpio, active, can_warn, can_punish, can_reward);
    Serial.println(buffer);
    tod temp;
    if(can_warn){
      Serial.print("warn_time: ");Serial.print(asctime(localtime(&warn_time)));
      warning.print();
    }
    if(can_punish){
      Serial.print("punish_time: ");Serial.print(asctime(localtime(&punish_time)));
      punish.print();
    }
    if(can_reward){
      Serial.print("reward_message: "); Serial.println(reward_message.c_str());
    }
    Serial.println("\n");
  }

  //convert to JsonObject
  JsonDocument task::to_json(){

    JsonDocument doc;

    doc["name"] = name.c_str();
    doc["type"]  = type;

    doc["can_punish"] = can_punish;
    doc["can_warn"] = can_warn;
    doc["can_reward"] = can_reward;

    tod temp;

    if(can_punish){
      temp = tod(punish_time);
      doc["punish_time"] = temp.to_json();
      doc["punishment"]=punish.to_json();

    }

    if(can_warn){
      temp = tod(warn_time);
      doc["warn_time"] = temp.to_json();
      doc["warning"] = warning.to_json();
    }

    if(can_reward){
      doc["reward_message"] = reward_message.c_str();
    }

    doc["gpio"] = gpio;
    return doc;
  }

  //empty constructor
  task::task(){

  }

  //json constructor
  task::task(JsonObject object){
    //calculate the start of the day
    time_t ctime = time(nullptr);
    tm day;
    localtime_r(&ctime,&day);
    day.tm_hour = 0; day.tm_min = 0; day.tm_sec = 0;
    time_t start_of_day = mktime(&day);

    name = std::string(object["name"]);
    type = object["type"];

    can_punish = object["can_punish"];
    can_warn = object["can_warn"];
    can_reward = object["can_reward"];

    tod temp;

    if(can_punish){
      JsonArray punish_array = object["punish_time"];
      temp = tod(punish_array);
      punish_time = start_of_day + temp.to_time();
      JsonObject task_punish = object["punishment"];
      punish = openshock::control(task_punish);
    }

    if(can_warn){
      JsonArray warn_array = object["warn_time"];
      temp = tod(warn_array);
      warn_time = start_of_day + temp.to_time();
      JsonObject task_warn = object["warning"];
      warning = openshock::control(task_warn);
    }

    if(can_reward) {
      reward_message = std::string(object["reward_message"]);
    }

    gpio = object["gpio"];
    active = true;
  }

  void task::check(){
    time_t ctime = time(nullptr);
    if(digitalRead(gpio)){ //really dumb software debounce
      delay(50);
      if(digitalRead(gpio)){ //if still high, deactivate task
        active = false;
        if(can_reward){ //display reward message
          oled.write_string_8_at(reward_message, 3, 0);
          oled.timed_clear(config.message_time*1000);
        }
      }
    }else if(can_punish && punish_time == ctime){ //if still active and its time and failure is an option, zap!
      oled.write_string_8_at(punish.message, 3, 0);
      active = false; //assume that if there was a warning, it came before the punishment
      oled.timed_clear(config.message_time*1000);
    }else if(can_warn && warn_time == ctime){ //if still active and its time and this task gives a warning, zap!
      oled.write_string_8_at(warning.message, 3, 0);
      control_request(config.os_config, warning);
      if(!can_punish) active = false; //if there is no punishment, then this task is done
      oled.timed_clear(config.message_time*1000);
    }
  }

  void task::disable(){
    active = false;
  }

  void task::enable(){
    punish_time += 86400;
    warn_time += 86400;
    active = true;
  }