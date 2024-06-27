#include "task_manager/control.h"

using namespace task_master;
  void control::print(){
    char buffer[128];
    sprintf(buffer, "strength: %u\ntype: %s\nduration: %u\nmessage: %s", strength, type.c_str(), dur, message.c_str());
    Serial.println(buffer);
    return;
  }

  JsonDocument control::to_json(){
    JsonDocument doc;
    doc["strength"] = strength;
    doc["type"] = type.c_str();
    doc["duration"] = dur;
    doc["message"] = message.c_str();
    return doc;

  }
  //empty constructor
  control::control(){

  }
  //json constructor
  control::control(JsonObject object){
    type = std::string(object["type"]);
    strength = object["strength"];
    dur = object["duration"];
    message = std::string(object["message"]);
  }
