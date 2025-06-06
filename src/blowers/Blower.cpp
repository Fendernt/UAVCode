#include "Blower.h"



Blower::Blower(BlowerDriver * blowerDriver, PWMTranslator * PWMTranslator)
{
    this->blowerDriver = blowerDriver;
    this->translator = PWMTranslator;
}

void Blower::leverkracht(float stuwkracht)
{
    blowerDriver->drive(translator->stuwkrachtnaarpwm(stuwkracht));
};