/* Motor class Version 2 (ESP32)
 * Developed by Isaiah Knorr
 * 19 November 2020
 * - Minor adjustments made when switching to Drok motor driver (Similar config to L293)
 * - Added braking when speed is set to 0
 * 24 November 2020
 * - Updated to work with PWMController class
 * *** For use with the Drok 200337 Motor Drive Board ***
 * 
 * VERSION HISTORY:
 * ----------------
 * Version 1:
 * 13 August 2020
 * - Added base attrubutes and behavior
 * 15 August 2020
 * - Implemented data structure and setup function
 *   to make initialization earsier and for improved
 *   code readability
 * 27 August 2020  
 * - Implemented setSpeed function which will adjust PWM signal
 * 31 August 2020
 * - Improved class structure   
 * 
 *** This version was for use with the L293 motor driver ***
 */

 #ifndef MOTOR_H
 #define MOTOR_H

 #include "pwmcontroller.h"
 
class Motor {
  private:
    float speed; // Speed of the motor, sign indicates direction
    int PWMChannel;
    int controlWire1;
    int controlWire2;
    int calculateDutyCycle(float speed);
    PWMController pwmControl;
    
  public:
    Motor(int controlWire1,int controlWire2,int PWMChannel,int PWMWire,int PWMFrequency,int PWMResolution);
    float getSpeed();
    bool setSpeed(float speed);
 };

// Constructor
Motor::Motor(int controlWire1,int controlWire2,int PWMChannel,int PWMWire,int PWMFrequency,int PWMResolution) 
  : pwmControl(PWMChannel,PWMWire,PWMFrequency,PWMResolution) { // Constructor parameters for pwmControl
  // Set speed to 0
  speed = 0.0;
  this->controlWire1 = controlWire1;
  this->controlWire2 = controlWire2;
  this->PWMChannel = PWMChannel;
  digitalWrite(controlWire1,LOW);
  digitalWrite(controlWire2,LOW);
}

// Returns the duty cycle for the PWM signal connected to the enable pin
int Motor::calculateDutyCycle(float speed) {
  // TODO: Adjust to work with resolutions other than 8-bit
  return abs(speed)*255;
}

// Returns the current speed of the motor
float Motor::getSpeed() {
  return speed;
}

// Sets the speed of the motor, returns false if an error occured
bool Motor::setSpeed(float speed) {
  // Local vars
  int dutyCycle = 0;
  bool isForward = true;
  
  // Check for valid speed range
  if(speed < -1.0 || speed > 1.0) return false;
  
  // Update speed stored in instance
  this->speed = speed;

  // This will cause motor to brake (TODO: Slow deceleration)
  if(speed == 0) {
    digitalWrite(controlWire1,LOW);
    digitalWrite(controlWire2,LOW);
    return true;
  }

  // Determine direction
  if(speed < 0) isForward = false;
  
  // Determine duty cycle corresponding to speed
  dutyCycle = calculateDutyCycle(speed);

  // Update control pins
  if(isForward) {
    digitalWrite(controlWire1,HIGH);
    digitalWrite(controlWire2,LOW);
  }
  else {
    digitalWrite(controlWire1,LOW);
    digitalWrite(controlWire2,HIGH);
  }

  // Update PWM signal (NOTE: ESP32 for arduino doesn't support an analogWrite function, so PWMController is used)
  pwmControl.update(dutyCycle);
  
  // Update speed variable
  this->speed = speed;
  
  return true;
}
#endif
