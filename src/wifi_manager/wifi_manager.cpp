  
  #include "wifi_manager/wifi_manager.h"
  #include "debug/bp.h"
using namespace task_master;
  //check if wifi.json exists, if not, create a bare bones config
  void wifi_manager::init(){
    clear_networks();
    File wifi_file = LittleFS.open("wifi.json", "r");
    if(!wifi_file){
      Serial.println("no wifi list detected! creating new wifi.json");
      create_wifi_json();
    }
    read_from_file();
  }

  //write the network list in ram to wifi.json
  void wifi_manager::write_to_file(){
    File wifi_file = LittleFS.open("wifi.json", "w");

    JsonDocument doc;

    JsonArray doc_wifi = doc["networks"].to<JsonArray>();

    for(wifi_config i: wifi_configs){
      doc_wifi.add(i.to_json());
    }

    doc.shrinkToFit();
    serializeJson(doc, wifi_file);
    wifi_file.close();
  }

  void wifi_manager::read_from_file(){
    wifi_configs.clear();
    JsonDocument wifi_doc;

    File wifi_file = LittleFS.open("/wifi.json", "r");
    DeserializationError error = deserializeJson(wifi_doc, wifi_file);
 
    wifi_file.close();
    if(error){
      Serial.print("deserializeJson() failed: ");
      Serial.println(error.c_str());
      Serial.println("wifi list not updated!");
      return;
    }
    for(JsonObject wifi_item : wifi_doc["networks"].as<JsonArray>()){
      add_network(wifi_item);
    }
  }

  void wifi_manager::create_wifi_json(){
    add_network("ssid", "password");
    //write these default values to wifi.json
    write_to_file();

  }

  void wifi_manager::clear_networks(){
    wifi_configs.clear();
  }

  void wifi_manager::add_network(const char* ssid, const char* password){
    wifi_config temp(ssid, password);
    wifi_configs.push_back(temp);
  }

  void wifi_manager::add_network(JsonObject object){
    wifi_config temp(object);
    wifi_configs.push_back(temp);
  }

  void wifi_manager::add_networks(){
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
      for(JsonObject wifi_json : doc["networks"].as<JsonArray>()) {
        wifi_config w(wifi_json);
        wifi_configs.push_back(w);
      }
      Serial.println("network(s) added!");
    }
  }

  void wifi_manager::print_networks(){
    for(wifi_config i: wifi_configs){
      i.print();
    }
  }

  void wifi_manager::delete_network(const char* ssid){
    bool found = false;
    for(auto i = wifi_configs.begin(); i != wifi_configs.end(); i++){
      if(strcmp(ssid, (*i).ssid.c_str()) == 0){
        wifi_configs.erase(i);
        found = true;
      }
    }
    if(!found){
      Serial.println("error! network not found in list!");
    }
  }

  bool wifi_manager::search_for_network(){
    int count = WiFi.scanNetworks();
    if(!count){
      Serial.println("\nerror! no wifi networks detected!");
      return false;
    }else{
      for(int i = 0; i < count; i++){
        for(wifi_config current: wifi_configs){
          if(strcmp(current.ssid.c_str(), WiFi.SSID(i)) == 0){
            char buffer[64];
            sprintf(buffer, "found network %s", current.ssid.c_str());
            Serial.println(buffer);
            WiFi.begin(current.ssid.c_str(), current.password.c_str());
            return true;
          }
        }
      }
    }
    Serial.println("no network found!");
    return false;
  }