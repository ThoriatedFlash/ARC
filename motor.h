/* Motor class Version 1
 * Developed by Isaiah Knorr
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
 ***  For use with the L293 motor driver ***
 */
 
#ifndef MOTOR_H
#define MOTOR_H

class Motor {
  private:
    float speed; // Speed of the motor, sign indicates direction
    int PWM;
    int controlWire1;
    int controlWire2;
    int calculateDutyCycle(float speed);
    
  public:
    Motor();
    Motor(int controlWire1,int controlWire2,int PWM);
    float getSpeed();
    bool setSpeed(float speed);
    bool setup(int controlWire1,int controlWire2,int PWM);
 };

// Implementation

// Constructors
Motor::Motor() {
}

Motor::Motor(int controlWire1,int controlWire2,int PWM) {
  // Set speed to 0
  speed = 0.0;
  this->controlWire1 = controlWire1;
  this->controlWire2 = controlWire2;
  this->PWM = PWM;
  digitalWrite(controlWire1,LOW);
  digitalWrite(controlWire2,LOW);
}

// Returns the duty cycle for the PWM signal connected to the enable pin
int Motor::calculateDutyCycle(float speed) {
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

  // Update PWM signal (NOTE: ESP32 for arduino doesn't support an analogWrite function)
  ledcWrite(PWM,dutyCycle);
  
  // Update speed variable
  this->speed = speed;
  
  return true;
}

// Used to setup parameters when object created using default constructor
bool Motor::setup(int controlWire1,int controlWire2,int PWM) {
  // *** TODO: Error checking ***
  // Set speed to 0
  speed = 0.0;
  this->controlWire1 = controlWire1;
  this->controlWire2 = controlWire2;
  this->PWM = PWM;
  digitalWrite(controlWire1,LOW);
  digitalWrite(controlWire2,LOW);
  return true;
}
#endif
