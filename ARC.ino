/* Autonomous Remote Control (ARC)
 * Developed by Isaiah Knorr
 * 
 * Version 0.1a
 * 12 August 2020
 * - Created Servo class to crontrol servo motors for steering, camera movement, etc.
 * 31 August 2020
 * - Improved class structure and OOP
 * - Improved Servo functionality and auto center 
 * - Fixed serial output screen flicker
 * 01 September 2020
 * - Reduced servo overshoot and jitter 
 * 
 * Known problems:
 * 1: ESP32 boot failure
 * Possible cause: The ESP32 is sensitive to noise in the voltage supply. The buck converter used is not of the highest quality.
 * This bug does not occur is the ESP32 is being powered by the micro USB line.
 * Possible solution: Higher quality voltage supply converter.
 * Temporary fix: Press reset until it boots. If problem persists, check battery voltage.
 * 
 * 2: Brief motor movement at boot
 * Possible cause: The GPIO output lines are undefined until the boot process in complete.
 * Possible solutions: Improved motor driver, different GPIO configuration.
 * 
 * 3: Low speed, overheating
 * Cause: Motor driver is not up to the task
 * Solution: Improved motor driver planned for future iteration.
 * 
 * 4: Minor overshoot and jitter when returning to center or maintaing center position
 * Possible cause: Too much delay between adjustments or inaccurate position sensor
 * Possible solution: Adjust timout for LCD to reduce minimum delay, or upgrade servo motor
 * 
 */

#include "servo.h"
#include "motor.h"
#include "controller.h"

// Define constants / pin assignments
// For servo and servo motor
const int MIN_ANGLE = -2;
const int MAX_ANGLE = 2;
const int HOME_ANGLE = 0;
const int WHITE_SERVO_WIRE = 25;
const int PURPLE_SERVO_WIRE = 32;
const int BLUE_SERVO_WIRE = 35;
const int GREEN_SERVO_WIRE = 34;
const int SERVO_MOTOR_PWM_CHANNEL = 0;
const int SERVO_MOTOR_PWM_FREQENCY = 5000;
const int SERVO_MOTOR_PWM_RESOLUTION = 8;
const int SERVO_MOTOR_ENABLE_WIRE =17; 
const int SERVO_MOTOR_CONTROL1 =  4;
const int SERVO_MOTOR_CONTROL2 = 16; 

// For rear motor
const int REAR_MOTOR_PWM_CHANNEL = 1;
const int REAR_MOTOR_PWM_FREQENCY = 5000;
const int REAR_MOTOR_PWM_RESOLUTION = 8;
const int REAR_MOTOR_ENABLE_WIRE =19; 
const int REAR_MOTOR_CONTROL1 =  5;
const int REAR_MOTOR_CONTROL2 = 18; 

// For controller
const int CONTROLLER_DATA_WIRE = 26;
const int CONTROLLER_LATCH_WIRE = 27;
const int CONTROLLER_CLOCK_WIRE = 14;

// For Serial
const int SERIAL_TX_WIRE = 13;
const int SERIAL_RX_WIRE = 2;
const int SERIAL_BAUD_RATE = 57600;

// For LED on the ESP32
const int LED_PIN = 2;

// Create Servo object
Servo steeringServo(MIN_ANGLE,MAX_ANGLE,HOME_ANGLE,WHITE_SERVO_WIRE,PURPLE_SERVO_WIRE,BLUE_SERVO_WIRE,GREEN_SERVO_WIRE,SERVO_MOTOR_CONTROL1,SERVO_MOTOR_CONTROL2,SERVO_MOTOR_PWM_CHANNEL);

// Create Motor object for the rear motor
Motor rearMotor(REAR_MOTOR_CONTROL1,REAR_MOTOR_CONTROL2,REAR_MOTOR_PWM_CHANNEL);

// Create Controller object
Controller controller(CONTROLLER_LATCH_WIRE,CONTROLLER_CLOCK_WIRE,CONTROLLER_DATA_WIRE);

void setup() {
  // Start Serial services
  Serial.begin(SERIAL_BAUD_RATE,SERIAL_8N1,SERIAL_RX_WIRE,SERIAL_TX_WIRE);

  // Initialize pins
  pinMode(LED_PIN, OUTPUT);
  pinMode(WHITE_SERVO_WIRE, OUTPUT);
  pinMode(PURPLE_SERVO_WIRE, OUTPUT);
  pinMode(BLUE_SERVO_WIRE, INPUT);
  pinMode(GREEN_SERVO_WIRE, INPUT);
  pinMode(SERVO_MOTOR_CONTROL1,OUTPUT);
  pinMode(SERVO_MOTOR_CONTROL2,OUTPUT);
  pinMode(REAR_MOTOR_CONTROL1,OUTPUT);
  pinMode(REAR_MOTOR_CONTROL2,OUTPUT);
  pinMode(CONTROLLER_DATA_WIRE,INPUT);
  pinMode(CONTROLLER_LATCH_WIRE,OUTPUT);
  pinMode(CONTROLLER_CLOCK_WIRE,OUTPUT);

  //Initialize PWM for servo motor
  ledcSetup(SERVO_MOTOR_PWM_CHANNEL,SERVO_MOTOR_PWM_FREQENCY,SERVO_MOTOR_PWM_RESOLUTION);
  ledcAttachPin(SERVO_MOTOR_ENABLE_WIRE,SERVO_MOTOR_PWM_CHANNEL);
  ledcWrite(SERVO_MOTOR_PWM_CHANNEL,0);

  // Initialize PWM for rear motor
  ledcSetup(REAR_MOTOR_PWM_CHANNEL,REAR_MOTOR_PWM_FREQENCY,REAR_MOTOR_PWM_RESOLUTION);
  ledcAttachPin(REAR_MOTOR_ENABLE_WIRE,REAR_MOTOR_PWM_CHANNEL);
  ledcWrite(REAR_MOTOR_PWM_CHANNEL,0);  
}

void loop() {
  controllerAction();
}

void controllerAction() {
  byte data = controller.getData();
  String serialMessage = "";
  switch(data) {
    case 0x00: // No buttons pressed
      rearMotor.setSpeed(0.0);
      steeringServo.setAngle(0);
      serialMessage = "Stop";
      break;
    case 0x10: // Up button pressed
      rearMotor.setSpeed(1.0);
      steeringServo.setAngle(0);
      serialMessage = "Forward";
      break;
    case 0x20: // Down button pressed
      rearMotor.setSpeed(-1.0);
      steeringServo.setAngle(0);
      serialMessage = "Reverse";
      break;
    case 0x40: // Left button pressed    
      steeringServo.setAngle(-2);
      serialMessage = "Left";
      break;
    case 0x80: // Right button pressed
      steeringServo.setAngle(2);
      serialMessage = "Right";
      break;
    case 0x50: // Up and left buttons pressed
      steeringServo.setAngle(-2);
      rearMotor.setSpeed(1.0);
      serialMessage = "Fd Left";
      break;
    case 0x60: // Down and left buttons pressed
      steeringServo.setAngle(-2);
      rearMotor.setSpeed(-1.0);
      serialMessage = "Rv Left";
      break;
    case 0x90: // Up and right buttons pressed
      steeringServo.setAngle(2);
      rearMotor.setSpeed(1.0);
      serialMessage = "Fd Right";
      break;
    case 0xA0: // Down and right buttons pressed
      steeringServo.setAngle(2);
      rearMotor.setSpeed(-1.0);
      serialMessage = "Rv Right";
      break;
    case 0xFF:
      rearMotor.setSpeed(0.0);
      serialMessage = "No Input";
      break;
    default:
      serialMessage = "Not Set";
  }
  Serial.println(serialMessage);
}
