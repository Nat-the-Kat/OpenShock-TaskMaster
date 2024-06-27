#include "task_manager/task_timed.h"

using namespace task_master;
  void task_timed::print(){
    char buffer[100];
    sprintf(buffer, "task name: %s]\ntype: %d\ngpio: %d\nactive: %d\ncan_warn: %d\ncan_punish: %d\ncan_reward: %d", name.c_str(), type, gpio, active, can_warn, can_punish, can_reward);
    Serial.println(buffer);
    Serial.print("window: "); window.print();
    Serial.print("can_warn: "); Serial.println(can_warn);
    Serial.print("can_punish: "); Serial.println(can_punish);
    Serial.print("can_reward: "); Serial.println(can_reward);
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

  JsonDocument task_timed::to_json(){
    JsonDocument doc;

    doc["name"] = name.c_str();
    doc["type"]  = type;

    doc["window"] = window.to_json();

    doc["can_punish"] = can_punish;
    doc["can_warn"] = can_warn;
    doc["can_reward"] = can_reward;

    if(can_punish){
      doc["punishment"]=punish.to_json();
    }

    if(can_warn){
      doc["warning"]=warning.to_json();
    }

    if(can_warn){
      doc["reward_message"] = reward_message.c_str();
    }

    doc["gpio"] = gpio;
    return doc;
  }

  task_timed::task_timed(){

  }

  task_timed::task_timed(JsonObject object){
    name = std::string(object["name"]);
    type = object["type"];

    JsonArray window_array = object["start"];
    window = tod(window_array);

    can_punish = object["can_punish"];
    can_warn = object["can_warn"];
    can_reward = object["can_reward"];

    if(can_punish){
      JsonObject task_punish = object["punishment"];
      punish = control(task_punish);
    }

    if(can_warn){
      JsonObject task_warn = object["warning"];
      warning = control(task_warn);
    }

    if(can_reward) {
      reward_message = std::string(object["reward_message"]);
    } 

    gpio = object["gpio"];

 

  }

  void task_timed::check(config* conf){
    // check if we are in the allowed window
    tod start;
    tod end;
    if(can_punish && can_warn){  //calculate the window start time from the warning time, note: this assumes the warning comes before the punishment
      start = warn_time - window;
      end = punish_time;
    }else if(can_punish && !can_warn){ //there is no warning, calculate the window start time from the punishment time
      start = punish_time - window;
      end = punish_time;
    }else if(!can_punish && can_warn){
      start = warn_time - window;
      end = warn_time;
    }else{ // if !can_warn and !can_punish, assume the task is garbage and ignore it
      return;
    }

    if(between(current_time, start, end) && active){ //inside the time window, and the task is still active
      if(digitalRead(gpio)){ //check if its being deactivated...
        delay(50);
        if(digitalRead(gpio)){ //if still high, deactivate task
          active = false;
          oled.load_font(font8);
          oled.cursor_pos(3,0);
          oled.write_string_8(reward_message);
          oled.timed_clear(conf->message_time*1000);
        }
      }
    }
    
    if(can_punish && punish_time == current_time && active){ //if still active and its time and failure is an option, zap!
      oled.load_font(font8);
      oled.cursor_pos(3,0);
      oled.write_string_8(warning.message.c_str());
      control_request(conf, punish);
      active = false; //assume that if there was a warning, it came before the punishment
      oled.timed_clear(conf->message_time*1000);
    }else if(can_warn && warn_time == current_time && active){ //if still active and its time and this task gives a warning, zap!
      oled.load_font(font8);
      oled.cursor_pos(3,0);
      oled.write_string_8(warning.message.c_str());
      control_request(conf, warning);
      if(!can_punish) active = false; //if there is no punishment, then this task is done
      oled.timed_clear(conf->message_time*1000);
    }
  }