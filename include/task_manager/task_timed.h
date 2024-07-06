#pragma once
#include "task.h"
namespace task_master{
  class task_timed : public task{
    public:
      tod window;

      task_timed();
      task_timed(JsonObject object);
      virtual void print()override;
      virtual JsonDocument to_json()override;
      virtual void check()override;
    


  };
}
