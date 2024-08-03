#pragma once
#include "task.h"
namespace task_master{
  class task_timed : public task{
    public:
      task_timed();
      task_timed(JsonObject object);
      virtual void print()override;
      virtual JsonDocument to_json()override;
      virtual void check()override;
      virtual void disable()override;
      virtual void enable()override;
    private:
      time_t window;

  };
}
