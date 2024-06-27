#include "config/config.h"

  using namespace task_master;
  
  //create a basic config file pretty sure i could just call write_to_file.....
  void config::create_config_json(){
    ntp_server = "pool.ntp.org";
    api_key = "api_key";
    shocker = "shocker";
    can_override = true;
    override_pin = 1;
    num_overrides = 1;
    message_time = 5;
    write_to_file();
  }

  //check if config.json exists, if not, create a default config
  void config::init(){
    File config_file = LittleFS.open("config.json", "r");

    if(!config_file){
      Serial.println("no config detected! creating new config.json");
      create_config_json();
    }else{
      config_file.close();
    }
    read_from_file();
    tm temp;
    time_t now = time(nullptr); 
    localtime_r(&now, &temp); 
    dow = temp.tm_wday;
  }

  //write the config in ram to config.json
  void config::write_to_file(){
    File config_file = LittleFS.open("config.json","w");

    JsonDocument doc;
    JsonObject config = doc["config"].to<JsonObject>();
    config["ntp_server"] = ntp_server;
    config["api_key"] = api_key;
    config["shocker"]  = shocker;
    config["can_override"] = can_override;

    if(can_override){
      config["override_pin"] = override_pin;
      config["num_overrides"] = num_overrides;
      config["reset_day"] = reset_day;
    }
    
    config["message_time"] = message_time;
    config["reset_time"] = reset_time.to_json();
    config["timezone"] = timezone.to_json();

    doc.shrinkToFit();
    serializeJson(doc, config_file);
    config_file.close();
  }

  void config::read_from_file(){
    JsonDocument config_doc;

    File config_file = LittleFS.open("config.json", "r");
    DeserializationError error = deserializeJson(config_doc, config_file);
    config_file.close();
    if(error){
      Serial.print("deserializeJson() failed: ");
      Serial.println(error.c_str());
      Serial.println("config not updated!");
      return;
    }

    JsonObject config = config_doc["config"];
    ntp_server = std::string(config["ntp_server"]);
    api_key = std::string(config["api_key"]);
    shocker = std::string(config["shocker"]);
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
    
    JsonArray config_timezone = config["timezone"];
    timezone = tod(config_timezone);
  }

  void config::print(){
    char buffer[256];
    sprintf(buffer, "ntp_server: %s\napi_key: %s\nshocker: %s", ntp_server.c_str(), api_key.c_str(), shocker.c_str());
    Serial.println(buffer);
    Serial.print("can_override: ");Serial.println(can_override);
    if(can_override){
      char b[128];
      sprintf(b,"override_pin: %d\nnum_overrides: %d\noverrides_left: %d\nreset_day: %d",override_pin,num_overrides,overrides_left,reset_day);
      Serial.println(b);
    }
    Serial.print("message_time: ");Serial.println(message_time);
    Serial.print("dow: ");Serial.println(dow);
    Serial.print("reset_time: ");reset_time.print();
    Serial.print("timezone: ");timezone.print();
  }


  void config::edit_config(){
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
      JsonObject config = doc["config"];
      ntp_server = std::string(config["ntp_server"]);
      api_key = std::string(config["api_key"]);
      shocker = std::string(config["shocker"]);
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
      JsonArray config_timezone = config["timezone"];
      timezone = tod(config_timezone);
      Serial.println("config updated!");
    }
  }
  

