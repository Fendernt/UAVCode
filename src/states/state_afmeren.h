#ifndef STATE_AFMEREN_H
#define STATE_AFMEREN_H

#include "../sensors/TOFSensor.h"
#include "../blowers/Blower.h"
#include "../blowers/PWMTranslator.h"
#include "../other/Globals.h"
#include <Arduino.h>

// Externe TOF-sensoren
extern TOFSensor tofLvoor;
extern TOFSensor tofLachter;

// Externe functie om kracht naar motor te sturen
void stuurKrachtNaarMotor(float kracht);

// De afmeer-functie die je in main aanroept
void run_state_afmeren(Blower& sideblower, PWMTranslator& translator, SideBlowerDriver& sideblowerdriver);

#endif
