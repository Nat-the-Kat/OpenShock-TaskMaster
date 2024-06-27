#include <LittleFS.h>
#include "helpers/filesystem.h"

  void read(const char* filename){
    //open the file, error out if it doesn't exist
    File file = LittleFS.open(filename, "r");
    if(!file){
      Serial.print("Failed to open file: "); Serial.println(filename);
      return;
    }
    Serial.print(filename); Serial.println(" contents:");

    //print file contents
    while(file.available()){
      Serial.write(file.read());
    }

    //close the file
    Serial.println("\nEOF");
    file.close();
    return;
  }

  void delete_file(const char* filename){
    if(LittleFS.remove(filename)){
      Serial.println("file deleted!");
    }else{
      Serial.println("file not found!");
    }
    return;
  }
