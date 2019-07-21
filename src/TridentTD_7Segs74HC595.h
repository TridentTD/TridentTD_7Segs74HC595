/*
 [TridentTD] : MANABU's Esp8266 IoT Library
 www.facebook.com/miniNodeMCU
 
 TridentTD_7Segs74HC595.h - LED Display 7 segments (74hc595) for ESP8266 & Arduino

 Version 1.0  25/05/2560 Buddism Era  (2017)
 
 Copyright (C) 2017   Ven.Phaisarn Techajaruwong


    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see http://www.gnu.org/licenses.
*/



#ifndef _TRIDENTTD_SEVENSEGS74HC595_H_
#define _TRIDENTTD_SEVENSEGS74HC595_H_

#define  TRIDENTTD_DEBUG_MODE

#ifdef TRIDENTTD_DEBUG_MODE
    #define DEBUG_PRINTER Serial
    #define DEBUG_PRINT(...) { DEBUG_PRINTER.print(__VA_ARGS__); }
    #define DEBUG_PRINTLN(...) { DEBUG_PRINTER.println(__VA_ARGS__); }
#else
    #define DEBUG_PRINT(...) { }
    #define DEBUG_PRINTLN(...) { }
#endif


#ifndef ESP_H
#include <avr/io.h>
#include <avr/interrupt.h>
#define RESOLUTION 65536    // Timer1 is 16 bit
#endif

#define MAX_DIGITS 4

#define INVERT_MODE     1

struct DigitalTube{
  String  name;
  int     columns[MAX_DIGITS];
};

class TridentTD_7Segs74HC595 {
  public:
    TridentTD_7Segs74HC595(int SCLK, int RCLK, int DIO, int num_module=1) ;  // SCLK : Serial Clock; RCLK : Register Clock; DIO = Serial Data

    bool    addModule(String module_name);
    void    init(bool invert_mode=false);
    void    setNumber(float f_number, int decimal=2);
    void    setText(String text);
    void    setTextScroll(String text, int scrolltime=500 , int nLoop=1);

    void    setNumber(String module_name, float f_number, int decimal=2);
    void    setText(String mudule_name, String text);
    void    setTextScroll(String mudule_name, String text, int scrolltime=500 , int nLoop=1);

    String  getVersion();
  private:
    float   _version = 2.1;
    bool    _invert_mode = false;
    void    _setColumn(int col, int character, boolean addDot=false,String module_name="");
    int     _getModuleIndex(String module_name);
    int     _module_i;
 
    #ifndef ESP_H
    unsigned char clockSelectBits;
    void    _isr_init(long microseconds=1000000);
    void    _setPR(long microseconds);
    void    _isr_add(void (*isr)(), long microseconds=-1);
    #endif

    int  charmap[65]= {
            3,159,37,13,153,73,65,31,1,9,   // (0-9)0-9
            17,1,99,3,97,113,67,145,243,135,145,227,85,19,3,49,25,115,73,31,129,129,169,145,137,37,       //(10-35)A-Z
            5,193,229,133,33,113,9,209,247,143,81,227,85,213,197,49,25,245,73,225,199,199,169,145,137,37, //(36-61)a-z
            253,255,53                      //(62)- , (63) space, (64) ?
          };
};



#endif 
/* _TRIDENTTD_SEVENSEGS74HC595_H_ */


