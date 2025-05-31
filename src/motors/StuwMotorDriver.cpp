#include "StuwMotorDriver.h"

StuwMotorDriver::StuwMotorDriver(int PWMA_1, int PWMA_2, int PWMB_1, int PWMB_2) {
    this->pwmPins[0] = PWMA_1;
    this->pwmPins[1] = PWMA_2;
    this->pwmPins[2] = PWMB_1;
    this->pwmPins[3] = PWMB_2;

    for(int i = 0; i < 4; i++){
        pinMode(pwmPins[i], OUTPUT);
        digitalWrite(pwmPins[i], LOW);
    }
}

void StuwMotorDriver::setMotor(int motor, int speed, int dir) {
    speed = constrain(speed, 0, 255);

    switch(motor){
        case 0:
            if (dir) {
                analogWrite(pwmPins[0], speed);  // Forward
                digitalWrite(pwmPins[1], LOW);
              } else {
                analogWrite(pwmPins[1], speed);  // Backward
                digitalWrite(pwmPins[0], LOW);
              }
            break;
        case 1:
            if (dir) {
                analogWrite(pwmPins[2], speed);  // Forward
                digitalWrite(pwmPins[3], LOW);
              } else {
                analogWrite(pwmPins[3], speed);  // Backward
                digitalWrite(pwmPins[2], LOW);
              }
            break;
    }
}

void StuwMotorDriver::setMotorSpeedRechts(int pwm) {
    if(pwm < 0){
        setMotor(1, abs(pwm), false);
    } else {
        setMotor(1, pwm, true);
    }
}

void StuwMotorDriver::setMotorSpeedLinks(int pwm) {
    if(pwm < 0){
        setMotor(0, abs(pwm), false);
    } else {
        setMotor(0, pwm, true);
    }
}

int StuwMotorDriver::setMotorSpeedBeiden(int pwm) {
    setMotorSpeedRechts(pwm);
    setMotorSpeedLinks(pwm);
}