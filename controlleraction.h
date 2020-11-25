/* Controller Action Class
 * Version 1
 * Written by Isaiah Knorr
 * 23 November 2020
 * 
 * This class processes the raw controller data and transforms reference parameters to create the desired action
 */ 

#ifndef CONTROLLER_ACTION_H
#define CONTROLLER_ACTION_H

#include "config.h"

 class ControllerAction {
  private:
  public:
    ControllerAction();
    void respond(byte controllerData,float* targetSpeed,int* steeringAngle,int* eyeAngle); 
 };

 ControllerAction::ControllerAction() {
    // Nothing needs to be done here
 }

 // This method will modify the speed, steering angle, and eye angle values based on the controller data
 void ControllerAction::respond(byte controllerData,float* currentSpeed,int* steeringAngle,int* eyeAngle) {
    float speedFactor = REGULAR_SPEED;
    float targetSpeed = 0.0;
    float targetSteeringAngle = STEERING_SERVO_HOME_ANGLE;
    float targetEyeAngle = EYE_SERVO_HOME_ANGLE;
    bool isManualEye = false;
    // Check for an unplugged controller or no buttons pressed
    switch(controllerData) {
      case 0xFF: // Controller is unplugged
      case 0x00: // No buttons are pressed
        *currentSpeed = 0.0;
        *steeringAngle = STEERING_SERVO_HOME_ANGLE;
        *eyeAngle = EYE_SERVO_HOME_ANGLE;
        return;
    }
    
    // Check if A button is pressed, for manual eye control
    if(controllerData & 0x01) {
      isManualEye = true;
      targetEyeAngle = *eyeAngle;
    }
    // Check if B button is pressed (for fast travel)
    if(controllerData & 0x02) {
      speedFactor = FAST_SPEED;
    }

    if(controllerData & 0x04) { // Select button (turns eye scan off)
      // Turn off scan
    }
    
    if(controllerData & 0x08) { // Start button (turns eye scan on)
      // Turn on scan
    }
    if(controllerData & 0x10) { // Up button (Move forward)
      targetSpeed = speedFactor;
    }
    if(controllerData & 0x20) { // Down button (Mover back)
      targetSpeed = -1.0 * speedFactor;
    }
    if(controllerData & 0x40) { // Left button (Turn left)
      if(isManualEye) {
        targetSteeringAngle = *steeringAngle;
        if(targetEyeAngle < EYE_SERVO_MAX_ANGLE) {
          targetEyeAngle = *eyeAngle + 3;
        }
      }
      else {
        targetSteeringAngle = STEERING_SERVO_MIN_ANGLE;
      }
    }
    if(controllerData & 0x80) { // Right button (Turn right)
      if(isManualEye) {
        targetSteeringAngle = *steeringAngle;
        if(targetEyeAngle > EYE_SERVO_MIN_ANGLE) {
          targetEyeAngle = *eyeAngle - 3;
        }
      }
      else {
        targetSteeringAngle = STEERING_SERVO_MAX_ANGLE;
      }
    }
    // Update values
    *currentSpeed = targetSpeed;
    *steeringAngle = targetSteeringAngle;
    *eyeAngle = targetEyeAngle;
 }

#endif
