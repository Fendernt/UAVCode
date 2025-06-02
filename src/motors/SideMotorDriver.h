#ifndef SIDEMOTORDRIVER_H
#define SIDEMOTORDRIVER_H

#include "../externalLibraries/SparkFun_TB6612.h"

class SideMotorDriver {
    public:
        SideMotorDriver(int pinIn1, int pinIn2, int pwmPin);

        //Snelheid, negatief is de andere kant op.
        void drive(int pwm);

        //Afremmen \ stoppen.
        void brake();

    private:
        Motor* motor;
        int pin1;
        int pin2;
        int pwmPin;

        int offset = 1;

        int pwm = 0;


};


#endif