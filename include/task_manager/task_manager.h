#pragma once
#include <ArduinoJson.h>
#include <Vector>
#include <string>
#include "task.h"

  namespace task_master {

    class task_manager {
      public:
        void init();
        void print();
        void read_from_file();
        void write_to_file();

        //task management functions
        void clear_tasks();
        void delete_task(const char* task_name);
        void add_tasks();

        //this one function does all the complicated stuff that i put off until the end.
        //turns outs it wasn't that complicated at all
        void check_tasks();

        std::string write_to_string();
      private:
        std::vector <task*> task_list;
        void read_from_stream(Stream &s);
        
        void add_task(JsonObject object);
    };
  }

  extern task_master::task_manager manager;

