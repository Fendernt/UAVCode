#include "state_muurstopper.h"

// === PID-regeling ===
float berekenAfstandKracht(float afstand) {
    // PID-instellingen
    static const float setpoint = 30.0;  // gewenste afstand in cm
    static const float Kp = 0.147;
    static const float Ki = 0.006;
    static const float Kd = 0.377;

    // Interne PID-variabelen
    static float integral = 0.0;
    static float last_error = 0.0;
    static float vorigeAfstand = setpoint;
    static unsigned long last_time = 0;


  float afstandFiltered = afstand * 0.3 + vorigeAfstand * 0.7;
  vorigeAfstand = afstandFiltered;

  float error = afstandFiltered - setpoint;  // positieve error = te ver weg

  unsigned long now = millis();
  float dt = (now - last_time) / 1000.0;
  if (dt <= 0.0) dt = 0.001;
  last_time = now;

  integral += error * dt;
  float derivative = (error - last_error) / dt;
  derivative = constrain(derivative, -50.0, 50.0);
  last_error = error;

  float kracht = Kp * error + Ki * integral + Kd * derivative;
  kracht = constrain(kracht, -0.20, 0.20);

  return kracht;
}

// === State-afhandeling ===
void run_state_muur_stopper(Blower& stuwMotorLinks, PWMTranslator& translatorlinks, BlowerDriver& linkerdriver,Blower& stuwMotorRechts, PWMTranslator& translatorrechts,BlowerDriver& rechterdriver) {
  float afstandVoor = tofVoor.getDistance();  // mm
  float afstandCM = afstandVoor / 10.0;  // naar cm
  float kracht = berekenAfstandKracht(afstandCM);
  float krachtGram = kracht / 0.00981;

  // Beide achterventilatoren leveren gelijke kracht
  //setRearLeftFan(krachtGram);
  //setRearRightFan(krachtGram);
  stuwMotorLinks.leverkracht(krachtGram);
  stuwMotorRechts.leverkracht(krachtGram);

  // Debug
  Serial.print("[MuurStopper] Afstand: ");
  Serial.print(afstandCM, 2);
  //Serial.print(" cm | Filtered: ");
  //Serial.print(vorigeAfstand, 2);
  Serial.print(" cm | Kracht: ");
  Serial.print(kracht, 3);
  Serial.print(" N | Gram: ");
  Serial.println(krachtGram, 2);
  Serial.print("pwmL, pwmR: ");
  Serial.print(linkerdriver.drive(translatorlinks.stuwkrachtnaarpwm(krachtGram)));
  Serial.println(rechterdriver.drive(translatorrechts.stuwkrachtnaarpwm(krachtGram)));

  _SDCardWriter.log(afstandCM, kracht, krachtGram, 15);
}
