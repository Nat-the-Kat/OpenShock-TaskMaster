#pragma once
#include "task.h"

namespace task_master{

  class task_repeat : public task{
    public:
      task_repeat();
      task_repeat(JsonObject object);
      virtual void print()override;
      virtual JsonDocument to_json()override;
      virtual void check()override;
      virtual void disable()override;
      virtual void enable()override;
    private:
      time_t start;
      time_t end;
      time_t interval;
      time_t next_warn;
      time_t next_punish;
      time_t next_time;

      bool interval_active = true;

      void calc_next_time();


  };
}

