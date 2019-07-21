/*
 [TridentTD] : MANABU's Esp8266 IoT Library
 www.facebook.com/miniNodeMCU
 
 TridentTD_7Segs74HC595.cpp - LED Display 7 segments (74hc595) for ESP8266 & Arduino

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

#include <Arduino.h>
#include "TridentTD_7Segs74HC595.h"


DigitalTube *modules;


int sclk_p, rclk_p, dio_p, nModule;
int col_index;

TridentTD_7Segs74HC595::TridentTD_7Segs74HC595(int SCLK, int RCLK, int DIO, int num_module) {
  sclk_p    = SCLK;
  rclk_p    = RCLK;
  dio_p     = DIO;
  nModule   = num_module;
  _module_i = 0;
  col_index = 0;

  modules = new DigitalTube[nModule];

  pinMode(sclk_p,OUTPUT);
  pinMode(rclk_p,OUTPUT);
  pinMode(dio_p ,OUTPUT);

}

bool TridentTD_7Segs74HC595::addModule(String module_name){
  if(_module_i<nModule){
    modules[_module_i++].name = module_name;

    setText(module_name,"    ");
    return true;
  }
  return false;
}

void TridentTD_7Segs74HC595::init(bool invert_mode){
  _invert_mode = invert_mode;
  digitalWrite(rclk_p, HIGH);

  #ifdef ESP_H
    timer1_isr_init();
    timer1_attachInterrupt([](){
  #else
    _isr_init(2000);
    _isr_add([]() {
  #endif
      digitalWrite(rclk_p, LOW);
      for(int i=nModule-1; i >=0; i--){
        shiftOut(dio_p, sclk_p, LSBFIRST, modules[i].columns[col_index]);
        shiftOut(dio_p, sclk_p, MSBFIRST, 1 << col_index);
      }
      digitalWrite(rclk_p, HIGH);
      col_index = col_index >= MAX_DIGITS - 1 ? 0 : col_index + 1;
    });
  #ifdef ESP_H
    timer1_enable(TIM_DIV16, TIM_EDGE, TIM_LOOP);  //5MHz (5 ticks/us - 1677721.4 us max)
    timer1_write((clockCyclesPerMicrosecond() / 16) * 200);
  #endif

}

#ifndef ESP_H
void    (*isrCallback)();

ISR(TIMER1_OVF_vect)          // interrupt service routine that wraps a user defined function supplied by attachInterrupt
{
  isrCallback();
}

void TridentTD_7Segs74HC595::_isr_init(long microseconds)
{
  TCCR1A = 0;                 // clear control register A 
  TCCR1B = _BV(WGM13);        // set mode 8: phase and frequency correct pwm, stop the timer
  _setPR(microseconds);
}

void TridentTD_7Segs74HC595::_setPR(long microseconds)   // AR modified for atomic access
{
  
  long cycles = (F_CPU / 2000000) * microseconds;                                // the counter runs backwards after TOP, interrupt is at BOTTOM so divide microseconds by 2
  if(cycles < RESOLUTION)              clockSelectBits = _BV(CS10);              // no prescale, full xtal
  else if((cycles >>= 3) < RESOLUTION) clockSelectBits = _BV(CS11);              // prescale by /8
  else if((cycles >>= 3) < RESOLUTION) clockSelectBits = _BV(CS11) | _BV(CS10);  // prescale by /64
  else if((cycles >>= 2) < RESOLUTION) clockSelectBits = _BV(CS12);              // prescale by /256
  else if((cycles >>= 2) < RESOLUTION) clockSelectBits = _BV(CS12) | _BV(CS10);  // prescale by /1024
  else        cycles = RESOLUTION - 1, clockSelectBits = _BV(CS12) | _BV(CS10);  // request was out of bounds, set as maximum
  
  char oldSREG;
  oldSREG = SREG;       
  cli();              // Disable interrupts for 16 bit register access
  ICR1 = cycles;                                          // ICR1 is TOP in p & f correct pwm mode
  SREG = oldSREG;
  
  TCCR1B &= ~(_BV(CS10) | _BV(CS11) | _BV(CS12));
  TCCR1B |= clockSelectBits;                                          // reset clock select register, and starts the clock
}

void TridentTD_7Segs74HC595::_isr_add(void (*isr)(), long microseconds)
{
  if(microseconds > 0) _setPR(microseconds);
  isrCallback = isr;                                       // register the user's callback with the real ISR
  TIMSK1 = _BV(TOIE1);                                     // sets the timer overflow interrupt enable bit
  // might be running with interrupts disabled (eg inside an ISR), so don't touch the global state
//  sei();
  TCCR1B |= clockSelectBits;//resume();                       
}
#endif


int TridentTD_7Segs74HC595::_getModuleIndex(String module_name){

  if( module_name != ""){
    for(int i=0; i< nModule; i++){
      if( module_name == modules[i].name )
        return i;
    }
  }
  return 0;
}

void TridentTD_7Segs74HC595::_setColumn(int col, int character, boolean addDot, String module_name) {

  int idx;
  if((character >= 0)&&(character <= 9)){ idx = character; } 
  else if((character >= 32)&&(character <= 47)){ idx = (character ==45)? 62 : 63; }
  else if(character == 63) { idx = 64; }
  else if((character >= '0')&&(character <= '9')) { idx = character - 48; } 
  else if((character >= 'A')&&(character <= 'Z')) { idx = character - 55; } 
  else if((character >= 'a')&&(character <= 'z')) { idx = character - 61; }
  modules[_getModuleIndex(module_name)].columns[col] = charmap[idx] & ((addDot)? 254: 255);
}


void TridentTD_7Segs74HC595::setNumber(float f_number, int decimal){
  setNumber("", f_number, decimal);
}

void TridentTD_7Segs74HC595::setNumber(String module_name, float f_number, int decimal){

  decimal = (decimal > MAX_DIGITS - 1)? MAX_DIGITS-1 : decimal;
  bool bZero = true;
  int number =  ((int)((f_number+0.0001) * pow(10,decimal))) % (int)pow(10,MAX_DIGITS);
  
  int value;
  for(int col= MAX_DIGITS-1; col >= 0 ; col--){
    value = ((int)(number / pow(10, col )))%10;
    if(value != 0) { bZero = false; }
    _setColumn((_invert_mode)? (MAX_DIGITS-1-col) : col, (col>decimal&& bZero)? ' ':value, (col!=decimal || decimal ==0)? false:true, module_name );
    delay(1);
  }
}

void TridentTD_7Segs74HC595::setText(String text){
  int len = text.length();
  const char *text_c = text.c_str();


  int chr_index=0;
  for(int col=MAX_DIGITS -1; col >=0  ; col--){
    int character = (chr_index < len)? text_c[chr_index]:' ';

    boolean addDot= false;
    if(text[chr_index+1] == '.'){
      addDot = true; chr_index++;
    }
    _setColumn( (_invert_mode)? (MAX_DIGITS-1-col) :col, character, addDot );
    chr_index++;
    delay(1);
  }
}

void TridentTD_7Segs74HC595::setText(String mudule_name, String text){
  int len = text.length();
  const char *text_c = text.c_str();


  int chr_index=0;
  for(int col=MAX_DIGITS -1; col >=0  ; col--){
    int character = (chr_index < len)? text_c[chr_index]:' ';

    boolean addDot= false;
    if(text[chr_index+1] == '.'){
      addDot = true; chr_index++;
    }
    _setColumn( (_invert_mode)? (MAX_DIGITS-1-col):col, character, addDot, mudule_name );
    chr_index++;
    delay(1);
  }
}


void TridentTD_7Segs74HC595::setTextScroll(String text, int scrolltime ,int nLoop ){
  nLoop = (nLoop<=0)? 1 : nLoop;
  
  for(int j = 0; j < nLoop; j++){
    String text2 = String("    ")+ text;
    int len      = text2.length();

    for(int i = 0; i < len; i++) {
      String subtext = text2.substring(0,8);
      setText(subtext);
      delay(scrolltime);

      if(text2.substring(1,2) != "."){
        text2 = text2.substring(1)+text2.substring(0,1);
      }else{
        text2 = text2.substring(2)+text2.substring(0,2);
      }
    }
    
    setText("    ");
    delay(scrolltime);
  }
}

void TridentTD_7Segs74HC595::setTextScroll(String module_name, String text, int scrolltime ,int nLoop ){
  nLoop = (nLoop<=0)? 1 : nLoop;
  
  for(int j = 0; j < nLoop; j++){
    String text2 = String("    ")+ text;
    int len      = text2.length();

    for(int i = 0; i < len; i++) {
      String subtext = text2.substring(0,8);
      setText(module_name, subtext);
      delay(scrolltime);

      if(text2.substring(1,2) != "."){
        text2 = text2.substring(1)+text2.substring(0,1);
      }else{
        text2 = text2.substring(2)+text2.substring(0,2);
      }
    }
    
    setText(module_name, "    ");
    delay(scrolltime);
  }
}

String TridentTD_7Segs74HC595::getVersion(){
  return (String)("[TridentTD_7Segs74HC595] Version ") + String(_version);
}

