#include "SideBlowerDriver.h"

SideBlowerDriver::SideBlowerDriver(int pinIn1, int pinIn2, int pwmPin,int pwmmin_vooruit,int pwmmax_vooruit,int pwmmin_achteruit,int pwmmax_achteruit){
    this->pin1 = pinIn1;
    this->pin2 = pinIn2;
    this->pwmPin = pwmPin;

    minpwm_v = pwmmin_vooruit;
    maxpwm_v = pwmmax_vooruit;

    minpwm_a = pwmmin_achteruit;
    maxpwm_a = pwmmax_achteruit;


    this->motor = new Motor(pin1, pin2, pwmPin, offset, 0);

}
int SideBlowerDriver::drive(int pwm){
    if(pwm>0){ //vooruit
        pwm = constrain(pwm,minpwm_v,maxpwm_v);
    }
    else{
        pwm = constrain(pwm,minpwm_a,maxpwm_a);
    }

    this->motor->drive(pwm);
    return pwm;
}

