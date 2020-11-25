# Autonomous Remote Control (ARC)
## Developed by Isaiah Knorr

Link to videos and pictures of the project can be found here:
https://drive.google.com/drive/folders/1AJNxVqdNGiia9y9qXp5feDVGAKEqmW58?usp=sharing

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
06 September 2020
- Created Servo class to crontrol servo motors for steering, camera movement, etc.
31 August 2020
- Improved class structure and OOP
- Improved Servo functionality and auto center 
- Fixed serial output screen flicker
01 September 2020
- Reduced servo overshoot and jitter 
 
## Known problems:
### 1: ESP32 boot failure
Possible cause: The ESP32 is sensitive to noise in the voltage supply. 
Possible solution: Create circuit to reset if boot failure detected.
Temporary fix: Press reset until it boots. If problem persists, check battery voltage.
 
### 2: Brief motor movement at boot
Possible cause: The GPIO output lines are undefined until the boot process in complete.
Possible solutions: Improved PWM controller, alternative GPIO configuration.
 
### 3: Low speed, overheating
Cause: New motor driver needs a heat sink
Solution: Install heat sink