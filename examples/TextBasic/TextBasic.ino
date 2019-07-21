#include <TridentTD_7Segs74HC595.h>

#define SCLK        D5
#define RCLK        D6
#define DIO         D7

TridentTD_7Segs74HC595  my7SEGMENT( SCLK, RCLK, DIO);


void setup() {
  Serial.begin(115200);
  Serial.println();

  Serial.println(my7SEGMENT.getVersion());

  my7SEGMENT.init();
  // my7SEGMENT.init(INVERT_MODE);   // หากต้องการให้แสดงผลเรียงใหม่แบบกลับตัวเลข 4 หลักหน้าไปหลัง

  my7SEGMENT.setTextScroll("ESP-8266");

}

void loop() {
  my7SEGMENT.setText("TEST");
  delay(2000);
  my7SEGMENT.setText("  2.4");
  delay(2000);
  my7SEGMENT.setText("53.8");
  delay(2000);
  my7SEGMENT.setText("A.p.5.2");
  delay(2000);
  my7SEGMENT.setText("t.24.7");
  delay(2000);
  my7SEGMENT.setText("H.12.0");
  delay(2000);
  my7SEGMENT.setText("End.");
  delay(2000);
}
