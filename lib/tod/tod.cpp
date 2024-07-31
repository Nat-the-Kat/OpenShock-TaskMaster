#include "tod.h"
#include <time.h>

  void tod::print(){
    char buffer[30];
    sprintf(buffer, "%d:%d:%d", hr, min, sec);
    Serial.println(buffer);
  }

  JsonDocument tod::to_json(){
    JsonDocument doc;
    doc.add(hr);
    doc.add(min);
    doc.add(sec);
    return doc;
  }

  tm tod::to_tm(){
    tm temp;
    temp.tm_hour = hr;
    temp.tm_min = min;
    temp.tm_sec = sec;
    return temp;
  }

  tod::tod(){

  }

  tod::tod(int h, int m, int s){
    hr = h;
    min = m;
    sec = s;
  }

  tod::tod(tm time){
    hr = time.tm_hour;
    min = time.tm_min;
    sec = time.tm_sec;
  }

  tod::tod(JsonArray array){
    hr=array[0];
    min=array[1];
    sec=array[2];
  }

  tod tod::operator+(const tod &addend){
    tod temp;
    tod buffer;
    temp.sec = sec + addend.sec;
    if(temp.sec >= 60){
      temp.sec -= 60;
      buffer.min++;
    }
    temp.min = min + addend.min + buffer.min;
    if(temp.min >= 60){
      temp.min -= 60;
      buffer.hr++;
    }
    temp.hr = hr + addend.hr + buffer.hr;
    if(temp.hr >= 24){
      temp.hr -= 24;
    }
    if(temp.hr < 0){
      temp.hr += 24;
    }
    return temp;
  }

    //it was around here that it really became obvious to me i should've done this differently....

  tod tod::operator-(const tod &subtrahend){
    tod temp;
    tod buffer;
    temp.sec = sec - subtrahend.sec;
    if(temp.sec < 0){
      temp.sec += 60;
      buffer.min++;
    }
    temp.min = min - subtrahend.min - buffer.min;
    if(temp.min < 0){
      temp.min += 60;
      buffer.hr++;
    }
    temp.hr = hr - subtrahend.hr - buffer.hr;
    if(temp.hr < 0){
      temp.hr += 24;
    }
    if(temp.hr > 23){
      temp.hr -= 24;
    }
    return temp;
  }

  bool tod::operator==(tod const &t){
    if(hr == t.hr && min == t.min && sec == t.sec) return true;
    return false;
  } 

  bool tod::operator!=(tod const &t){
    if(hr != t.hr || min != t.min || sec != t.sec) return true;
    return false;
  } 
    
  bool tod::operator<(tod const &t){
    if(hr<t.hr){
      return true;
    }else if(hr == t.hr && min < t.min){
      return true;
    }else if(hr == t.hr && min == t.min && sec < t.sec){
      return true;
    }else{
      return false;
    }
  } 

  bool tod::operator>(tod const &t){
    if(hr>t.hr){
      return true;
    }else if(hr == t.hr && min > t.min){
      return true;
    }else if(hr == t.hr && min == t.min && sec > t.sec){
      return true;
    }else{
      return false;
    }
  } 

  bool tod::operator<=(tod const &t){
    if(hr<=t.hr){
      return true;
    }else if(hr == t.hr && min <= t.min){
      return true;
    }else if(hr == t.hr && min == t.min && sec <= t.sec){
      return true;
    }else{
      return false;
    }
  }

  bool tod::operator>=(tod const &t){
    if(hr >= t.hr){
      return true;
    }else if(hr == t.hr && min >= t.min){
      return true;
    }else if(hr == t.hr && min == t.min && sec >= t.sec){
      return true;
    }else{
      return false;
    }
  } 

  //returns true if c_time is between start and end, including if c_time = start or c_time = end
  bool between_inclusive(tod c_time, tod start, tod end){
    tod temp_end = end;
    tod temp_current = c_time;
    if(start > end){ //if true, end time must be past midnight
      temp_end.hr += 24; //temporarily make end time bigger
      if(c_time < end){
        temp_current.hr += 24; // if i did this math right, 
      }
    }

    if(temp_current >= start && temp_current <= temp_end){
      return true;
    }
    return false;
  }

  //returns true if c_time is between start and end, but not if c_time = start or c_time = end
  bool between(tod c_time, tod start, tod end){
    tod temp_end = end;
    tod temp_current = c_time;
    if(start > end){
      temp_end.hr += 24;
    }
    if(c_time < end){
      temp_current.hr += 24;
    }
    if(temp_current > start && temp_current < temp_end){
      return true;
    }
    return false;
  }

  void update_time(){
    tm temp;
    time_t now = time(nullptr); 
    localtime_r(&now, &temp);
    current_time = tod(temp);
  }

  tod current_time;

