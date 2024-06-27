#include "oled.h"

  void ssd1306::write_command(uint8_t c){
    Wire.beginTransmission(address);
    Wire.write(0x80);
    Wire.write(c);
    Wire.endTransmission();
  }

  void ssd1306::write_data(unsigned char data){
    Wire.beginTransmission(address);
    Wire.write(0x40);
    Wire.write(data);
    Wire.endTransmission();
  }

  void ssd1306::init(uint8_t a){
    address= a;
    write_command(0xae);  //display off
    write_command(0xd5);  //set osc frequency
    write_command(0x80);
    write_command(0xa8);  //set mux ratio
    write_command(0x3f);
    write_command(0xd3);  //set display offset
    write_command(0x00);
    write_command(0x8d);  //set charge pump regulator
    write_command(0x14);
    write_command(0x40);  //set display start line
    write_command(0xa1);  //set segment remap
    write_command(0xc8);  //set com scan direction
    write_command(0xda);  //set com pin configuration
    write_command(0x12);
    write_command(0x81);  //set contrast control
    write_command(0xcf);
    write_command(0xd9);  //set precharge
    write_command(0xf1);
    write_command(0xdb);  //adjust regulator
    write_command(0x40);
    write_command(0xa4);  //disable entire display on? super confusing command... data sheet could use some better wording
    write_command(0xa6);  //set normal display
    write_command(0x2e);  //deactivate scrolling
    write_command(0x20);  //set horizontal display mode
    write_command(0x00);
    write_command(0xaf);  //display on
  }

  void ssd1306::load_font(const uint8_t* f){
    font = f;
    font_width = font[0];
    first = font[1];
    last = font[2];
  }

  void ssd1306::cursor_pos(uint8_t row, uint8_t col){
    pos_row = row;
    pos_col = col;
    uint8_t pos = font_width * col;
    write_command(0xB0 + row);  //set page

    /*this is the only binary arithmetic in this project. :(
    i miss assembly... 
    */
    uint8_t l = pos & 0x0f;
    uint8_t h = (pos >> 4) & 0x0f;
    write_command(0x00 + l);  //set column lower nibble
    write_command(0x10 + h);  //set column higher nibble
  }

  void ssd1306::clear(){
    write_command(0xae);
    for(int r = 0; r < 8; r++) {
      cursor_pos(r,0);
      for(int c = 0; c < 16; c++) {  
        write_char_8(' ');
      }
    }
    write_command(0xaf);
    cursor_pos(0,0);
  }

  void ssd1306::timed_clear(int ms){
    add_alarm_in_ms(ms,clear_callback,NULL,false);
  }

  void ssd1306::set_frame_callback(int ms, bool (*func)(repeating_timer *t)){
    add_repeating_timer_ms(-ms,func,NULL,&frame_timer);
  }

  void ssd1306::write_char_8(uint8_t c){
    if(c < first || c > last) {
      c = ' ';
    }
    for(uint8_t i = 0; i < font_width; i++){
      int pos = (c-first) * font_width + 3 + i;
      write_data(font[pos]);
    }
  }

  void ssd1306::write_time_8(tod time, bool seconds){
    char buffer[16];
    uint8_t len;
    if(seconds){ 
      len = sprintf(buffer, "%02d:%02d:%02d", time.hr,time.min,time.sec);
    }else{
      len = sprintf(buffer, "%02d:%02d", time.hr,time.min);
    }
    for(int i = 0; i < len; i++){
      write_char_8(buffer[i]);
    }
  }

  void ssd1306::write_string_8(const char *string){
    uint8_t i = 0;
    while(string[i]){
      write_char_8(string[i]);
      i++;
    }
  }

  void ssd1306::write_string_8(std::string s){
    write_string_8(s.c_str());
  }

  //this is kinda scuffed, but i dont really want to add in a way to tell which rows are 
  //8 pixel charcters and which are 16 pixel characters
  void ssd1306::write_char_16(char ch){
    write_char_8(ch);
    cursor_pos(pos_row + 1, pos_col);//move to the next page of memory
    write_char_8(ch + 96);
    cursor_pos(pos_row - 1, pos_col + 1); 
  }

  void ssd1306::write_string_16(const char *string){
    load_font(font16);
    int i = 0;
    while(string[i]){
      write_char_16(string[i]);
      i++;
    }
  }

    void ssd1306::write_string_16(std::string s){
      write_string_16(s.c_str());
  }

  void ssd1306::write_time_16(tod time, bool seconds){
    char buffer[16];
    uint8_t len;
    if(seconds){ 
      len = sprintf(buffer, "%02d:%02d:%02d", time.hr,time.min,time.sec);
    }else{
      len = sprintf(buffer, "%02d:%02d", time.hr,time.min);
    }
    for(int i = 0; i < len; i++){
      write_char_16(buffer[i]);
    }
  }
  


  ssd1306 oled;

  int64_t clear_callback(alarm_id_t id, void *data){
    oled.clear();
    return 0;
  }
