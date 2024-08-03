#include "config.h"

using namespace openshock;

config::config(){

}

config::config(JsonObject object){
  server = std::string(object["server"]);
  api_key = std::string(object["api_key"]);
  shocker = std::string(object["shocker"]);
}

JsonDocument config::to_json(){
  JsonDocument doc;
  doc["server"] = server.c_str();
  doc["api_key"] = api_key.c_str();
  doc["shocker"] = shocker.c_str();
  return doc;
}

void config::print(){
  char buffer[256];
  sprintf(buffer,"OpenShock instance: %s\nOpenShock API key: %s\nshocker id: %s",server.c_str(),api_key.c_str(),shocker.c_str());
  Serial.println(buffer);
}