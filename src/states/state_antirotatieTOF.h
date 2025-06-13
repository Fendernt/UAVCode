#ifndef STATE_ANTIROTATIETOF_H
#define STATE_ANTIROTATIETOF_H

#include <Arduino.h>
#include "../blowers/Blower.h"
#include "../blowers/PWMTranslator.h"
#include "../sensors/TOFSensor.h"
#include "../other/Globals.h"

// Externe ToF-sensoren (bij linkerzijde hovercraft)
extern TOFSensor tofLvoor;
extern TOFSensor tofLachter;

// Hoekberekening en krachtbepaling
float berekenYawHoek(); // berekent hoek in radialen
float berekenCorrectiekrachtTOF(float hoek);

// Uitvoerfunctie voor antidraaien
void run_state_antirotatieTOF(Blower& stuwMotorLinks, PWMTranslator& translatorlinks, BlowerDriver& linkerdriver,Blower& stuwMotorRechts, PWMTranslator& translatorrechts,BlowerDriver& rechterdriver);

#endif
