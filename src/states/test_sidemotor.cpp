#include "test_sidemotor.h"

void test_sidemotor(Blower& sideblower, SidePWMTranslator& translator,SideBlowerDriver& sideblowerdriver) {

    Serial.println("Test sidemotor!");

/*
vooruit:
drive(10) -> 20 pwm
drive (-10) -> -20 pwm

drive (255) -> 230 pwm
drive (-255) -> -230 pwm


*/

translator.printdebug();

for(int i = -250;i<=250;i=i+10){
    translator.pwmnaarstuwkracht_test(i);
    translator.stuwkrachtnaarpwm_test(translator.pwmnaarstuwkracht(i));
}
}