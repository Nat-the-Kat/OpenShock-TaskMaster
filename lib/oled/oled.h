#pragma once
#include <Arduino.h>
#include <Wire.h>
#include "fonts/font8.h"
#include "fonts/font16.h"
#include <tod.h>
#include <pico/stdlib.h>

class ssd1306 {
  public:

    void init(int sda, int scl, uint8_t a = 0x3c);
    void clear();
    void timed_clear(int ms);
    void load_font(const uint8_t* font);
    void cursor_pos(uint8_t row, uint8_t col);
    void set_frame_callback(int ms, bool (*func)(repeating_timer *t));

    //8 pixel text functions
    void write_string_8(const char *string); 
    void write_string_8(std::string s); 
    void write_time_8(tod time, bool seconds = false);

    //16 pixel text functions
    void write_string_16(const char *string);
    void write_string_16(std::string s);
    void write_time_16(tod time, bool seconds = false);

  private:
    void write_command(uint8_t c);
    void write_data(uint8_t d);
    void write_char_8(uint8_t c);
    void write_char_16(char ch);
    repeating_timer frame_timer;

    

    const uint8_t* font;
    uint8_t font_width;   //character width
    uint8_t pos_row;
    uint8_t pos_col;
    uint8_t address;    //i2c address, if for some reason whatever module you use has a different address
    uint8_t first;    //first character that the font has data for
    uint8_t last;     //last character that the font has data for
};

int64_t clear_callback(alarm_id_t id, void *data);

extern ssd1306 oled;

