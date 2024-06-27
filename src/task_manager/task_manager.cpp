#include "task_manager/task_manager.h"
#include "debug/bp.h"
using namespace task_master;

  //create a new tasks.json, with a dummy task in it
  void task_manager::create_tasks_json(){
    clear_tasks();
    task* t = new task;
    task_list.push_back(t);
    write_to_file();
  }
    
  void task_manager::init(){
    File tasks_file = LittleFS.open("tasks.json", "r");
    if(!tasks_file){
      Serial.println("no task list detected! creating new tasks.json");
      create_tasks_json();
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
    JsonDocument doc;

    DeserializationError error = deserializeJson(doc, task_file);
    task_file.close();
    if(error){
      Serial.print("deserializeJson() failed: ");
      Serial.println(error.c_str());
      return;
    }

    for(JsonObject task_json : doc["tasks"].as<JsonArray>()){
      task* t = new task(task_json);
      task_list.push_back(t);
    }
    doc.clear();
  }

  void task_manager::write_to_file(){

    File tasks_file = LittleFS.open("tasks.json","w");

    JsonDocument doc;
    JsonArray tasks = doc["tasks"].to<JsonArray>();


    for(task* current_task: task_list){
      doc["tasks"].add(current_task->to_json());
    }

    doc.shrinkToFit();
    serializeJson(doc, tasks_file);
    tasks_file.close();
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
    JsonDocument doc;
    DeserializationError error = deserializeJson(doc, Serial);
    if(error){
      Serial.print("deserializeJson() failed: ");
      Serial.println(error.c_str());
      return;
    }else{
      for (JsonObject task_json : doc["tasks"].as<JsonArray>()) {
        add_task(task_json);
      }
      Serial.println("task(s) added!");
    }    
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



  void task_manager::check_tasks(config* conf){

    //check if any task needs updating
    for(task* t: task_list){
      if(t->active){
        t->check(conf);
      }
    }
    //check if its reset time
    if(current_time == conf->reset_time){
      //update day of week
      tm temp;
      time_t now = time(nullptr); 
      localtime_r(&now, &temp); //this seems wasteful...
      conf->dow = temp.tm_wday;

      //its reset_day, reset overrides (if enabled)
      if(conf->dow == conf->reset_day && conf->can_override){

        conf->overrides_left = conf->num_overrides;
      }
      //reactivate all tasks
      for(task* t: task_list){
        t->active = true;
      }
    }
    if(conf->can_override){
      if(digitalRead(conf->override_pin) && conf->overrides_left != 0){
        delay(500);  //really dumb software debounce
        if(digitalRead(conf->override_pin)){ //if still high, deactivate task
          //make sure that if we need to display text, the correct font is loaded, because if forgot the frame callback can happen in the middle of this and change the font
          oled.load_font(font8);
          oled.cursor_pos(3,0);
          oled.write_string_8("override used!");
          oled.timed_clear(conf->message_time*1000);
          conf->overrides_left--;

        }
      }
    }
  }
