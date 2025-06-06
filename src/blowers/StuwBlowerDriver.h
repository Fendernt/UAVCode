#ifndef STUWBLOWERDRIVER_H
#define STUWBLOWERDRIVER_H

#include "BlowerDriver.h"
#include <Arduino.h>

class StuwBlowerDriver : public BlowerDriver{
    public:
    StuwBlowerDriver(int PWMA_1, int PWMA_2);
    virtual int drive(int pwm);
    private:
    int pwmPins[2] = {};
};

#endif