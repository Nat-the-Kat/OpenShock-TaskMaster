#include "task_manager/task.h"
#include "config/config.h"


using namespace task_master;
  void task::print(){
    char buffer[256];
    sprintf(buffer, "task name: %s]\ntype: %d\ngpio: %d\nactive: %d\ncan_warn: %d\ncan_punish: %d\ncan_reward: %d", name.c_str(), type, gpio, active, can_warn, can_punish, can_reward);
    Serial.println(buffer);
    if(can_warn){
      Serial.print("warn_time: "); warn_time.print();
      warning.print();
    }
    if(can_punish){
      Serial.print("punish_time: "); punish_time.print();
      punish.print();
    }
    if(can_reward){
      Serial.print("reward_message :"); Serial.println(reward_message.c_str());
    }
  }

  //convert to JsonObject
  JsonDocument task::to_json(){

    JsonDocument doc;

    doc["name"] = name.c_str();
    doc["type"]  = type;

    doc["can_punish"] = can_punish;
    doc["can_warn"] = can_warn;
    doc["can_reward"] = can_reward;

    if(can_punish){
      doc["punish_time"] = punish_time.to_json();
      doc["punishment"]=punish.to_json();

    }

    if(can_warn){
      doc["warn_time"] = warn_time.to_json();
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

    name = std::string(object["name"]);
    type = object["type"];

    can_punish = object["can_punish"];
    can_warn = object["can_warn"];
    can_reward = object["can_reward"];

    if(can_punish){
      JsonArray punish_array = object["punish_time"];
      punish_time = tod(punish_array);
      JsonObject task_punish = object["punishment"];
      punish = openshock::control(task_punish);
    }

    if(can_warn){
      JsonArray warn_array = object["warn_time"];
      warn_time = tod(warn_array);
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
    if(digitalRead(gpio)){ //really dumb software debounce
      delay(50);
      if(digitalRead(gpio)){ //if still high, deactivate task
        active = false;
        if(can_reward){ //display reward message
          oled.load_font(font8);
          oled.cursor_pos(3,0);
          oled.write_string_8(reward_message);
          oled.timed_clear(conf.message_time*1000);
        }
      }
    }else if(can_punish && punish_time == current_time){ //if still active and its time and failure is an option, zap!
      oled.load_font(font8);
      oled.cursor_pos(3,0);
      oled.write_string_8(warning.message);
      control_request(conf.os_config, punish);
      active = false; //assume that if there was a warning, it came before the punishment
      oled.timed_clear(conf.message_time*1000);
    }else if(can_warn && warn_time == current_time){ //if still active and its time and this task gives a warning, zap!
      oled.load_font(font8);
      oled.cursor_pos(3,0);
      oled.write_string_8(warning.message);
      control_request(conf.os_config, warning);
      if(!can_punish) active = false; //if there is no punishment, then this task is done
      oled.timed_clear(conf.message_time*1000);
    }
  }

  void task::disable(){
    active = false;
  }