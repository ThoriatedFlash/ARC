/* Easy Ultrasonic Reader
 * Written by Isaiah Knorr
 * 21 October, 2020
 * 
 * For use with the HC-SR04 Ultrasonic module
 */ 

 #ifndef ULTRASONIC_H
 #define ULTRASONIC_H

 class Ultrasonic {
  private:
    int triggerPin;
    int echoPin;
  public: 
    Ultrasonic();
    Ultrasonic(int triggerPin,int echoPin);
    double getDistance();
    double getDistance(int repeatNumber);
 };

 // Constructors
  Ultrasonic::Ultrasonic() {
 }
 
 Ultrasonic::Ultrasonic(int triggerPin,int echoPin) {
  this->triggerPin = triggerPin;
  this->echoPin = echoPin;
 }
 
 // Returns the distance to the sensor in inches
 double Ultrasonic::getDistance() {
  // Set trigger high for 10 micro seconds
  digitalWrite(triggerPin,HIGH);
  delayMicroseconds(10);
  digitalWrite(triggerPin,LOW);
  
  // See datasheet to see where this conversion came from - was refined based on tests for increaced accuracy
  double distance = (pulseIn(echoPin,HIGH)/ 146.591) - 0.088;

  // Check set error value of -1 if out of sensor range
  if(distance > 400) 
    return -1.0; 
    
  // Return average value
  return distance;
 } 
#endif
 
 
