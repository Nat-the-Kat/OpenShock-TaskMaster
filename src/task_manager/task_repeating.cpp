#include "task_manager/task_repeating.h"
#include "config/config.h"
#include "helpers/time_helper.h"

using namespace task_master;

  void task_repeat::print(){
    char buffer[256];
    sprintf(buffer, "task name: %s]\ntype: %d\ngpio: %d\nactive: %d\ncan_warn: %d\ncan_punish: %d\ncan_reward: %d", name.c_str(), type, gpio, active, can_warn, can_punish, can_reward);
    Serial.println(buffer);
    Serial.print("start: ");Serial.print(asctime(localtime(&start)));
    Serial.print("end: ");Serial.print(asctime(localtime(&end)));
    Serial.print("interval: ");Serial.print(asctime(gmtime(&interval)));
    Serial.print("next_time: ");Serial.print(asctime(localtime(&next_time)));
    if(can_warn){
      Serial.print("next_warn: ");Serial.print(asctime(localtime(&next_warn)));
      warning.print();
    }
    if(can_punish){
      Serial.print("next_punish: ");Serial.print(asctime(localtime(&next_punish)));
      punish.print();
    }
    if(can_reward){
      Serial.print("reward_message: "); Serial.println(reward_message.c_str());
    }
    Serial.println("\n");
  }

  //convert to JsonObject
  JsonDocument task_repeat::to_json(){
    JsonDocument doc;
    

    doc["name"] = name.c_str();
    doc["type"]  = type;
    tod temp;
    temp = tod(start);
    doc["start"] = temp.to_json();
    temp = tod(end);
    doc["end"] = temp.to_json();
    temp = tod(interval,true);
    doc["interval"] = temp.to_json();
    doc["can_punish"] = can_punish;
    doc["can_warn"] = can_warn;
    doc["can_reward"] = can_reward;

    

    if(can_punish){
      temp = tod(punish_time,true);
      doc["punish_time"] = temp.to_json();
      doc["punishment"]=punish.to_json();

    }

    if(can_warn){
      temp = tod(warn_time,true);
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
  task_repeat::task_repeat(){

  }

  //json constructor
  task_repeat::task_repeat(JsonObject object){
    name = std::string(object["name"]);
    type = object["type"];

    JsonArray start_array = object["start"];
    tod temp;
    temp = tod(start_array);
    start = temp.to_time();

    JsonArray end_array = object["end"];
    temp = tod(end_array);
    end = temp.to_time();

    JsonArray interval_array = object["interval"];
    temp = tod(interval_array);
    interval = temp.to_time();

    can_punish = object["can_punish"];
    can_warn = object["can_warn"];
    can_reward = object["can_reward"];

    if(can_punish){
      JsonArray punish_array = object["punish_time"];
      temp = tod(punish_array);
      punish_time = temp.to_time();
      JsonObject task_punish = object["punishment"];
      punish = openshock::control(task_punish);
    }

    if(can_warn){
      JsonArray warn_array = object["warn_time"];
      temp = tod(warn_array);
      warn_time = temp.to_time();
      JsonObject task_warn = object["warning"];
      warning = openshock::control(task_warn);
    }

    if(can_reward) {
      reward_message = std::string(object["reward_message"]);
    } 
    gpio = object["gpio"];
    
    
    //calculate the start of the day
    time_t ctime = time(nullptr);
    time_t day_start = start_of_day();

    //calculate all the initial times
    start = day_start + start;
    end = day_start + end;
    interval = interval;
    time_t prev_end = end - 86400;
    time_t prev_start = start - 86400;

    if(start > end){ //the end time must be tomorrow, so add a day to the end time
      end += 86400;
      prev_end += 86400;
    }

    //check if we are outside the task time
    if(ctime <= start && ctime >= prev_end){
      next_time = start + interval;
      if(can_warn){
        next_warn = start + warn_time;
      }
      if(can_punish){
        next_punish = start + punish_time;
      }
      interval_active = true;

      //we are inside the task time
    }else{
      time_t temp_start;
      time_t temp_end;
      if(ctime < end && ctime > start){
        temp_start = start;
        temp_end = end;
      }

      if(ctime < prev_end && ctime > prev_start){
        temp_start = prev_start;
        temp_end = prev_end;
        start = prev_start;
        end = prev_end;
      }  
      next_time = temp_start;

      calc_next_time();
    }
    interval_active = true;
    active = true;
  }

  void task_repeat::check(){
    time_t ctime = time(nullptr);
    
    if(ctime == next_time){ //its time to recalculate next_time
      calc_next_time();
    }

    
    if(digitalRead(gpio) && interval_active){ //really dumb software debounce
      delay(50);
      if(digitalRead(gpio)){ //if still high, deactivate task
        interval_active = false;
        if(can_reward){
          oled.write_string_8_at(reward_message, 3, 0);
          oled.timed_clear(config.message_time*1000);
        }
      }
    }

    if(can_punish && next_punish == ctime && interval_active){ //if this iteration is still active and its time and failure is an option, zap!
      oled.write_string_8_at(punish.message, 3, 0);
      control_request(config.os_config, punish);
      interval_active = false; //assume that if there was a warning, it came before the punishment
      oled.timed_clear(config.message_time*1000);
    }else if(can_warn && next_warn == ctime && interval_active){ //if this iteration is still active and its time and this task gives a warning, zap!
      oled.write_string_8_at(warning.message, 3, 0);
      control_request(config.os_config, warning);
      if(!can_punish) interval_active = false; //if there is no punishment, then this task is done
      oled.timed_clear(config.message_time*1000);
    }
  }

  void task_repeat::disable(){
    active = false;
    interval_active = false;
    next_time = start + interval;
    if(can_warn){
      next_warn = start + warn_time;
    }
    if(can_punish){
      next_punish = start + punish_time;
    }
  }

  void task_repeat::enable(){
    active = true;
    interval_active = true;
  }
  
  void task_repeat::calc_next_time(){
      time_t ctime = time(nullptr);
      //loop thru and find the next interval start
      while(next_time <= ctime){
        next_time += interval;
      }
      if(next_time > end){
        next_time = end;
      }
      if(can_warn){
        next_warn = next_time - interval + warn_time;
      }
      if(can_punish){
        next_punish = next_time - interval + punish_time;
      }
      interval_active = true;

      if(ctime == end && ctime == next_time){ //set next_time to start + interval + 1 day
        start += 86400;
        end += 86400;
        next_time = start + interval;
        if(can_warn){
          next_warn = start + warn_time;
        }
        if(can_punish){
          next_punish = start + punish_time;
        }
        
        interval_active = true;
        active = false;
      }

  }

