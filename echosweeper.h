/* EchoSweeper (Eye) class
 * Written by Isaiah Knorr
 * 17 November, 2020
 * 
 * This class defines an EchoSweeper object, which will act as a cheap alternative to 
 * a LIDAR unit. However, the accuracy and speed are much lower than LIDAR. This will 
 * act as the basis for basic obstacle avoidance and object tracking.
 * 
 * Version 1:
 * ---------
 * - Created basic class structure and operation
 * - Implemented the getAngle and setAngle functions
 * - Implemented the sweep function, which returns the average distance of a sweep
 * TODO: Store all values of sweep in an array and return this value (may need to create a struct)
 * For use with the HC-SR04 Ultrasonic module and SG90 servo motor
 */ 

#ifndef ECHOSWEEPER_H
#define ECHOSWEEPER_H

#include "ultrasonic.h"
#include "servoesp32.h"

 class EchoSweeper {

  private:
    int minAngle;
    int maxAngle;
    int minRange;
    int maxRange;
    int divisions;
    double averageDistance;
    bool isForward;
    ServoESP32 servoMotor;
    Ultrasonic ultrasonicSensor;   

  public:
    EchoSweeper(int minAngle,int maxAngle,int homeAngle,int PWMChannel,int PWMWire,int PWMFrequency,int PWMResolution,int triggerPin,int echoPin,int divisions,int minRange,int maxRange);
    double sweep();
    double getDistance();
    bool setAngle(int angle);
    int getAngle();
  
 };

 // Constructor
 EchoSweeper::EchoSweeper(int minAngle,int maxAngle,int homeAngle,int PWMChannel,int PWMWire,int PWMFrequency,int PWMResolution,int triggerPin,int echoPin,int divisions,int minRange,int maxRange)
    : servoMotor(minAngle,maxAngle,homeAngle,PWMChannel,PWMWire,PWMFrequency,PWMResolution), ultrasonicSensor(triggerPin,echoPin) // This is the initilization list for the sub classes of EchoSweeper
  {
  //Servo servoMotor(minAngle,maxAngle,homeAngle,PWMChannel);
  this->minAngle = minAngle;
  this->maxAngle = maxAngle;
  this->minRange = minRange;
  this->maxRange = maxRange;
  this->divisions = divisions;
  averageDistance = 0;
 }

 // The sweep function will sweep between two angles, reading the distance at each position
 double EchoSweeper::sweep() {
  // TODO: check for valid ranges
  double currentDistance = 0;
  int hitCount = 0;
  // When sweeping from the min to max angle
  if(isForward) {
    for(int i = minAngle; i < maxAngle; i = i +((maxAngle - minAngle)/divisions)) {
      // Set angle for current reading
      servoMotor.setAngle(i);
      delay(10);
      currentDistance  = ultrasonicSensor.getDistance();
      // Check for a read in range being observed
      if(currentDistance >= minRange && currentDistance <= maxRange) {
        hitCount++;
        averageDistance += currentDistance;
      }
    }
  }
  // When sweeping from max to min angle
  else {
    // Split the sweep up into pieces, based on the # of divisions
    for(int i = maxAngle; i > minAngle; i = i - ((maxAngle - minAngle)/divisions)) {
      // Set angle for current reading
      servoMotor.setAngle(i);
      delay(10);
      currentDistance  = ultrasonicSensor.getDistance();
      // Check for a read in range being observed
      if(currentDistance >= minRange && currentDistance <= maxRange) {
        hitCount++;
        averageDistance += currentDistance;
      }
    }
  }
  // Swap sweep direction for next sweep (this creates the back and forth motion)
  isForward = !isForward;
  
  // Compute average distance
  if(hitCount > 0) {
    averageDistance = averageDistance / hitCount;
  }
  else {
    averageDistance = -1; // This indicates no object detected in range
  }
    return averageDistance;
 }

// Returns the distance of an object to the ultrasonic sensor
 double EchoSweeper::getDistance() {
  //servoMotor.goHome();
  return ultrasonicSensor.getDistance();
 }

// Returns the current angle of the servo
int EchoSweeper::getAngle() {
  return servoMotor.getAngle();
}

// Sets the angle of the servo
bool EchoSweeper::setAngle(int angle) {
  // Check for valid angle
  if(angle > maxAngle || angle < minAngle) {
    return false;
  }
  servoMotor.setAngle(angle);
  return true;
}
#endif
