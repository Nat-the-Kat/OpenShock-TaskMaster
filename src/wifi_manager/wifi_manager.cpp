#include "wifi_manager/wifi_manager.h"
#include <LittleFS.h>


using namespace task_master;
  //check if wifi.json exists
  void wifi_manager::init(){
    wifi_configs.clear();
    retry_count = max_retry_count;
    File wifi_file = LittleFS.open("wifi.json", "r");
    if(!wifi_file){
      Serial.println("no wifi list detected! starting configuration network!");
      start_config_ap();
    } else {
      read_from_file();
      attempt_connection();
    }
  }

  void wifi_manager::reset(){
    WiFi.disconnect();
    init();

  }

  //write the network list in ram to wifi.json
  void wifi_manager::write_to_file(){
    File wifi_file = LittleFS.open("wifi.json", "w");
    std::string temp = write_to_string();
    wifi_file.write(temp.c_str(),temp.size());
    wifi_file.close();
  }

  void wifi_manager::read_from_file(){
    wifi_configs.clear();
    File wifi_file = LittleFS.open("wifi.json", "r");
    read_from_stream(wifi_file);
    wifi_file.close();
  }

  void wifi_manager::add_network(JsonObject object){
    wifi_config temp(object);
    wifi_configs.push_back(temp);
  }

  void wifi_manager::read_from_serial(){
    Serial.println("waiting for json string...");
    while(!Serial.available());
    if(read_from_stream(Serial)){
      Serial.println("network(s) added!");
      write_to_file();
    }
    
  }
    

  void wifi_manager::print_networks(){
    for(wifi_config i: wifi_configs){
      i.print();
    }
  }

  bool wifi_manager::delete_network(const char* ssid){
    for(auto i = wifi_configs.begin(); i != wifi_configs.end(); i++){
      if(strcmp(ssid, (*i).ssid.c_str()) == 0){
        wifi_configs.erase(i);
        Serial.println("network deleted!");
        return true;
      }
    }
    Serial.println("error! network not found in list!");
    return false;
  }

  bool wifi_manager::attempt_connection(){
    WiFi.disconnect();
    while(retry_count <= 0){
      int count = WiFi.scanNetworks();
      if(!count){
        Serial.println("\nerror! no wifi networks detected!");
        retry_count--;
      }else{
        for(int i = 0; i < count; i++){
          for(wifi_config current: wifi_configs){
            if(strcmp(current.ssid.c_str(), WiFi.SSID(i)) == 0){
              char buffer[64];
              sprintf(buffer, "found network %s", current.ssid.c_str());
              Serial.println(buffer);
              WiFi.begin(current.ssid.c_str(), current.password.c_str());
              Serial.print("Connected! IP address: ");
              Serial.println(WiFi.localIP());
              retry_count = max_retry_count; //reset retry count
              return true;
            }
          }
        }
      }
      Serial.println("no networks found! retrying...");
      retry_count--;
    }
    //if we make it here, consider trying to find a known network a failure
    return false;
  }

  bool wifi_manager::read_from_stream(Stream &s){
    JsonDocument doc;
    DeserializationError error = deserializeJson(doc, s);
    if(error){
      Serial.print("deserializeJson() failed: ");
      Serial.println(error.c_str());
      return false;
    }else{
      for(JsonObject wifi_json : doc["networks"].as<JsonArray>()) {
        wifi_config w(wifi_json);
        wifi_configs.push_back(w);
      }
    }

    return true;
  }

  std::string wifi_manager::write_to_string(){
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

  bool wifi_manager::start_config_ap(){
    if(!WiFi.beginAP("TaskMaster")){
      return false;
    }else{
      return true;
    }
    
  }


  //this is way more complicated than it needs to be...
  uint8_t wifi_manager::is_connected(){
    if(current_state != ap_active){
      if(WiFi.status() == WL_CONNECTED){
        return wifi_connected;
      }else{
        if(!attempt_connection()){
          if(!start_config_ap()){
            return disconnected;
          }else{
            return ap_active;
          }  
        }
      }
    }else{
      if(WiFi.status() == WL_CONNECTED){
        return ap_active;
      }else{
        return disconnected;
      }
    }
    return wifi_error;
  }

  wifi_manager w_manager;