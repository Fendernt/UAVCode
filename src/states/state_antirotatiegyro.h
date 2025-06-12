#ifndef STATE_ANTIROTATIEGYRO_H
#define STATE_ANTIROTATIEGYRO_H

#include <Arduino.h>
#include "../blowers/Blower.h"
#include "../blowers/PWMTranslator.h"
#include "../sensors/GyroSensor.h"

// Externe afhankelijkheden (zoals gyro en motorfuncties)
extern float getYawRate(); // moet elders gedefinieerd worden


// Hoofd PID-functie die de krachten berekent op basis van yaw-rate
float berekenCorrectiekrachtGyro(float yawRate);

// Uitvoerfunctie voor antidraaien
void run_state_antirotatieGyro(Blower& stuwMotorLinks, Blower& stuwMotorRechts);

#endif
