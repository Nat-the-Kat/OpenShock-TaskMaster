#include "helpers/filesystem.h"
#include <LittleFS.h>

  void read(std::string filename){
    //open the file, error out if it doesn't exist
    File file = LittleFS.open(filename.c_str(), "r");
    if(!file){
      Serial.print("Failed to open file: "); Serial.println(filename.c_str());
      return;
    }
    Serial.print(filename.c_str()); Serial.println(" contents:");

    //print file contents
    while(file.available()){
      Serial.write(file.read());
    }

    //close the file
    Serial.println("\nEOF");
    file.close();
    return;
  }

  void delete_file(std::string filename){
    if(LittleFS.remove(filename.c_str())){
      Serial.println("file deleted!");
    }else{
      Serial.println("file not found!");
    }
    return;
  }
