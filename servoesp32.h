/* ServoESP32 Class Version 2 (For use with the ESP32)
 *  09 November 2020
 *  - Updated for use with the SG90 servo and removed obsolete code
 *  24 November 2020
 *  - Updated to support PWMController class
 *  - Renamed class to ServoESP32 
 *  
 *  VERSION HISTORY
 * ---------------
 * Servo Class Version 1
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
 * Version 1 was for use with original RC truck servo motor of unknown specification
 */
 
 #ifndef SERVOESP32_H
 #define SERVOESP32_H

 #include "pwmcontroller.h"

// Servo class definition
class ServoESP32 {
  private:
    int currentAngle; // The angle the servo is currently
    int minAngle; // The minimum angle the servo can travel
    int maxAngle; // The maximum angle the servo can travel
    int homeAngle; // The angle considered to be home
    int PWMChannel; // The PWM signal that controls the servo angle
    PWMController pwmControl;
    
  public:
    ServoESP32(int minAngle,int maxAngle,int homeAngle,int PWMChannel,int PWMWire,int PWMFrequency,int PWMResolution);
    int calculateDutyCycle(int angle); // Calculate PWM duty cycle for servo angle
    int getAngle();
    bool goHome();
    bool setAngle(int desiredAngle); 
    
};

// Implementation

// Constructor
ServoESP32::ServoESP32(int minAngle,int maxAngle,int homeAngle,int PWMChannel,int PWMWire,int PWMFrequency,int PWMResolution)
  : pwmControl(PWMChannel,PWMWire,PWMFrequency,PWMResolution) {
  // Load vaules for this servo instance
  this->minAngle = minAngle;
  this->maxAngle = maxAngle;
  this->homeAngle = homeAngle;
  this->currentAngle = homeAngle;
  this->PWMChannel = PWMChannel;
  
  // Go to home position
  goHome();
}

// Calculates the PWM duty cycle for a given input angle
int ServoESP32::calculateDutyCycle(int angle) {
 
  /* 0 degrees = 1ms = 5% duty cycle -> 180 degrees = 2ms = 10% duty cycle (See datasheet for more info)
   *  NOTE: 2^12 = 4096, for 12 bit resolution
   *  TODO: Adjust to work with resolutions other than 12-bit
   */ 

   double dutyCycle = (1.0/3600.0)*angle + 0.05;
   return 4096*dutyCycle;
}

// Returns the current angle of the servo
int ServoESP32::getAngle() {
  return currentAngle;
}

// Goes to the home position
bool ServoESP32::goHome() {
  setAngle(homeAngle);
  return true;
}

//Sets the angle of the servo motor. The return value is false if an error has occured
// Note: Only sets currentAngle if valid, actual PWM is set from main loop (See below)
bool ServoESP32::setAngle(int desiredAngle) {
  // Check that desired angle is within range of servo motor
  if(desiredAngle < minAngle || desiredAngle > maxAngle) {
    return false;
  }
  
   pwmControl.update(calculateDutyCycle(desiredAngle));
  
  // Update currentAngle
  currentAngle = desiredAngle;
  return true;
}

#endif
