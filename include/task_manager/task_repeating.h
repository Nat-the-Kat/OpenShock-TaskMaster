#pragma once
#include "task.h"

namespace task_master{

  class task_repeat : public task{
    public:
      tod start;
      tod end;
      tod interval;
      bool interval_active = true;

      task_repeat();
      task_repeat(JsonObject object);
      virtual void print()override;
      virtual JsonDocument to_json()override;
      virtual void check()override;
      virtual void disable()override;
    private:
      tod next_start;
      tod next_time;
      tod next_warn;
      tod next_punish;

      void calc_next_time(tod time);

  };
}

