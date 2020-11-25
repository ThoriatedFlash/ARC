/* Autonomous Remote Control (ARC)
   Developed by Isaiah Knorr

   Version 0.2a
   06 November 2020
   - Created Ultrasonic class to read from the HC-SR04 module
   - Upgraded to SG90 Servo motor for better control and less wires
   18 November 2020
   - Created EchoSweeper class which uses an ultrasonic sensor and a servo to scan the environment
     similar to LIDAR (not as fast or as accurate, but cheap)
   - Created PWMController class to manage all PWM signals
   23 November 2020
   - Created the ControllerAction class to determine action based on controller input
   - Moved all constants to config.h for a centralized location
   24 November 2020
   - Modified PWMController class to remove static function
   - Renamed Servo class to ServoESP32 for clairity (as not to confuse with the Servo class for Arduino)

   VERSION HISTORY
   ---------------
   Version 0.1a
   12 August 2020
   - Created Servo class to crontrol servo motors for steering, camera movement, etc.
   31 August 2020
   - Improved class structure and OOP
   - Improved Servo functionality and auto center
   - Fixed serial output screen flicker
   01 September 2020
   - Reduced servo overshoot and jitter

   KNOWN ISSUES
   ------------
   1: ESP32 boot failure
   Possible cause: The ESP32 is sensitive to noise in the voltage supply. 
   Possible solution: Create circuit to reset if boot failure detected.
   Temporary fix: Press reset until it boots. If problem persists, check battery voltage.

   2: Brief motor movement at boot
   Possible cause: The GPIO output lines are undefined until the boot process in complete.
   Possible solutions: Improved PWM controller, alternative GPIO configuration.

   3: Low speed, overheating
   Cause: New motor driver needs a heat sink
   Solution: Install heat sink

*/

#include "config.h"
#include "servoesp32.h"
#include "motor.h"
#include "controller.h"
#include "controlleraction.h"
#include "ultrasonic.h"
#include "echosweeper.h"
#include "pwmcontroller.h"

// Used to store if eye is in sweeping mode
bool isSweeping = false;

// Stores distance from ultrasonic sensor in eye to an object in front of it
float eyeDistance;

// Stores raw data received by the controller
byte controllerData;

// For movement and control of the vehicle
float targetSpeed;
int targetSteeringAngle,targetEyeAngle;

// Used to store if currently in manual mode or autonomous mode
bool isManualMode = true;

// Create Servo object
ServoESP32 steeringServo(STEERING_SERVO_MIN_ANGLE,STEERING_SERVO_MAX_ANGLE,STEERING_SERVO_HOME_ANGLE,
  STEERING_SERVO_PWM_CHANNEL,STEERING_SERVO_PWM_WIRE,STEERING_SERVO_PWM_FREQENCY,STEERING_SERVO_PWM_RESOLUTION);

// Create Motor object for the rear motor
Motor rearMotor(REAR_MOTOR_CONTROL_WIRE1,REAR_MOTOR_CONTROL_WIRE2,REAR_MOTOR_PWM_CHANNEL,REAR_MOTOR_ENABLE_WIRE,
  REAR_MOTOR_PWM_FREQENCY,REAR_MOTOR_PWM_RESOLUTION);

// Create Controller object
Controller controller(CONTROLLER_LATCH_WIRE,CONTROLLER_CLOCK_WIRE,CONTROLLER_DATA_WIRE);

// Create ControllerAction object
ControllerAction controllerAction;

// Create EchoSweeper object
EchoSweeper eye(EYE_SERVO_MIN_ANGLE,EYE_SERVO_MAX_ANGLE,EYE_SERVO_HOME_ANGLE,EYE_PWM_CHANNEL,EYE_PWM_WIRE,
  EYE_PWM_FREQENCY,EYE_PWM_RESOLUTION,EYE_TRIGGER_WIRE,EYE_ECHO_WIRE,EYE_DIVISIONS,EYE_MIN_RANGE,EYE_MAX_RANGE);

void setup() {
  // Start Serial services
  Serial.begin(SERIAL_BAUD_RATE,SERIAL_8N1,SERIAL_RX_WIRE,SERIAL_TX_WIRE);

  // Initialize pins
  pinMode(LED_WIRE,OUTPUT);
  pinMode(REAR_MOTOR_CONTROL_WIRE1,OUTPUT);
  pinMode(REAR_MOTOR_CONTROL_WIRE2,OUTPUT);
  pinMode(CONTROLLER_DATA_WIRE,INPUT);
  pinMode(CONTROLLER_LATCH_WIRE,OUTPUT);
  pinMode(CONTROLLER_CLOCK_WIRE,OUTPUT);
  pinMode(EYE_TRIGGER_WIRE,OUTPUT);
  pinMode(EYE_ECHO_WIRE,INPUT);
  eyeDistance = 0.0;
  targetSpeed = 0.0;
  targetSteeringAngle = STEERING_SERVO_HOME_ANGLE;
  targetEyeAngle = EYE_SERVO_HOME_ANGLE;
}

void loop() {
  // Check Mode, used to switch between manual and automated
  if(isManualMode) { 
    // Read raw data from controller
    controllerData = controller.getData();
    
    // Update response based on input
    controllerAction.respond(controllerData,&targetSpeed,&targetSteeringAngle,&targetEyeAngle);
    
    // Update servos and motors
    rearMotor.setSpeed(targetSpeed);
    steeringServo.setAngle(targetSteeringAngle);
    eye.setAngle(targetEyeAngle);

    // Check if sweeping (enabled when start is held down)
    if(controllerData == 0x08) {
      eyeDistance = eye.sweep();
    }
    else {
      eyeDistance = eye.getDistance();
    }
    
    // Send serial message
    Serial.println(eyeDistance);
  }
  // TODO: Autonomous mode, controlled with smart phone
}
