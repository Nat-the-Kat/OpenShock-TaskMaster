#include "wifi_manager/wifi_config.h"

using namespace task_master;
  void wifi_config::print(){
    Serial.print("wifi ssid: "); Serial.println(ssid.c_str());
    Serial.print("wifi password: "); Serial.println(password.c_str());
  }

  //convert config to json
  JsonDocument wifi_config::to_json(){
    JsonDocument doc;
    doc["ssid"] = ssid;
    doc["password"] = password;
    return doc;
  }

  //empty constructor
  wifi_config::wifi_config(){

  }

  //json constructor
  wifi_config::wifi_config(JsonObject object){
    ssid = std::string(object["ssid"]);
    password = std::string(object["password"]);

  }

  //text string constructor
  wifi_config::wifi_config(const char* s, const char* p){
    ssid = std::string(s);
    password = std::string(p);
  }
