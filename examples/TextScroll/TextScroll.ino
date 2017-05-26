#include <TridentTD_7Segs74HC595.h>

#define SCLK        D5
#define RCLK        D6
#define DIO         D7

TridentTD_7Segs74HC595  my7SEGMENT( SCLK, RCLK, DIO);


void setup() {
  Serial.begin(115200);
  Serial.println();

  my7SEGMENT.init();  
  my7SEGMENT.setTextScroll("ESP-8266");

}

void loop() {
  int t = random(100);

  my7SEGMENT.setTextScroll("t."+String(t)+ "c");

  int h = random(100);

  my7SEGMENT.setTextScroll("H."+String(h));

}
