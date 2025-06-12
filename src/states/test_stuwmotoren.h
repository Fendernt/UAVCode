#ifndef TEST_STUWMOTOREN_H
#define TEST_STUWMOTOREN_H

#include "../blowers/Blower.h"
#include "../blowers/BlowerDriver.h"
#include "../blowers/StuwBlowerDriver.h"

#include <Arduino.h>

void test_stuwmotoren(Blower& blowerR, StuwPWMTranslator& translatorR,StuwBlowerDriver& driverR,Blower& blowerL, StuwPWMTranslator& translatorL,StuwBlowerDriver& driverL);


#endif