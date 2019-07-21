#include <TridentTD_7Segs74HC595.h>

#define SCLK        D5
#define RCLK        D6
#define DIO         D7

TridentTD_7Segs74HC595  my7SEGMENT( SCLK, RCLK, DIO);


void setup() {
  Serial.begin(115200);
  Serial.println();

  my7SEGMENT.init();  
  // my7SEGMENT.init(INVERT_MODE);   // หากต้องการให้แสดงผลเรียงใหม่แบบกลับตัวเลข 4 หลักหน้าไปหลัง
  
  my7SEGMENT.setTextScroll("ESP-8266");

}

void loop() {
  for(float f=0; f <99; f++){
    my7SEGMENT.setNumber(f,0);
    delay(50);
  }

  for(float f=0; f <20; f+=0.1){
    my7SEGMENT.setNumber(f,1);
    delay(50);
  }

  for(float f=0; f <10; f+=0.01){
    my7SEGMENT.setNumber(f,2);
    delay(50);
  }

  for(float f=0; f <1.1; f+=0.001){
    my7SEGMENT.setNumber(f,3);
    delay(50);
  }

}
