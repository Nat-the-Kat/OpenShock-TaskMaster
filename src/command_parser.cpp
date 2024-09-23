#include <LittleFS.h>
#include <oled.h>
#include <pico/stdlib.h>
#include <tod.h>
#include <WiFi.h>

#include "command_parser.h"
#include "config/config.h"
#include "helpers/filesystem.h"
#include "helpers/stream_to_string.h"
#include "task_manager/task_manager.h"
#include "task_manager/task_repeating.h"
#include "wifi_manager/wifi_manager.h"


//#include <wifiz.h> 

#define version_string "v0.2.0"

//very simple serial command parser to allow modifying the configuration

//this file is an unholy mess, it desperately needs cleanup. there is almost certainly a way better way to do this.
  void task_master::parse_serial(){
    std::string input = stream_to_string(Serial);
    Serial.print(">"); Serial.println(input.c_str());

    //compare against valid commands
    if(strncmp(input.c_str(), "read ", 5) == 0){
      input.erase(input.begin(),input.begin() + 5);
      read(input);

    }else if(strncmp(input.c_str(), "delete ", 7) == 0){
      input.erase(input.begin(),input.begin() + 7);
      delete_file(input);

    }else if(strncmp(input.c_str(), "fsinfo", 6) == 0){
      FSInfo fsinfo;
      LittleFS.info(fsinfo);
      char buffer[128];
      sprintf(buffer, "fs info:\ntotalBytes: %ld\nusedBytes: %ld\nblockSize: %d\npageSize: %d\nmaxOpenFiles: %d\nmaxPathLength: %d", fsinfo.totalBytes, fsinfo.usedBytes, fsinfo.blockSize, fsinfo.pageSize, fsinfo.maxOpenFiles, fsinfo.maxPathLength);
      Serial.println(buffer);

    }else if(strncmp(input.c_str(), "help", 4) == 0){
      help();

    }else if(strncmp(input.c_str(), "reboot", 6) == 0){
      rp2040.reboot();

    }else if(strncmp(input.c_str(), "heap", 4) == 0){
      Serial.print("theoretical free heap:"); Serial.println(rp2040.getFreeHeap());
      Serial.print("total heap:"); Serial.println(rp2040.getTotalHeap());
      Serial.print("used heap:"); Serial.println(rp2040.getUsedHeap());

    }else if(strncmp(input.c_str(), "ver", 3) == 0){
      Serial.println(version_string);

    }else if(strncmp(input.c_str(), "test", 4) == 0){

      
    }else if(strncmp(input.c_str(), "view_tasks", 10) == 0){
      task_manager.print();

    }else if(strncmp(input.c_str(), "view_config", 11) == 0){
      config.print();

    }else if(strncmp(input.c_str(), "view_networks", 13) == 0){
      wifi_manager.print();

    }else if(strncmp(input.c_str(), "factory_reset", 13) == 0){
      delete_file("config.json");
      delete_file("tasks.json");
      delete_file("wifi.json");
      config.init();
      task_manager.init();
      wifi_manager.init();

    }else if(strncmp(input.c_str(), "ip_address", 10) == 0){
      if(WiFi.status() != WL_CONNECTED){
        Serial.println("wifi is not connected, check the network config!");
      }else{ //we have wifi
        Serial.print("local ip address: ");Serial.println(WiFi.localIP());
      }
    }else{
      Serial.println("Unknown command, try help to see valid commands.");
    }
  }
  void task_master::help(){ 
      Serial.println("read filename           output contents of specified file to the serial terminal.");
      Serial.println("delete filename         deletes specified file.");
      Serial.println("fsinfo                  outputs some info about LittleFS, including used space");
      Serial.println("view_tasks              prints out the current task list stored in ram.");
      Serial.println("view_config             prints out the current configuration stored in ram.");
      Serial.println("view_networks           prints out the current network list stored in ram.");
      Serial.println("ip_address              prints local ip address (if wifi is connected).");
      Serial.println("factory_reset           delete config.json, tasks.json and wifi.json, and create new ones with default values");
      Serial.println("help                    you just used it, you silly bean :P");
      Serial.println("reboot                  reboot the mcu");
      Serial.println("ver                     returns firmware version.");   
  }