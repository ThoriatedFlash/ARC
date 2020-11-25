/* Configuration file for Autonomous Remote Control
 *  
 *  Some constants needed to be accessed by the top level and sub-classes in some cases,
 *  so all of the constants used for the project are now placed here.
 *  This should prevent errors caused by changing a constant in one file but not the other
 *  and will prevent duplicating data in class instances.
 */

 #ifndef CONFIG_H
 #define CONFIG_H

// For rear motor
const int REAR_MOTOR_CONTROL_WIRE1 =  4;
const int REAR_MOTOR_CONTROL_WIRE2 = 16;

// TODO: SWITCH TO INSTANCE BASED FOR PWM
const int REAR_MOTOR_ENABLE_WIRE =17; 
const int REAR_MOTOR_PWM_CHANNEL = 0; 
const int REAR_MOTOR_PWM_FREQENCY = 5000;
const int REAR_MOTOR_PWM_RESOLUTION = 8;

// For steering servo
const int STEERING_SERVO_MIN_ANGLE = 0;
const int STEERING_SERVO_MAX_ANGLE = 180;
const int STEERING_SERVO_HOME_ANGLE = 90;

// TODO: SWITCH TO INSTANCE BASED FOR PWM
const int STEERING_SERVO_PWM_WIRE = 25;
const int STEERING_SERVO_PWM_CHANNEL = 2;
const int STEERING_SERVO_PWM_FREQENCY = 50;
const int STEERING_SERVO_PWM_RESOLUTION = 12;
 
// For controller / controllerAction()
const int CONTROLLER_DATA_WIRE = 26;
const int CONTROLLER_LATCH_WIRE = 27;
const int CONTROLLER_CLOCK_WIRE = 14;

// For Serial communication
const int SERIAL_TX_WIRE = 13;
const int SERIAL_RX_WIRE = 2;
const int SERIAL_BAUD_RATE = 115200;

// For LED on the ESP32
const int LED_WIRE = 2;

// For eye
const int EYE_TRIGGER_WIRE = 5;
const int EYE_ECHO_WIRE = 18;
const int EYE_SERVO_MIN_ANGLE = 25;
const int EYE_SERVO_MAX_ANGLE = 155;
const int EYE_SERVO_HOME_ANGLE = 90;
const int EYE_DIVISIONS = 25;
const int EYE_MIN_RANGE = 12;
const int EYE_MAX_RANGE = 60;

// TODO: SWITCH TO INSTANCE BASED FOR PWM
const int EYE_PWM_WIRE = 33;
const int EYE_PWM_CHANNEL = 4;
const int EYE_PWM_FREQENCY = 50;
const int EYE_PWM_RESOLUTION = 12;

// For ControllerAction
const float REGULAR_SPEED = 0.5;
const float FAST_SPEED = 1.0;
  
 #endif
