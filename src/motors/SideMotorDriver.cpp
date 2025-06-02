#include "SideMotorDriver.h"

SideMotorDriver::SideMotorDriver(int pinIn1, int pinIn2, int pwmPin){
    this->pin1 = pinIn1;
    this->pin2 = pinIn2;
    this->pwmPin = pwmPin;

    this->motor = new Motor(pin1, pin2, pwmPin, offset, 0);
}

void SideMotorDriver::drive(int pwm){
    this->motor->drive(pwm);
}

void SideMotorDriver::brake(){
    this->motor->brake();
}