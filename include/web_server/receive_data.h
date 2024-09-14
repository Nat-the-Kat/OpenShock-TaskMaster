#include "server.h"

namespace task_master::web_server{
  void config_write();
  void tasks_write();
  void networks_write();
  void recieve_data(int dest);
}