#ifndef STATE_90GRADENDRAAI_H
#define STATE_90GRADENDRAAI_H

#include <Arduino.h>
#include "../blowers/Blower.h"
#include "../blowers/PWMTranslator.h"
#include "../sensors/GyroSensor.h"

// Externe afhankelijkheden (zoals gyro en motorfuncties)
extern float getYawRate(); // moet elders gedefinieerd worden


// Hoofd PID-functie die de krachten berekent op basis van yaw-rate
float berekenCorrectiekrachtdraaien(float yawRate);

// Uitvoerfunctie voor antidraaien
void run_state_draaien(Blower& stuwMotorLinks, PWMTranslator& translatorlinks, BlowerDriver& linkerdriver,Blower& stuwMotorRechts, PWMTranslator& translatorrechts,BlowerDriver& rechterdriver, GyroSensor& gyro);

#endif
