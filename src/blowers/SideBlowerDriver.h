#ifndef SIDEBLOWERDRIVER_H
#define SIDEBLOWERDRIVER_H

#include "BlowerDriver.h"
#include "../externalLibraries/SparkFun_TB6612.h"

class SideBlowerDriver : public BlowerDriver{
    public:
    SideBlowerDriver(int pinIn1, int pinIn2, int pwmPin,int pwmmin_vooruit,int pwmmax_vooruit,int pwmmin_achteruit,int pwmmax_achteruit);
    //Snelheid, negatief is de andere kant op.
    virtual int drive(int pwm);

    
    private:
        Motor* motor;
        int pin1;
        int pin2;
        int pwmPin;

        int offset = 1;

        int pwm = 0;

        int maxpwm_v;
        int minpwm_v;

        int maxpwm_a;
        int minpwm_a;

};

#endif