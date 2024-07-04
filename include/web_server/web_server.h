#pragma once
#include "wifi_manager/wifi_manager.h"
#include "task_manager/task_manager.h"
#include "config/config.h"
#include <WebServer.h>
#include <LittleFS.h>

namespace task_master{
  namespace web_server{
    extern WebServer server;
    void init();
    void not_found();
    void index_page();
    void config_page();
    void networks_page();
    void tasks_page();

    void config_ram();
    void config_flash();

    void tasks_ram();
    void tasks_flash();

    void networks_ram();
    void networks_flash();

    void fetch_jquery();
    }
}