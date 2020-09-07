/* Controller Class Version 1
 * Developed by Isaiah Knorr
 * 29 August 2020
 * - Added base attrubutes and behavior
 * - Implemented getData() to successfully read from NES controller
 * 31 August 2020
 * - Improved class structure
 * 
 *** For use with Nintendo NES conroller or other compatible controller using 5V/3.3V ***
 *** Note: NES controller works at 3.3 or 5V, 8bitdo retro reciecer works at 5V only ***
 * 
 */
 
#ifndef CONTROLLER_H
#define CONTROLLER_H

 class Controller {
  private:
    byte data;
    int dataWire;
    int latchWire;
    int clockWire;
    
  public:
    Controller(int latchWire, int clockWire, int dataWire);
    byte getData();
 };


// Implementation

// Constructor
Controller::Controller(int latchWire, int clockWire, int dataWire) {
  this->latchWire = latchWire;
  this->clockWire = clockWire;
  this->dataWire = dataWire;
}

/* Read data from controller and return the value.
 *  The data corresponds to the following buttons:
 *  data[7:0] <--> [A,B,Select,Start,Up,Down,Left,Right]
 */
byte Controller::getData() {
  // Reset data value from last read
  data = 0x00;
  
  // Trigger the latch to cause controller to read buttons pressed
  // *** TODO: Check if a lower delay is possible to reduce delay during reads ***
  digitalWrite(latchWire,HIGH);
  delay(1);
  digitalWrite(latchWire,LOW);
  delay(1);
// Repeat 8 times to read each button
  for(int i = 0; i < 8; i++) {
    // Shift the data left
    data = data >> 1;
    
    // Read button value, switch to non inverted signal format
    if(digitalRead(dataWire) == LOW) data = data | 0x80;
    
    // Set clock high
    digitalWrite(clockWire,HIGH);
    delay(1);
    
    // Set clock low
    digitalWrite(clockWire,LOW);
    delay(1);
  }
  
  return data;
}
#endif
