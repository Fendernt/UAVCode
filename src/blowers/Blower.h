#ifndef BLOWER_H
#define BLOWER_H

#include "PWMTranslator.h"
#include "SidePWMTranslator.h"
#include "StuwPWMTranslator.h"
#include "SideBlowerDriver.h"
#include "StuwBlowerDriver.h"
#include "BlowerDriver.h"

class Blower{
    public:
        Blower(BlowerDriver *blowerDriver, PWMTranslator *PWMTranslator);

        void leverkracht(float stuurkracht);

    private:
        PWMTranslator* translator;
        BlowerDriver* blowerDriver;

};

#endif