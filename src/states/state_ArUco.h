#ifndef STATE_ARUCO_AFSTAND_H
#define STATE_ARUCO_AFSTAND_H

#include "../sensors/TOFSensor.h"
#include "../blowers/Blower.h"
#include "../other/Globals.h"
#include <Arduino.h>


// Muurstop-logica, aanroepen in je main loop of state-machine
void run_state_aruco_afstand(float afstandPixels, Blower& stuwMotorLinks ,Blower& stuwMotorRechts);
void run_state_aruco_orienteren(float xHoek, Blower& stuwMotorLinks ,Blower& stuwMotorRechts);
#endif
