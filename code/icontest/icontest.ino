#include <Wire.h>
#include "ssd1306_i2c.h"
#include "icons.h"

// Initialize the oled display for address 0x3c
// sda-pin=14 and sdc-pin=12
SSD1306 display(0x3c, 8, 9);

void setup() {
  // initialize display
  display.init();
  display.clear();
  display.display();
}

void loop() {
  display.flipScreenVertically();
  display.drawXbm(0,0, 50, 50,cloudy_bits);
  display.setFontScale2x2(true);
  display.drawString( 70, 0, "53F");  
  //display.drawString( 105, 0, "F");  
  display.drawString( 70, 20, "39%");  
  display.setFontScale2x2(false);
  //display.drawString(25, 53, "3 min ago");
  display.drawString(20, 55, "42 F | 61 F");
  
  display.display();
  delay(5000);

}
