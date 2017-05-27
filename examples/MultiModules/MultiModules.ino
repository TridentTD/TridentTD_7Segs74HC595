#include <TridentTD_7Segs74HC595.h>

#define SCLK        5
#define RCLK        6
#define DIO         7
#define NUM_MODULES 3     // จำนวนโมดูลที่ต่อพ่วงกันไปเรื่อยๆ

TridentTD_7Segs74HC595  my7SEGMENT( SCLK, RCLK, DIO, NUM_MODULES);


void setup() {
  Serial.begin(115200);
  Serial.println();

  Serial.println(my7SEGMENT.getVersion());

  my7SEGMENT.addModule("Temp");   // เพิ่ม  โมดูล  ด้วยการกำหนดชื่อที่ต้องการ
  my7SEGMENT.addModule("Humid");
  my7SEGMENT.addModule("Pressure");
  
  my7SEGMENT.init();

  my7SEGMENT.setTextScroll("Temp","ESP-8266",300);
  my7SEGMENT.setTextScroll("Humid","Arduino",300);
  my7SEGMENT.setTextScroll("Pressure","Hello",300);

}

void loop() {
  my7SEGMENT.setText("Temp","t.-01");
  my7SEGMENT.setText("Humid","H.-02");
  my7SEGMENT.setText("Pressure","P.-03");
  delay(2000);

  float t = ((float)random( 200, 500))/10;
  float h = random( 60, 90);
  float p = ((float)random( 100, 5000))/1000;

  
  my7SEGMENT.setNumber("Temp",  t ,1);
  my7SEGMENT.setNumber("Humid", h, 0);
  my7SEGMENT.setNumber("Pressure", p, 3);
  delay(2000);
}