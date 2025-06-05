#include "test_tofsensoren.h"  


void test_tofsensoren(TOFSensor &tofLachter, TOFSensor& tofLvoor, TOFSensor& tofVoor){
    int afstand1 = tofLachter.getDistance();
    int afstand2 =tofLvoor.getDistance();
    int afstand3 = tofVoor.getDistance();
    Serial.print("tofLachter");
    Serial.print(afstand1);
    Serial.println("mm");
    Serial.print("tofLvoor");
    Serial.print(afstand2);
    Serial.println("mm");
    Serial.print("tofvoor");    	
    Serial.print(afstand3);
    Serial.println("mm");

    Serial.println("Test tof!");
}