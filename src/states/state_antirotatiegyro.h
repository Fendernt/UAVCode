#ifndef STATE_ANTIROTATIE_H
#define STATE_ANTIROTATIE_H

#include <Arduino.h>
#include "../blowers/Blower.h"
#include "../blowers/PWMTranslator.h"

// Externe afhankelijkheden (zoals gyro en motorfuncties)
extern float getYawRate(); // moet elders gedefinieerd worden
extern void setRearLeftFan(float forceInGrams);
extern void setRearRightFan(float forceInGrams);

// Hoofd PID-functie die de krachten berekent op basis van yaw-rate
float berekenCorrectiekracht(float yawRate);

// Uitvoerfunctie voor antidraaien
void run_state_antirotatie(Blower& sideblower, PWMTranslator& translator, SideBlowerDriver& sideblowerdriver);

#endif
