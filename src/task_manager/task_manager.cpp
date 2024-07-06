#include <LittleFS.h>
#include <pico/stdlib.h>
#include <pico/util/datetime.h>
#include <openshock.h>
#include "task_manager/task_manager.h"
#include "task_manager/task_repeating.h"
#include "task_manager/task_timed.h"
#include "config/config.h"

using namespace task_master;
 
  void task_manager::init(){
    File tasks_file = LittleFS.open("tasks.json", "r");
    if(!tasks_file){
      Serial.println("no task list detected! please create one with add_tasks!");
    }
    read_from_file();
  }

  void task_manager::print(){
    for(int i = 0; i < task_list.size(); i++){
      task_list[i]->print();
    }
  }

  void task_manager::clear_tasks(){
    task_list.erase(task_list.begin(),task_list.end());
  }

  void task_manager::read_from_file(){
    clear_tasks();
    File task_file = LittleFS.open("tasks.json","r");
    read_from_stream(task_file);
    task_file.close();
  }

  void task_manager::write_to_file(){
    File task_file = LittleFS.open("tasks.json","w");
    std::string temp = write_to_string();
    task_file.write(temp.c_str(),temp.size());
    task_file.close();
  }

  void task_manager::delete_task(const char* task_name){
    bool found = false;
    for(int i = 0; i < task_list.size(); i++){
      if(strcmp(task_name, task_list[i]->name.c_str())==0){
        task_list.erase(task_list.begin()+i);
        found = true;
      }
    }
    if(!found){
      Serial.println("error! task not found!");
    }
  }

  void task_manager::add_tasks(){
    Serial.println();
    Serial.println("waiting for json string...");
    while(!Serial.available());
    read_from_stream(Serial);
    Serial.println("task(s) added!");   
  }

  void task_manager::add_task(JsonObject object){
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

  void task_manager::check_tasks(){
    //check if any task needs updating
    for(task* t: task_list){
      if(t->active){
        t->check();
      }
    }
    //check if its reset time
    if(current_time == conf.reset_time){
      //update day of week
      tm temp;
      time_t now = time(nullptr); 
      localtime_r(&now, &temp); //this seems wasteful...
      conf.dow = temp.tm_wday;

      //its reset_day, reset overrides (if enabled)
      if(conf.dow == conf.reset_day && conf.can_override){

        conf.overrides_left = conf.num_overrides;
      }
      //reactivate all tasks
      for(task* t: task_list){
        t->active = true;
      }
    }
    if(conf.can_override){
      if(digitalRead(conf.override_pin) && conf.overrides_left != 0){
        delay(500);  //really dumb software debounce
        if(digitalRead(conf.override_pin)){ //if still high, deactivate task
          //make sure that if we need to display text, the correct font is loaded, because if forgot the frame callback can happen in the middle of this and change the font
          oled.load_font(font8);
          oled.cursor_pos(3,0);
          oled.write_string_8("override used!");
          oled.timed_clear(conf.message_time*1000);
          conf.overrides_left--;

        }
      }
    }
  }

  void task_manager::read_from_stream(Stream &s){
    JsonDocument doc;
    DeserializationError error = deserializeJson(doc, s);
    if(error){
      Serial.print("deserializeJson() failed: ");
      Serial.println(error.c_str());
      return;
    }else{
      for (JsonObject task_json : doc["tasks"].as<JsonArray>()) {
        add_task(task_json);
      }
    } 
  }

  std::string task_manager::write_to_string(){
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

  task_manager manager;
