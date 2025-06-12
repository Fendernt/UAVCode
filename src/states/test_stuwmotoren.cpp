#include "test_stuwmotoren.h"

#include <Arduino.h>
void test_stuwmotoren(Blower& blowerR, StuwPWMTranslator& translatorR,StuwBlowerDriver& driverR,Blower& blowerL, StuwPWMTranslator& translatorL,StuwBlowerDriver& driverL){

Serial.println("Linker motor (m1):");
translatorL.printdebug();

for(int i = -250;i<=250;i=i+10){
    translatorL.pwmnaarstuwkracht_test(i);
    translatorL.stuwkrachtnaarpwm_test(translatorL.pwmnaarstuwkracht(i));
}

Serial.println("Rechter motor (m2):");
translatorR.printdebug();

for(int i = -250;i<=250;i=i+10){
    translatorR.pwmnaarstuwkracht_test(i);
    translatorR.stuwkrachtnaarpwm_test(translatorR.pwmnaarstuwkracht(i));
}
}
