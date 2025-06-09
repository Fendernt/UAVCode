#ifndef STUWPWMTRANSLATOR_H
#define STUWPWMTRANSLATOR_H
#include"PWMTranslator.h"
#include <Math.h>
#include <Arduino.h>

class StuwPWMTranslator: public PWMTranslator{
    public:
    StuwPWMTranslator(float a_voor, float b_voor, float c_voor, float a_achter, float b_achter, float c_achter);
    virtual float pwmnaarstuwkracht(int pwm);
    virtual int stuwkrachtnaarpwm(float stuwkracht);
    
    private:
    float a1,b1,c1;
    float a2,b2,c2;

    float maxkracht_vooruit;
    float maxkracht_achteruit;

};


#endif