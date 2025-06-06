#include "test_sidemotor.h"

void test_sidemotor(SideBlowerDriver& blower){

    Serial.println("Test sidemotor!");


/*
vooruit:
drive(10) -> 20 pwm
drive (-10) -> -20 pwm

drive (255) -> 230 pwm
drive (-255) -> -230 pwm


*/

Serial.print("10 pwm gestuurd naar de sidemotor");
Serial.print(blower.drive(10));
Serial.println(" pwm geleverd");

}