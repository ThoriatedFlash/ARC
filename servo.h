/* Servo Class Version 1
 * Developed by Isaiah Knorr
 * 12 August 2020
 * - Added base attrubutes and behavior
 * 17 August 2020
 * - Implemented data structure and setup function
 * 31 August 2020
 * - Improved class structure
 * - Encapsulated the servoMotor inside of the servo class
 * - Implemented setAngle and goHome functions
 * 
 * For use with original RC truck servo motor of unknown specification
 */
 
#ifndef SERVO_H
#define SERVO_H
#include "motor.h" // Needed for servo motor

// Servo class definition
class Servo {
  private:
    int currentAngle; // The angle the servo is currently
    int minAngle; // The minimum angle the servo can travel
    int maxAngle; // The maximum angle the servo can travel
    int homeAngle; // The angle considered to be home
    int whiteWire;
    int purpleWire;
    int blueWire;
    int greenWire;
    int angleConfirmCount;
    Motor servoMotor;
    
  public:
    Servo(int minAngle,int maxAngle,int homeAngle,int whiteWire,int purpleWire,int blueWire,int greenWire,int controlWire1,int controlWire2,int PWM);
    int getAngle();
    bool goHome();
    bool setAngle(int desiredAngle); 
};

// Implementation

// Constructor
Servo::Servo(int minAngle,int maxAngle,int homeAngle,int whiteWire,int purpleWire,int blueWire,int greenWire,int controlWire1,int controlWire2,int PWM) {
  angleConfirmCount = 0;
  this->minAngle = minAngle;
  this->maxAngle = maxAngle;
  this->homeAngle = homeAngle;
  this->currentAngle = homeAngle;
  this->whiteWire = whiteWire;
  this->purpleWire = purpleWire;
  this->blueWire = blueWire;
  this->greenWire = greenWire;
  servoMotor.setup(controlWire1,controlWire2,PWM);
}

// Returns the current angle of the servo
int Servo::getAngle() {
  byte data = 0x00;
  String dir  = "";
  // Reset input pins
  digitalWrite(whiteWire,LOW);
  digitalWrite(purpleWire,LOW);
  
  // Set input1 high and read
  digitalWrite(whiteWire,HIGH);
  if(digitalRead(blueWire)) data = data | 0x01;
  if(digitalRead(greenWire)) data = data | 0x02;

  // Set input2 high and read
  digitalWrite(whiteWire,LOW);
  digitalWrite(purpleWire,HIGH);
  if(digitalRead(blueWire)) data = data | 0x04;
  if(digitalRead(greenWire)) data = data | 0x08;

  // Reset input pins
  digitalWrite(whiteWire,LOW);
  digitalWrite(purpleWire,LOW);

  switch(data) {
    case 0x00:
      dir = "Center1";
      currentAngle = 0;
      break;
    case 0x02: 
      dir = "S Right";
      currentAngle = 1;
      break;
    case 0x04:
      dir = "Center2";
      currentAngle = 0;
      break;
    case 0x06:
      dir = "R Edge";
      currentAngle = 2;
      break;
    case 0x0C:
      dir = "S Left";
      currentAngle = -1;
      break;
    case 0x09:
      dir = "L Edge";
      currentAngle = -2;
      break;
    default:
      currentAngle = -3;
      dir = "Error";
  }

  Serial.println(dir);
  return currentAngle;
}

// Goes to the home position
bool Servo::goHome() {
  // TODO:
  return true;
}

//Sets the angle of the servo motor. The return value is false if an error has occured
bool Servo::setAngle(int desiredAngle) {
  // The motor delay will be based on how far to the desired angle
  int difference = abs(currentAngle - desiredAngle);
  int motorDelay = 20*difference;
  // Check that desired angle is within range of servo motor
  if(desiredAngle < minAngle || desiredAngle > maxAngle) {
    return false;
  }
  
  // Check for read error (TODO: See if this did anything)
  if(currentAngle < minAngle) {
    // Read again
    getAngle();
    return false;
  }
  // Check if the servo is in the correct position
  if(currentAngle == desiredAngle) {
    servoMotor.setSpeed(0.0);
    delay(100);
    // The angle will stop being read after 3 consecutive checks
    // This is done to prevent jitter while at the center position
    if(angleConfirmCount < 3) {
      getAngle();
      angleConfirmCount++;
    }
    return true;
  }
  // Reset because the desired angle has not been confirmed
  angleConfirmCount = 0;
  // Move servo to reach desired angle, with speed proportional to distance to go
  if(currentAngle < desiredAngle) {
    // Move right in this case
    servoMotor.setSpeed(0.85 + 0.0375*difference);
  }
  else {
   // Move left
    servoMotor.setSpeed(-0.85 - 0.0375*difference);
  }
  // Let motor run for a bit based on angle difference
  delay(motorDelay);
  servoMotor.setSpeed(0.0);
  // Update currentAngle
  getAngle();
  return false;
}
#endif
