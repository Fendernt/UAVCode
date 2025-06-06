#ifndef PWMTRANSLATOR_H
#define PWMTRANSLATOR_H
#include <Math.h>
#include <Arduino.h>

class PWMTranslator{
    public:
    PWMTranslator(float a_voor, float b_voor, float c_voor, float a_achter, float b_achter, float c_achter);
    float pwmnaarsuwkracht(int pwm);
    int stuwkrachtnaarpwm(float stuwkracht);
    
    private:
    float a1,b1,c1;
    float a2,b2,c2;

    float maxkracht_vooruit;
    float maxkracht_achteruit;

};


#endif