#include "task_manager/task_timed.h"
#include "config/config.h"
#include "helpers/time_helper.h"

using namespace task_master;
  void task_timed::print(){
    char buffer[256];
    sprintf(buffer, "task name: %s]\ntype: %d\ngpio: %d\nactive: %d\ncan_warn: %d\ncan_punish: %d\ncan_reward: %d", name.c_str(), type, gpio, active, can_warn, can_punish, can_reward);
    Serial.println(buffer);
    Serial.print("start of time window: ");Serial.print(asctime(localtime(&window)));
    Serial.print("can_warn: "); Serial.println(can_warn);
    Serial.print("can_punish: "); Serial.println(can_punish);
    Serial.print("can_reward: "); Serial.println(can_reward);

    if(can_warn){
      Serial.print("warn_time: ");Serial.print(asctime(localtime(&warn_time)));
      warning.print();
    }
    if(can_punish){
      Serial.print("punish_time: ");Serial.print(asctime(localtime(&punish_time)));
      punish.print();
    }
    if(can_reward){
      Serial.print("reward_message:"); Serial.println(reward_message.c_str());
    }
    Serial.println("\n");
  }

  JsonDocument task_timed::to_json(){
    JsonDocument doc;

    doc["name"] = name.c_str();
    doc["type"]  = type;

    tod temp;
    temp = tod(window);

    doc["window"] = temp.to_json();

    doc["can_punish"] = can_punish;
    doc["can_warn"] = can_warn;
    doc["can_reward"] = can_reward;

    

    if(can_punish){
      temp = tod(punish_time,true);
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

  task_timed::task_timed(){

  }

  task_timed::task_timed(JsonObject object){
    //calculate the start of the day
    time_t ctime = time(nullptr);
    tm day;
    localtime_r(&ctime,&day);
    day.tm_hour = 0; day.tm_min = 0; day.tm_sec = 0;
    time_t day_start = start_of_day();

    name = std::string(object["name"]);
    type = object["type"];

    tod temp_window;
    tod temp_warn;
    tod temp_punish;

    JsonArray window_array = object["window"];
    temp_window = tod(window_array);
    window = day_start + temp_window.to_time();

    can_punish = object["can_punish"];
    can_warn = object["can_warn"];
    can_reward = object["can_reward"];

    if(can_punish){
      JsonArray punish_array = object["punish_time"];
      
      temp_punish = tod(punish_array);
      punish_time = day_start + temp_punish.to_time();
      JsonObject task_punish = object["punishment"];
      punish = openshock::control(task_punish);
    }

    if(can_warn){
      JsonArray warn_array = object["warn_time"];
      temp_warn = tod(warn_array);
      warn_time = day_start + temp_warn.to_time();
      JsonObject task_warn = object["warning"];
      warning = openshock::control(task_warn);
    }

    if(can_reward) {
      reward_message = std::string(object["reward_message"]);
    } 

    gpio = object["gpio"];

    //calculate the time window
    if(can_punish && can_warn){  //calculate the window start time from the warning time, note: this assumes the warning comes before the punishment
      window = temp_warn.to_time() - temp_window.to_time() + day_start;
    }else if(can_punish && !can_warn){ //there is no warning, calculate the window start time from the punishment time
      window = temp_punish.to_time() - temp_window.to_time() + day_start;
    }else if(!can_punish && can_warn){
      window = temp_warn.to_time() - temp_window.to_time() + day_start;
    }else{ //if !can_warn and !can_punish, assume the task is garbage and ignore it
      return;
    }
 

  }

  void task_timed::check(){
    time_t ctime = time(nullptr);

    if(ctime > window && active){ //inside the time window, and the task is still active
      if(digitalRead(gpio)){ //check if its being deactivated...
        delay(50);
        if(digitalRead(gpio)){ //if still high, deactivate task
          active = false;
          oled.write_string_8_at(reward_message, 3, 0);
          oled.timed_clear(config.message_time*1000);
        }
      }
    }
    
    if(can_punish && punish_time == ctime && active){ //if still active and its time and failure is an option, zap!
      oled.write_string_8_at(punish.message, 3, 0);
      control_request(config.os_config, punish);
      active = false; //assume that if there was a warning, it came before the punishment
      punish_time += 86400;
      oled.timed_clear(config.message_time*1000);
    }else if(can_warn && warn_time == ctime && active){ //if still active and its time and this task gives a warning, zap!
      oled.write_string_8_at(warning.message, 3, 0);
      control_request(config.os_config, warning);
      if(!can_punish) active = false; //if there is no punishment, then this task is done
      warn_time += 86400;
      oled.timed_clear(config.message_time*1000);
    }
  }

  void task_timed::disable(){
    active = false;
  }

  void task_timed::enable(){
    window += 86400;
    punish_time += 86400;
    warn_time += 86400;
    active = true;
  }