#include "state_afmeren.h"
#include <Arduino.h>

// === PID instellingen ===
float setpoint = 20.0;  // gewenste afstand in cm
float Kp = 0.12;
float Ki = 0.0;
float Kd = 0.8;

float integral = 0.0;
float last_error = 0.0;
unsigned long last_time = 0;

// === Externe TOF sensor objecten ===
// TOFSensor tofLvoor(47);
// TOFSensor tofLachter(48);
// TOFSensor tofVoor(49);  // wordt niet gebruikt

extern TOFSensor tofLvoor;
extern TOFSensor tofLachter;

void setup() {
  last_time = millis();
  // Eventuele sensor initialisatie, indien nodig
}

float regelAfstand(float afstandGemiddeld) {
  float error = setpoint - afstandGemiddeld;

  unsigned long now = millis();
  float dt = (now - last_time) / 1000.0;  // dt in seconden
  if (dt <= 0.0) dt = 0.001;               // voorkom deling door 0
  last_time = now;

  integral += error * dt;
  float derivative = (error - last_error) / dt;
  last_error = error;

  float kracht = Kp * error + Ki * integral + Kd * derivative;

  // Beperk kracht tot max ±0.15 N
  kracht = constrain(kracht, -0.15, 0.15);

  return kracht;
}

void loop() {
  // Lees afstanden in mm, converteer naar cm
  float afstandVoor = tofLvoor.getDistance();     // mm
  float afstandAchter = tofLachter.getDistance(); // mm
  float afstandGemiddeld = (afstandVoor + afstandAchter) / 2.0 / 10.0; // cm

  // Bereken kracht met PID-regelaar
  float kracht = regelAfstand(afstandGemiddeld);

  // Hier kun je de kracht (float in Newton) doorgeven aan de motorregelaar
  // bv: stuurKrachtNaarMotor(kracht);
  
  // Of print het voor debugging:
  Serial.print("Afstand: ");
  Serial.print(afstandGemiddeld);
  Serial.print(" cm, Kracht: ");
  Serial.print(kracht);
  Serial.println(" N");

  delay(10);  // optioneel: kleine pauze
}
