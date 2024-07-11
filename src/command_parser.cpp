#include "command_parser.h"
#include "wifi_manager/wifi_manager.h"
#include "task_manager/task_manager.h"
#include "config/config.h"
#include "helpers/filesystem.h"
#include <pico/stdlib.h>
#include "helpers/stream_to_string.h"
#include <tod.h>
#include <LittleFS.h>
#include <oled.h>


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
      FSInfo64 fsinfo;
      LittleFS.info64(fsinfo);
      char buffer[128];
      sprintf(buffer, "fs info:\ntotalBytes: %ld\nusedBytes: %ld\nblockSize: %d\npageSize: %d\nmaxOpenFiles: %d\nmaxPathLength: %d", fsinfo.totalBytes, fsinfo.usedBytes, fsinfo.blockSize, fsinfo.pageSize, fsinfo.maxOpenFiles, fsinfo.maxPathLength);
      Serial.println(buffer);

    }else if(strncmp(input.c_str(), "help", 4) == 0){
      input.erase(input.begin(),input.begin() + 4);
      help(input);

    }else if(strncmp(input.c_str(), "reboot", 6) == 0){
      rp2040.reboot();

    }else if(strncmp(input.c_str(), "heap", 4) == 0){
      Serial.print("theoretical free heap:"); Serial.println(rp2040.getFreeHeap());
      Serial.print("total heap:"); Serial.println(rp2040.getTotalHeap());
      Serial.print("used heap:"); Serial.println(rp2040.getUsedHeap());

    }else if(strncmp(input.c_str(), "ver", 3) == 0){
      Serial.println("v0.0.0");

    }else if(strncmp(input.c_str(), "test", 4) == 0){


    }else if(strncmp(input.c_str(), "write_tasks", 11) == 0){
      manager.write_to_file();

    }else if(strncmp(input.c_str(), "read_tasks", 10) == 0){
      manager.read_from_file();

    }else if(strncmp(input.c_str(), "view_tasks", 10) == 0){
      manager.print();

    }else if(strncmp(input.c_str(), "clear_tasks", 11) == 0){
      manager.clear_tasks();

    }else if(strncmp(input.c_str(), "delete_task ", 12) == 0){
      input.erase(input.begin(),input.begin() + 12);
      manager.delete_task(input.c_str());

    }else if(strncmp(input.c_str(), "add_tasks", 9) == 0){
      manager.add_tasks();

    }else if(strncmp(input.c_str(), "write_config", 12) == 0){
      conf.write_to_file();

    }else if(strncmp(input.c_str(), "read_config", 11) == 0){
      conf.read_from_file();

    }else if(strncmp(input.c_str(), "view_config", 11) == 0){
      conf.print();

    }else if(strncmp(input.c_str(), "edit_config", 11) == 0){
      conf.edit_config();

    }else if(strncmp(input.c_str(), "write_networks", 14) == 0){
      //w_manager.write_to_file();

    }else if(strncmp(input.c_str(), "read_networks", 13) == 0){
      //w_manager.read_from_file();

    }else if(strncmp(input.c_str(), "view_networks", 13) == 0){
      w_manager.print_networks();

    }else if(strncmp(input.c_str(), "add_networks", 12) == 0){
      w_manager.read_from_serial();

    }else if(strncmp(input.c_str(), "clear_networks", 14) == 0){
      //w_manager.clear_networks();

    }else if(strncmp(input.c_str(), "delete_network ", 15) == 0){
      input.erase(input.begin(),input.begin() + 15);
      w_manager.delete_network(input.c_str());

    }else if(strncmp(input.c_str(), "factory_reset", 13) == 0){
      delete_file("config.json");
      delete_file("tasks.json");
      delete_file("wifi.json");
      conf.init();
      manager.init();
      w_manager.init();

    }else if(strncmp(input.c_str(), "ip_address", 10) == 0){
      if(WiFi.status() != WL_CONNECTED){
        Serial.println("wifi is not connected, check the network config!");
      }else{ //we have wifi
        Serial.print("local ip address: ");Serial.println(WiFi.localIP());
      }

    }else{
      Serial.println("Unknown command, try help to see valid commands.");
      oled.clear();
    }
  }

  //this is a kinda gross way to do this, but it works soooo.....
  void task_master::help(std::string s){ 
    if(strncmp(s.c_str(), " file", 5) == 0){
      Serial.println("file commands:");
      Serial.println("read filename           output contents of specified file to the serial terminal.");
      Serial.println("delete filename         deletes specified file.");
      Serial.println("fsinfo                  outputs some info about LittleFS, including used space");
    }else if(strncmp(s.c_str(), " tasks", 6) == 0){
      Serial.println("task editing commands:");
      Serial.println("write_tasks             write task list in ram to tasks.json.");
      Serial.println("read_tasks              reads tasks.json into ram.");
      Serial.println("view_tasks              prints out the current task list stored in ram.");
      Serial.println("delete_task name        remove task specified from task list in ram.");
      Serial.println("clear_tasks             remove all tasks from task list in ram.");
      Serial.println("add_tasks               add a new task (or tasks) to the list in ram, using json to define the task(s)");
    }else if(strncmp(s.c_str(), " config", 7) == 0){
      Serial.println("configuration commands:");
      Serial.println("write_config            write configuration in ram to config.json.");
      Serial.println("read_config             reads config.json into ram.");
      Serial.println("view_config             prints out the current configuration stored in ram.");
      Serial.println("edit_config             edit configuration in ram, using json to specify values.");
    }else if(strncmp(s.c_str(), " wifi", 5) == 0){
      Serial.println("wifi commands:");
      Serial.println("write_networks          write netork list in ram to wifi.json.");
      Serial.println("read_networks           reads wifi.json into ram.");
      Serial.println("view_networks           prints out the current network list stored in ram.");
      Serial.println("delete_network ssid     remove wifi network specified from network list in ram.");
      Serial.println("add_networks            add a new network(s) to the list in ram, using json to define the wifi credentials");
      Serial.println("ip_address              prints local ip address (if wifi is connected).");
    }else if(strncmp(s.c_str(), " misc", 5) == 0){
      Serial.println("misc commands:");
      Serial.println("factory_reset           delete config.json,tasks.json and wifi.json, and create new ones with default values\n");
      //Serial.println("help                you just used it, you silly bean :P");
      Serial.println("reboot                  reboot the mcu");
      Serial.println("ver                     returns firmware version.");
    }else{
      Serial.println("what do you need help with?");
      Serial.println("try \'help topic\' where topic is any of the following:");
      Serial.println("file, tasks, config, wifi, misc");
    }       
  }