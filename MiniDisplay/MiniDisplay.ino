/* MiniDisplay Version 1.0
 * Written by Isaiah Knorr
 * 
 * 06 Septemeber 2020
 * - Imported code from previous arduino project
*/

#include <Wire.h> // This one is needed for I2C
#include <Adafruit_GFX.h> // Used for graphics
#include <Adafruit_SSD1306.h> // Needed for the 1306 OLED

#define WIDTH 128 // Screen width
#define HEIGHT 32 // Screen height
#define RESET 4 // Screen reset pin
#define SWITCH 1
// For keypad
#define UP 0
#define DOWN 1
#define LEFT 2
#define RIGHT 3
#define SELECT 4
#define NONE 5

int xpos = 0;
int ypos = 0;
int dy = 0;
int dx = 2;
int button = 0; // A0
int key; // Stores key pressed
int last = NONE;
int pressCounter = 0;
boolean longPress = false;
const int LONG_PRESS_THRESHOLD = 20;
// Create instance of Adafruit_SSD1306
Adafruit_SSD1306 oled(WIDTH,HEIGHT, &Wire, RESET);
// NOTE: &Wire = address to I2C?
static const unsigned char PROGMEM car_bmp[] =
{ 0, 0, 1, 255, 0, 0, 0, 0, 0, 0, 31, 255, 128, 0, 0, 0, 
0, 0, 255, 255, 192, 0, 0, 0, 0, 3, 255, 255, 192, 0, 0, 0, 
0, 15, 255, 7, 192, 0, 0, 0, 0, 31, 240, 7, 192, 0, 0, 0, 
0, 127, 128, 7, 252, 0, 0, 0, 0, 254, 0, 7, 255, 252, 0, 0, 
1, 252, 0, 7, 255, 255, 192, 0, 3, 240, 0, 3, 255, 255, 248, 0, 
7, 224, 0, 0, 255, 255, 255, 0, 31, 192, 0, 0, 0, 7, 255, 192, 
63, 128, 0, 0, 0, 0, 127, 240, 127, 0, 127, 223, 231, 248, 15, 252, 
254, 0, 127, 223, 247, 252, 1, 254, 252, 0, 127, 223, 247, 252, 0, 127, 
252, 0, 63, 159, 231, 248, 0, 31, 248, 0, 0, 0, 0, 0, 0, 15, 
240, 31, 128, 0, 0, 0, 124, 15, 240, 127, 224, 0, 0, 1, 255, 15, 
240, 255, 240, 0, 0, 7, 255, 143, 241, 255, 248, 0, 0, 7, 255, 207, 
249, 240, 248, 0, 0, 15, 199, 239, 255, 224, 127, 255, 255, 255, 3, 255, 
127, 224, 127, 255, 255, 255, 1, 255, 63, 224, 63, 255, 255, 255, 1, 254, 
31, 224, 127, 255, 255, 255, 3, 252, 1, 240, 252, 0, 0, 15, 131, 224, 
1, 255, 248, 0, 0, 15, 255, 192, 0, 255, 240, 0, 0, 7, 255, 192, 
0, 127, 224, 0, 0, 3, 255, 0, 0, 31, 128, 0, 0, 0, 254, 0 };

// This function determines the key pressed
int getKey(int buttonPressed) {
  if (buttonPressed > 1000) return NONE; 
  if (buttonPressed < 50 && buttonPressed > 30)  return RIGHT;
  if (buttonPressed < 195 && buttonPressed > 100) return UP;
  if (buttonPressed < 380 && buttonPressed > 300) return DOWN;
  if (buttonPressed < 555 && buttonPressed > 400) return LEFT;
  if (buttonPressed < 790 && buttonPressed > 700) return SELECT;
}

void setup() {
  Serial.begin(115200);
  Serial.setTimeout(50);
  while(!oled.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3C for 128x32
    Serial.println(F("SSD1306 allocation failed"));
  }
  oled.clearDisplay(); // clears the screen
  oled.display();
  oled.setCursor(0,0);
  oled.setTextSize(1);
  oled.setTextColor(WHITE);
  oled.println("Autonomous Remote ");
  oled.println("Control v0.1a");
  oled.println("by Isaiah Knorr");
  oled.println("iknorr@asu.edu");
  oled.display();
  delay(3000);
}

void loop() {
  key = getKey(analogRead(button));
  if(key != last || longPress) {
    if(key != last) {
      last = key;
      longPress = false;
      pressCounter = 0;
    }
    switch(key) {
      case UP:
      dy--;
      if(ypos < -32) ypos = 32;
      delay(10);
      break;
      case DOWN:
      dy++;
      if(ypos > 32) ypos = -32;
      delay(10);
      break;
      case LEFT:
      dx--;
      if(xpos < -64) xpos = 128;
      delay(10);
      break;
      case RIGHT:
      dx++;
      if(xpos > 128) xpos = -64;
      delay(10);
      break;
      case SELECT:
      xpos = 32;
      ypos = 0;
      dx = 0;
      dy = 0;
      break;
      default:
      pressCounter = 0;
      longPress = false;
    }
  }
  else {
    if(key != NONE) pressCounter++;
    if(pressCounter >= LONG_PRESS_THRESHOLD)
      longPress = true;
  }
  if(analogRead(SWITCH) > 675) {
    xpos += dx;
    ypos += dy;
    if(xpos >128) {
      xpos = -64;
      if(rand() % (20 * dx) == 1) {
        for(int i = 0; i < 10; i++) {
          oled.clearDisplay();
          oled.setTextSize(2);
          oled.setCursor(0,0);
          if(i % 2 == 0) 
            oled.setTextColor(BLACK, WHITE);
          else
            oled.setTextColor(WHITE);
          oled.println("  Project  ");
          oled.println("    ARC    ");
          oled.display();
          delay(100);
        }
      }
    }
     if(xpos < -64) xpos = 128;
     if(ypos < -32) ypos = 32;
     if(ypos > 32) ypos = -32;
     oled.clearDisplay();
    oled.drawBitmap(xpos,ypos, car_bmp, 64, 32, 1);
  }
  else {
    if(Serial.available()) oled.clearDisplay();
    oled.setCursor(0,0);
    oled.setTextSize(1);
    while(Serial.available()) { 
      oled.println(Serial.readString());
    }
  }
  oled.display();
}
