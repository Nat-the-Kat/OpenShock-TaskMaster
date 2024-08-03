#include "control.h"

using namespace openshock;
  void control::print(){
    char buffer[128];
    sprintf(buffer, "intensity: %u\ntype: %s\nduration: %u\nmessage: %s", intensity, type.c_str(), duration, message.c_str());
    Serial.println(buffer);
    return;
  }

  JsonDocument control::to_json(){
    JsonDocument doc;
    doc["intensity"] = intensity;
    doc["type"] = type.c_str();
    doc["duration"] = duration;
    doc["message"] = message.c_str();
    return doc;

  }
  //empty constructor
  control::control(){

  }
  //json constructor
  control::control(JsonObject object){
    type = std::string(object["type"]);
    intensity = object["intensity"];
    duration = object["duration"];
    message = std::string(object["message"]);
  }
