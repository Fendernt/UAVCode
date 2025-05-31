#ifndef STUWMOTORDRIVER_H
#define STUWMOTORDRIVER_H

#include <Arduino.h>

class StuwMotorDriver {
    public:
      StuwMotorDriver(int PWMA_1, int PWMA_2, int PWMB_1, int PWMB_2);
      void setMotorSpeedLinks(int pwm);
      void setMotorSpeedRechts(int pwm);
      int setMotorSpeedBeiden(int pwm);

    private:
      int pwmPins[4] = {};
      
      void setMotor(int motor, int speed, int dir);
};

#endif