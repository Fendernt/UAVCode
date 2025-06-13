#ifndef STATE_VOORUIT_H
#define STATE_VOORUIT_H

#include <Arduino.h>
#include "../blowers/Blower.h"
#include "../blowers/PWMTranslator.h"
#include "../sensors/TOFSensor.h"
#include "../other/Globals.h"

// TOF-sensoren
extern TOFSensor tofVoor;
extern TOFSensor tofLvoor;
extern TOFSensor tofLachter;

// Hoofdloop voor gecombineerde regeling
void run_state_combined_muur_rotatie(
    Blower& stuwMotorLinks, PWMTranslator& translatorlinks, BlowerDriver& linkerdriver,Blower& stuwMotorRechts, PWMTranslator& translatorrechts,BlowerDriver& rechterdriver);


#endif
