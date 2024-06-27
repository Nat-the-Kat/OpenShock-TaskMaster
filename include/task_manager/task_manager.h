#pragma once
#include <LittleFS.h>
#include <ArduinoJson.h>
#include "task.h"
#include "task_repeating.h"
#include "task_timed.h"
#include "config/config.h"
#include "oscontrol.h"
#include <Vector>
#include <hardware/rtc.h>
#include <pico/stdlib.h>
#include <pico/util/datetime.h>

  namespace task_master {

    class task_manager {
      public:
        config* conf;
        void init();
        void init_rtc();
        void print();
        void read_from_file();
        void write_to_file();

        //task management functions
        void clear_tasks();
        void delete_task(const char* task_name);
        void add_tasks();
        void add_task(JsonObject object);

        //this one function does all the complicated stuff that i put off until the end.
        //turns outs it wasn't that complicated at all
        void check_tasks(config* conf);


      private:
        tod* clear_time = nullptr;
        std::vector <task*> task_list;
        static void alarm(void);
        void create_tasks_json();

    };
  }

