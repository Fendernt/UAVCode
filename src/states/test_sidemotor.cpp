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
    Serial.print("imput pwm:");
    Serial.print(i);
    Serial.print(" , ");
    Serial.print(translator.pwmnaarstuwkracht(i),4);
    Serial.print(" komt overeen met: ");
    Serial.print(translator.stuwkrachtnaarpwm(translator.pwmnaarstuwkracht(i)));
    Serial.println(" pwm");
    }
}