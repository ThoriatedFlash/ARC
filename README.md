# Autonomous Remote Control (ARC)
## Developed by Isaiah Knorr
 
### Version 0.1a
#### 06 September 2020
Created Servo class to crontrol servo motors for steering, camera movement, etc.
#### 31 August 2020
Improved class structure and OOP
Improved Servo functionality and auto center 
Fixed serial output screen flicker
#### 01 September 2020
Reduced servo overshoot and jitter 
 
### Known problems:
#### 1: ESP32 boot failure
Possible cause: The ESP32 is sensitive to noise in the voltage supply. The buck converter used is not of the highest quality.
This bug does not occur is the ESP32 is being powered by the micro USB line.
Possible solution: Higher quality voltage supply converter.
Temporary fix: Press reset until it boots. If problem persists, check battery voltage.
 
#### 2: Brief motor movement at boot
Possible cause: The GPIO output lines are undefined until the boot process in complete.
Possible solutions: Improved motor driver, different GPIO configuration.
 
#### 3: Low speed, overheating
Cause: Motor driver is not up to the task
Solution: Improved motor driver planned for future iteration.
 
#### 4: Minor overshoot and jitter when returning to center or maintaing center position
Possible cause: Too much delay between adjustments or inaccurate position sensor
Possible solution: Adjust timout for LCD to reduce minimum delay, or upgrade servo motor
