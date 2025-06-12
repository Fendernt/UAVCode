#ifndef STATE_MUURSTOPPER_H
#define STATE_MUURSTOPPER_H

#include "../sensors/TOFSensor.h"
#include "../blowers/Blower.h"
#include "../other/Globals.h"
#include <Arduino.h>

// Enkele TOF-sensor die naar de muur kijkt
extern TOFSensor tofVoor;

// Externe functies om achterventilatoren aan te sturen (in gram kracht)
void setRearLeftFan(float krachtGram);
void setRearRightFan(float krachtGram);

// Muurstop-logica, aanroepen in je main loop of state-machine
void run_state_muur_stopper(Blower& stuwMotorLinks, PWMTranslator& translatorlinks, BlowerDriver& linkerdriver,Blower& stuwMotorRechts, PWMTranslator& translatorrechts,BlowerDriver& rechterdriver);

#endif
