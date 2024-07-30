#include "task_manager/task_repeating.h"
#include "config/config.h"
#include "debug/bp.h"

using namespace task_master;

  void task_repeat::print(){
    char buffer[256];
    sprintf(buffer, "task name: %s]\ntype: %d\ngpio: %d\nactive: %d\ncan_warn: %d\ncan_punish: %d\ncan_reward: %d", name.c_str(), type, gpio, active, can_warn, can_punish, can_reward);
    Serial.println(buffer);
    Serial.print("start: "); start.print();
    Serial.print("end: "); end.print();
    Serial.print("interval: "); interval.print();
    Serial.print("next_time: "); next_time.print();
    if(can_warn){
      Serial.print("next_warn: "); next_warn.print();
      warning.print();
    }
    if(can_punish){
      Serial.print("next_punish: "); next_punish.print();
      punish.print();
    }
    if(can_reward){
      Serial.print("reward_message :"); Serial.println(reward_message.c_str());
    }
  }

  //convert to JsonObject
  JsonDocument task_repeat::to_json(){
    JsonDocument doc;

    doc["name"] = name.c_str();
    doc["type"]  = type;
    doc["start"] = start.to_json();
    doc["end"] = end.to_json();
    doc["interval"] = interval.to_json();
    doc["can_punish"] = can_punish;
    doc["can_warn"] = can_warn;
    doc["can_reward"] = can_reward;

    if(can_punish){
      doc["punish_time"] = punish_time.to_json();
      doc["punishment"]=punish.to_json();
    }

    if(can_warn){
      doc["warn_time"] = warn_time.to_json();
      doc["warning"]=warning.to_json();
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
    start = tod(start_array);
    next_time = start;

    JsonArray end_array = object["end"];
    end = tod(end_array);

    JsonArray interval_array = object["interval"];
    interval = tod(interval_array);

    can_punish = object["can_punish"];
    can_warn = object["can_warn"];
    can_reward = object["can_reward"];


    calc_next_time(current_time);

    if(can_punish){
      JsonArray punish_array = object["punish_time"];
      punish_time = tod(punish_array);
      JsonObject task_punish = object["punishment"];
      punish = openshock::control(task_punish);
      next_punish = (next_time - interval) + punish_time;
    }

    if(can_warn){
      JsonArray warn_array = object["warn_time"];
      warn_time = tod(warn_array);
      JsonObject task_warn = object["warning"];
      warning = openshock::control(task_warn);
      next_warn =  (next_time - interval) + warn_time;
    }

    if(can_reward) {
      reward_message = std::string(object["reward_message"]);
    } 
    gpio = object["gpio"];
    
    
    interval_active = true;
    active = true;
  }

  //i realized im doing a lot of if(can_warn) and if(can_punish), seems inefficient. maybe there is a better way?
  //also this is extremely messy, im sure there is a way to do this in a much cleaner way.
  void task_repeat::check(){
    
    if(current_time == next_time && current_time != end){ //its time to recalculate next_time
      if(next_time + interval > end){ //the next interval is the last
        next_time = end;
        if(can_warn){
          next_warn = end + warn_time - interval;
        }
        if(can_punish){
          next_punish = end + punish_time - interval;
        }
        interval_active = true;
      }else{ //the next interval isn't the last
        if(can_warn){
          next_warn = next_time + warn_time;
        }
        if(can_punish){
          next_punish = next_time + punish_time;
        }
        next_time = next_time + interval;
        interval_active = true;
      }
    }
    
    if(current_time == end && current_time == next_time){ //set next_time to start interval
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
    
    if(digitalRead(gpio) && interval_active){ //really dumb software debounce
      delay(50);
      if(digitalRead(gpio)){ //if still high, deactivate task
        interval_active = false;
        if(can_reward){
          oled.write_string_8_at(reward_message, 3, 0);
          oled.timed_clear(conf.message_time*1000);
        }
      }
    }

    if(can_punish && next_punish == current_time && interval_active){ //if this iteration is still active and its time and failure is an option, zap!
      oled.write_string_8_at(punish.message, 3, 0);
      control_request(conf.os_config, punish);
      interval_active = false; //assume that if there was a warning, it came before the punishment
      oled.timed_clear(conf.message_time*1000);
    }else if(can_warn && next_warn == current_time && interval_active){ //if this iteration is still active and its time and this task gives a warning, zap!
      oled.write_string_8_at(warning.message, 3, 0);
      control_request(conf.os_config, warning);
      if(!can_punish) interval_active = false; //if there is no punishment, then this task is done
      oled.timed_clear(conf.message_time*1000);
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

  void task_repeat::calc_next_time(tod time){

    if(between_inclusive(time,start,end)){
      bp(1);
      while(next_time < time){
        next_time = next_time + interval;
        Serial.print("next_time = ");next_time.print();Serial.println();
        //delay(10);
      }
      bp(2);
      tod temp_end = end;
      tod temp_current = time;
      if(start > end){
        temp_end.hr += 24;
        if(time < end){
          temp_current.hr += 24;
        }
      }
      if(next_time + interval > temp_end){ //the next interval is the last
      bp(3);
        next_time = end;
      }
    }else{ //we are outside the running time of the task
    
     bp(4);
      next_time = start + interval;
    }
  }

