#include <LittleFS.h>
#include <pico/stdlib.h>
#include <pico/util/datetime.h>
#include <openshock.h>
#include "task_manager/task_manager.h"
#include "task_manager/task_repeating.h"
#include "task_manager/task_timed.h"
#include "config/config.h"

using namespace task_master;
 
  void task_manager_class::init(){
    task_list.erase(task_list.begin(),task_list.end());
    File tasks_file = LittleFS.open("tasks.json", "r");
    if(!tasks_file){
      Serial.println("no task list detected!");
    }
    read_from_file();
  }

  void task_manager_class::print(){
    for(int i = 0; i < task_list.size(); i++){
      task_list[i]->print();
    }
  }

  void task_manager_class::check_tasks(){
    //check if any task needs updating
    for(task* t: task_list){
      if(t->active){
        t->check();
      }
    }
    //check if its reset time
    if(current_time == config.reset_time){
      //update day of week
      tm temp;
      time_t now = time(nullptr); 
      localtime_r(&now, &temp); //this seems wasteful...
      config.dow = temp.tm_wday;

      //its reset_day, reset overrides (if enabled)
      if(config.dow == config.reset_day && config.can_override){

        config.overrides_left = config.num_overrides;
      }
      //reactivate all tasks
      for(task* t: task_list){
        t->enable();
      }
    }
    if(config.can_override){
      if(digitalRead(config.override_pin) && config.overrides_left != 0){
        delay(500);  //really dumb software debounce
        if(digitalRead(config.override_pin)){ //if still high, deactivate task
          oled.write_string_8_at("override used!",3,0);
          oled.timed_clear(config.message_time*1000);
          config.overrides_left--;
          //actually deactivate all tasks...
          for(task* t: task_list){
            t->disable();
          }
        }
      }
    }
  }

  std::string task_manager_class::write_to_string(){
    std::string out;
    JsonDocument doc;
    JsonArray tasks = doc["tasks"].to<JsonArray>();

    for(task* current_task: task_list){
      doc["tasks"].add(current_task->to_json());
    }

    doc.shrinkToFit();
    serializeJson(doc, out);
    return out;
  }

  void task_manager_class::read_from_file(){
    task_list.erase(task_list.begin(),task_list.end());
    File task_file = LittleFS.open("tasks.json","r");
    JsonDocument doc;
    DeserializationError error = deserializeJson(doc, task_file);
    if(error){
      Serial.print("deserializeJson() failed: ");
      Serial.println(error.c_str());
      return;
    }else{
      for (JsonObject task_json : doc["tasks"].as<JsonArray>()) {
        add_task(task_json);
      }
    } 
    task_file.close();
  }

  void task_manager_class::add_task(JsonObject object){
    int type = object["type"];
    task* t;
    if(type == 1){
      t = new task(object);
    }else if(type == 2){
      t = new task_timed(object);
    }else if(type == 3){
      t = new task_repeat(object);
    }else{
      Serial.print("unknown task type: "); Serial.println(type);
      Serial.println("defaulting to type 1");
      t = new task(object);
    }
    task_list.push_back(t);
  }

  task_manager_class task_manager;
