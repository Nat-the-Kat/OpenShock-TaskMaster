#include "config/config.h"
#include <LittleFS.h>

  using namespace task_master;

  //check if config.json exists, if not, create a default config
  void config_class::init(){
    File config_file = LittleFS.open("config.json", "r");

    if(!config_file){
      Serial.println("no config detected!");
    }else{
      config_file.close();
    }
    read_from_file();
    //why this works when the first time i tried the exact same code it failed will forever remain a mystery to me.
    setenv("TZ",timezone_rule.c_str(),1);
    tzset();

    tm temp;
    time_t now = time(nullptr); 
    localtime_r(&now, &temp); 
    dow = temp.tm_wday;
  }

  void config_class::print(){
    char buffer[64];
    sprintf(buffer, "ntp_server: %s", ntp_server.c_str());
    Serial.println(buffer);
    Serial.println("os_config:");
    os_config.print();
    Serial.print("can_override: ");Serial.println(can_override);
    if(can_override){
      char b[128];
      sprintf(b,"override_pin: %d\nnum_overrides: %d\noverrides_left: %d\nreset_day: %d",override_pin,num_overrides,overrides_left,reset_day);
      Serial.println(b);
    }
    Serial.print("message_time: ");Serial.println(message_time);
    Serial.print("dow: ");Serial.println(dow);
    Serial.print("reset_time: ");reset_time.print();
    Serial.print("timezone_name: ");Serial.println(timezone_name.c_str());
    Serial.print("timezone_rule: ");Serial.println(timezone_rule.c_str());
  }

  std::string config_class::write_to_string(){
    std::string out;
    JsonDocument doc;
    JsonObject config = doc["config"].to<JsonObject>();
    config["ntp_server"] = ntp_server;
    config["os_config"] = os_config.to_json();
    config["can_override"] = can_override;

    if(can_override){
      config["override_pin"] = override_pin;
      config["num_overrides"] = num_overrides;
      config["reset_day"] = reset_day;
    }
    
    config["message_time"] = message_time;
    config["reset_time"] = reset_time.to_json();
    config["timezone_name"] = timezone_name.c_str();
    config["timezone_rule"] = timezone_rule.c_str();


    doc.shrinkToFit();
    serializeJson(doc, out);
    return out;
  }

  void config_class::read_from_file(){
    File config_file = LittleFS.open("config.json", "r");
        JsonDocument doc;
    DeserializationError error = deserializeJson(doc, config_file);
    if(error){
      Serial.print("deserializeJson() failed: ");
      Serial.println(error.c_str());
      return;
    }else{
      JsonObject config = doc["config"];
      ntp_server = std::string(config["ntp_server"]);
      JsonObject osconfig = config["os_config"];
      os_config = openshock::config(osconfig);
      can_override = config["can_override"];
      if(can_override){
        override_pin = config["override_pin"];
        num_overrides = config["num_overrides"];
        reset_day = config["reset_day"];
      }

      message_time = config["message_time"];
      overrides_left = num_overrides;

      JsonArray config_reset_time = config["reset_time"];
      reset_time = tod(config_reset_time);
      timezone_name = std::string(config["timezone_name"]);
      timezone_rule = std::string(config["timezone_rule"]);
    }
    config_file.close();
  }

  config_class config;
