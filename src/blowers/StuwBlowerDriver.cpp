#include "StuwBlowerDriver.h"

StuwBlowerDriver::StuwBlowerDriver(int PWMA_1, int PWMA_2){
    this->pwmPins[0] = PWMA_1;
    this->pwmPins[1] = PWMA_2;
    for(int i = 0; i < 2; i++){
        pinMode(pwmPins[i], OUTPUT);
        digitalWrite(pwmPins[i], LOW);
    }
};

int StuwBlowerDriver::drive(int pwm){

    if (pwm<0) {
        analogWrite(pwmPins[0], abs(pwm));  // Forward
        digitalWrite(pwmPins[1], LOW);
        return pwm;
    } else {
        analogWrite(pwmPins[1], abs(pwm));  // Backward
        digitalWrite(pwmPins[0], LOW);
        return pwm;
    }
};
