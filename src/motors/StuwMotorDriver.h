#ifndef STUWMOTORDRIVER_H
#define STUWMOTORDRIVER_H

#include <Arduino.h>

class StuwMotorDriver {
    public:
      StuwMotorDriver(int PWMA_1, int PWMA_2, int PWMB_1, int PWMB_2);

      //Snelheid linker motor
      void setMotorSpeedLinks(int pwm);

      //Snelheid rechter motor.
      void setMotorSpeedRechts(int pwm);

      //Beide motors.
      int setMotorSpeedBeiden(int pwm);

    private:
      int pwmPins[4] = {};
      

      //Private functie om een motor snelheid te zetten.
      void setMotor(int motor, int speed, int dir);
};

#endif