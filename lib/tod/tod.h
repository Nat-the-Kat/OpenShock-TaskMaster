#pragma once
#include <ArduinoJson.h>

  //"simple" time-of-day class
  //this definetely was a stupid choice, i really should've done something else.
  //oh well, sunk cost and all that... 
  class tod{
    public:
    int hr = 0;
    int min = 0;
    int sec = 0;

    void print();
    JsonDocument to_json();
    tm to_tm();

    tod();
    tod(int h, int m, int s);
    tod(tm time);
    tod(JsonArray array);

    tod operator+(tod const& addend);

    tod operator-(tod const &subtrahend);

    bool operator==(tod const &t);

    bool operator!=(tod const &t);
  
    bool operator<(tod const &t);

    bool operator>(tod const &t);

    bool operator<=(tod const &t);

    bool operator>=(tod const &t);
  };


  bool between_inclusive(tod c_time, tod start, tod end);
  bool between(tod c_time, tod start, tod end);

  extern tod timezone;
  extern tod current_time;