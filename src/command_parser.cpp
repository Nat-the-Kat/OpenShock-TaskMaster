#include "command_parser.h"
#include <tod.h>

//very simple serial command parser to allow modifying the configuration

//this file is an unholy mess, it desperately needs cleanup. there is almost certainly a way better way to do this.
  void task_master::parse_serial(task_manager* manager, config* conf, wifi_manager* w_manager){
    char input[100] = "\n";
    int index = 0;
    while(Serial.available() > 0 && index < sizeof(input) - 1){
      char c = Serial.read();
      if (c == '\n') break;
      input[index++] = c;
    }
    input[index] = '\0';

    Serial.print(">"); Serial.println(input);

    //compare against valid commands
    if(strncmp(input, "read ", 5) == 0){
      char* filename = input + 5;
      read(filename);

    }else if(strncmp(input, "delete ", 7) == 0){
      char* filename = input + 7;
      delete_file(filename);

    }else if(strncmp(input, "fsinfo", 6) == 0){
      FSInfo64 fsinfo;
      LittleFS.info64(fsinfo);
      char buffer[128];
      sprintf(buffer, "fs info:\ntotalBytes: %ld\nusedBytes: %ld\nblockSize: %d\npageSize: %d\nmaxOpenFiles: %d\nmaxPathLength: %d", fsinfo.totalBytes, fsinfo.usedBytes, fsinfo.blockSize, fsinfo.pageSize, fsinfo.maxOpenFiles, fsinfo.maxPathLength);
      Serial.println(buffer);

    }else if(strncmp(input, "help", 4) == 0){
      char* topic = input + 4;
      help(topic);

    }else if(strncmp(input, "reboot", 6) == 0){
      rp2040.reboot();

    }else if(strncmp(input, "heap", 4) == 0){
      Serial.print("theoretical free heap:"); Serial.println(rp2040.getFreeHeap());
      Serial.print("total heap:"); Serial.println(rp2040.getTotalHeap());
      Serial.print("used heap:"); Serial.println(rp2040.getUsedHeap());

    }else if(strncmp(input, "ver", 3) == 0){
      Serial.println("v0.00");

    }else if(strncmp(input, "test", 4) == 0){


    }else if(strncmp(input, "write_tasks", 11) == 0){
      manager->write_to_file();

    }else if(strncmp(input, "read_tasks", 10) == 0){
      manager->read_from_file();

    }else if(strncmp(input, "view_tasks", 10) == 0){
      manager->print();

    }else if(strncmp(input, "clear_tasks", 11) == 0){
      manager->clear_tasks();

    }else if(strncmp(input, "delete_task ", 12) == 0){
      char* task_name = input + 12;
      manager->delete_task(task_name);

    }else if(strncmp(input, "add_tasks", 9) == 0){
      manager->add_tasks();

    }else if(strncmp(input, "write_config", 12) == 0){
      conf->write_to_file();

    }else if(strncmp(input, "read_config", 11) == 0){
      conf->read_from_file();

    }else if(strncmp(input, "view_config", 11) == 0){
      conf->print();

    }else if(strncmp(input, "edit_config", 11) == 0){
      conf->edit_config();

    }else if(strncmp(input, "write_networks", 14) == 0){
      w_manager->write_to_file();

    }else if(strncmp(input, "read_networks", 13) == 0){
      w_manager->read_from_file();

    }else if(strncmp(input, "view_networks", 13) == 0){
      w_manager->print_networks();

    }else if(strncmp(input, "add_networks", 12) == 0){
      w_manager->add_networks();

    }else if(strncmp(input, "clear_networks", 14) == 0){
      w_manager->clear_networks();

    }else if(strncmp(input, "delete_network ", 15) == 0){
      char* task_name = input + 15;
      w_manager->delete_network(task_name);

    }else if(strncmp(input, "factory_reset", 13) == 0){
      delete_file("config.json");
      delete_file("tasks.json");
      delete_file("wifi.json");
      conf->init();
      manager->init();
      w_manager->init();

    }else{
      Serial.println("Unknown command, try help to see valid commands.");
    }
    return;
  }

  //this is a kinda gross way to do this, but it works soooo.....
  void task_master::help(const char* string){ 
    if(strncmp(string, " file", 5) == 0){
      Serial.println("file commands:");
      Serial.println("read filename           output contents of specified file to the serial terminal.");
      Serial.println("delete filename         deletes specified file.");
      Serial.println("fsinfo                  outputs some info about LittleFS, including used space");
    }else if(strncmp(string, " tasks", 6) == 0){
      Serial.println("task editing commands:");
      Serial.println("write_tasks             write task list in ram to tasks.json.");
      Serial.println("read_tasks              reads tasks.json into ram.");
      Serial.println("view_tasks              prints out the current task list stored in ram.");
      Serial.println("delete_task name        remove task specified from task list in ram.");
      Serial.println("clear_tasks             remove all tasks from task list in ram.");
      Serial.println("add_tasks               add a new task (or tasks) to the list in ram, using json to define the task(s)");
    }else if(strncmp(string, " config", 7) == 0){
      Serial.println("configuration commands:");
      Serial.println("write_config            write configuration in ram to config.json.");
      Serial.println("read_config             reads config.json into ram.");
      Serial.println("view_config             prints out the current configuration stored in ram.");
      Serial.println("edit_config             edit configuration in ram, using json to specify values.");
    }else if(strncmp(string, " wifi", 5) == 0){
      Serial.println("wifi commands:");
      Serial.println("write_networks          write netork list in ram to wifi.json.");
      Serial.println("read_networks           reads wifi.json into ram.");
      Serial.println("view_networks           prints out the current network list stored in ram.");
      Serial.println("delete_network ssid     remove wifi network specified from network list in ram.");
      Serial.println("add_networks            add a new network(s) to the list in ram, using json to define the wifi credentials");
    }else if(strncmp(string, " misc", 5) == 0){
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