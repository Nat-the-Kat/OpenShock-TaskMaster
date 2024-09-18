#include "wifi_manager/wifi_manager.h"
#include <LittleFS.h>
#include <WiFi.h>
#include "debug/bp.h"

WiFiMulti wifi;

using namespace task_master;
  //check if wifi.json exists
  void wifi_manager_class::init(){
    WiFi.disconnect();
    wifi_configs.clear();
    retry_count = max_retry_count;
    if(!LittleFS.exists("wifi.json")){
      Serial.println("no wifi list detected! starting configuration network!");
      start_config_ap();
    } else {
      read_from_file();
      attempt_connection();
    }
  }

  void wifi_manager_class::print(){
    for(wifi_config i: wifi_configs){
      i.print();
    }
  }

  std::string wifi_manager_class::write_to_string(){
    std::string out;
    JsonDocument doc;
    JsonArray doc_wifi = doc["networks"].to<JsonArray>();
    for(wifi_config i: wifi_configs){
      doc_wifi.add(i.to_json());
    }
    doc.shrinkToFit();
    serializeJson(doc, out);
    return out;
  }


  //clean up me!
  void wifi_manager_class::check_connection(){
    if(WiFi.status() == WL_CONNECTED){
      return;
    }else{
      if(!attempt_connection()){
        start_config_ap();
      }  
    }
  }

  //clean up me!
  bool wifi_manager_class::attempt_connection(){
    retry_count = max_retry_count;
    wifi.clearAPList();
    for(wifi_config current: wifi_configs){
      wifi.addAP(current.ssid.c_str(),current.password.c_str());
    }
    while(retry_count > 0){
      if(wifi.run()==WL_CONNECTED){
        return true;
      }
      Serial.println("no networks found! retrying...");
      delay(250);
      bp(retry_count);
      retry_count--;
    }
    //if we make it here, consider trying to find a known network a failure
    return false;
  }

  void wifi_manager_class::add_network(JsonObject object){
    wifi_config temp(object);
    wifi_configs.push_back(temp);
  }   


  bool wifi_manager_class::start_config_ap(){
    if(!WiFi.beginAP("TaskMaster")){
      return false;
    }else{
      return true;
    }
  }

  void wifi_manager_class::read_from_file(){
    wifi_configs.clear();
    File wifi_file = LittleFS.open("wifi.json", "r");
    JsonDocument doc;
    DeserializationError error = deserializeJson(doc, wifi_file);
    if(error){
      Serial.print("deserializeJson() failed: ");
      Serial.println(error.c_str());
      return;
    }else{
      for(JsonObject wifi_json : doc["networks"].as<JsonArray>()) {
        wifi_config w(wifi_json);
        wifi_configs.push_back(w);
      }
    }
    wifi_file.close();
  }

  wifi_manager_class wifi_manager;