#ifndef SIDEPWMTRANSLATOR_H
#define SIDEPWMTRANSLATOR_H
#include "PWMTranslator.h"
#include <Math.h>
#include <Arduino.h>

class SidePWMTranslator : public PWMTranslator{
    public:
    SidePWMTranslator(float a_voor, float b_voor, float c_voor, float a_achter, float b_achter, float c_achter);
    virtual float pwmnaarstuwkracht(int pwm);
    virtual int stuwkrachtnaarpwm(float stuwkracht);
    void printdebug(void);
    
    private:
    float a1,b1,c1;
    float a2,b2,c2;

    int minpwm_vooruit, maxpwm_vooruit;
    float minkracht_vooruit,maxkracht_vooruit;

    int minpwm_achteruit, maxpwm_achteruit;
    float minkracht_achteruit,maxkracht_achteruit;

    int offset_vooruit, offset_achteruit;


};


#endif