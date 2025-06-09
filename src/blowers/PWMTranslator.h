#ifndef PWMTRANSLATOR_H
#define PWMTRANSLATOR_H

class PWMTranslator{

public:
    virtual float pwmnaarstuwkracht(int pwm)=0;
    virtual int stuwkrachtnaarpwm(float stuwkracht)=0;
};


#endif