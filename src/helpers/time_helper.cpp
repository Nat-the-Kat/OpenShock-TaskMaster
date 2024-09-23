#include <helpers/time_helper.h>

time_t start_of_day(){
    time_t ctime = time(nullptr);
    tm day;
    localtime_r(&ctime,&day);
    day.tm_hour = 0; day.tm_min = 0; day.tm_sec = 0;
    time_t start = mktime(&day);
    return start;
}