#include "helpers/stream_to_string.h"


//this can prolly get purged, but im going to keep it for rn
std::string stream_to_string(Stream &s){
  std::string out;
  while(s.available() > 0){
    char c = s.read();
    if(c == '\n') break;
    out.push_back(c);
  }
  return out;
}