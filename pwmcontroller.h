/* PWMController class
 * Written by Isaiah Knorr
 * 17 November, 2020
 * 
 * This class acts as a single controller of PWM signals. This was needed to overcome 
 * dificulties using the ledcWrite function in multiple classes (caused ESP32 boot loop)
 * 
 * Version 1:
 * ---------
 * 17 November 2020
 * - Created basic class structure and operation
 * - Initilizes PWM signals during instance creation
 * - Implemented updatePWM class function
 * 24 November 2020
 * - Created structure to store channel information
 * - Eliminated need for static function
 */ 

#ifndef PWMCONTROLLER_H
#define PWMCONTROLLER_H

 struct ChannelData {
  int channelNumber;
  int wireNumber;
  int frequency;
  int resolution;
 };

 class PWMController {
  private:
    ChannelData channelData;
  public:
    PWMController(int channelNumber,int wireNumber,int frequency,int resolution);
    void update(int dutyCycle);
 };

 PWMController::PWMController(int channelNumber,int wireNumber,int frequency,int resolution) {
  // TODO:
  // Check for valid and open channel number

  channelData.channelNumber = channelNumber;
  channelData.wireNumber = wireNumber;
  channelData.frequency = frequency;
  channelData.resolution = resolution;
  
  // Attach pin to channel
  ledcAttachPin(channelData.wireNumber,channelData.channelNumber);

  // Initialize channel
  ledcSetup(channelData.channelNumber,channelData.frequency,channelData.resolution);

  // Set to 0% duty cycle
  ledcWrite(channelData.channelNumber,0);
 }

 void PWMController::update(int dutyCycle) {
  ledcWrite(channelData.channelNumber,dutyCycle);
 }
#endif
