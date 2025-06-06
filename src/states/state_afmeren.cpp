#include "state_afmeren.h"


// PID-instellingen afgeleid van jouw Python-code
const float setpoint = 20.0;  // gewenste afstand in cm
const float Kp = 0.147;
const float Ki = 0.0;
const float Kd = 0.2;  // verlaagd voor minder schokken

// Interne PID-status
float integral = 0.0;
float last_error = 0.0;
float vorigeAfstand = setpoint;
unsigned long last_time = 0;

// === PID-regelaar: bereken kracht in Newton ===
float berekenKracht(float afstandGemiddeld) {
  // Simpele low-pass filter om meetruis te onderdrukken
  float afstandFiltered = afstandGemiddeld * 0.3 + vorigeAfstand * 0.7;
  vorigeAfstand = afstandFiltered;

  float error = setpoint - afstandFiltered;

  unsigned long now = millis();
  float dt = (now - last_time) / 1000.0;
  if (dt <= 0.0) dt = 0.001;
  last_time = now;

  integral += error * dt;

  float derivative = (error - last_error) / dt;
  derivative = constrain(derivative, -50.0, 50.0);  // jitter beperken

  last_error = error;

  float kracht = Kp * error + Ki * integral + Kd * derivative;

  // Beperk kracht zoals in de simulatie
  kracht = constrain(kracht, -0.11, 0.15);

  return kracht;
}

// === State-afhandelingsfunctie ===
void run_state_afmeren(Blower& sideblower) {
  // Lees TOF-afstanden in mm en converteer naar cm
  float afstandVoor = tofLvoor.getDistance();     // mm
  float afstandAchter = tofLachter.getDistance(); // mm

  // Check op ongeldige waarden (0 betekent meestal fout)
  if (afstandVoor == 0 || afstandAchter == 0) {
    Serial.println("[Afmeren] TOF-fout: afstand = 0 mm");
    sideblower.leverkracht(0);
    return;
  }

  float afstandGemiddeld = (afstandVoor + afstandAchter) / 2.0 / 10.0; // cm

  float kracht = berekenKracht(afstandGemiddeld);
  
  sideblower.leverkracht(kracht);

 

  // Debug info
  Serial.print("[Afmeren] Afstand: ");
  Serial.print(afstandGemiddeld, 2);
  Serial.print(" cm, Filtered: ");
  Serial.print(vorigeAfstand, 2);
  Serial.print(" cm, Kracht: ");
  Serial.print(kracht, 3);
  Serial.println(" N");
}


