#include "state_ArUco.h"

// === PID-regeling ===
float berekenAfstandKrachtArUco(float afstand) {
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
#define pixelsToCm 3.4
void run_state_aruco_afstand(float afstandPixels, Blower& stuwMotorLinks ,Blower& stuwMotorRechts) {
  float afstandCM = afstandPixels / pixelsToCm;  // naar cm

  float kracht = berekenAfstandKrachtArUco(afstandCM);
  float krachtGram = kracht / 0.00981;

  // Beide achterventilatoren leveren gelijke kracht
  //setRearLeftFan(krachtGram);
  //setRearRightFan(krachtGram);
  stuwMotorLinks.leverkracht(krachtGram);
  stuwMotorRechts.leverkracht(krachtGram);

  // Debug
  Serial.print("[Afstand ArUco] Afstand: ");
  Serial.print(afstandCM, 2);
  //Serial.print(" cm | Filtered: ");
  //Serial.print(vorigeAfstand, 2);
  Serial.print(" cm | Kracht: ");
  Serial.print(kracht, 3);
  Serial.print(" N | Gram: ");
  Serial.println(krachtGram, 2);

  //_SDCardWriter.log(afstandCM, kracht, krachtGram, 15);
}



float berekenOrientatieKrachtArUco(float yawRate_rad) {
  // PID-waarden voor yaw-stabilisatie (verlaagd)
  static const float Kp = 0.08f;
  static const float Ki = 0.01f;
  static const float Kd = 0.01f;

  // Systeeminstellingen
  static const float gewensteYaw = 0.0f;      // in rad/s
  static const float armLength = 0.1f;        // in meter
  static const float gravity = 9.81f;         // m/s^2

  static const float maxForceForward = 0.2f;     // N
  static const float maxForceBackward = -0.2f;   // N

  // PID-status
  static float yaw_integral = 0.0f;
  static float yaw_last_error = 0.0f;
  static unsigned long yaw_last_time = 0;

  float error = gewensteYaw - yawRate_rad;

  unsigned long now = millis();
  float dt = (now - yaw_last_time) / 1000.0f;
  if (dt <= 0.0f) dt = 0.001f;
  yaw_last_time = now;

  yaw_integral += error * dt;

  // Anti-windup
  yaw_integral = constrain(yaw_integral, -10.0f, 10.0f);

  float derivative = (error - yaw_last_error) / dt;
  yaw_last_error = error;

  float torque = Kp * error + Ki * yaw_integral + Kd * derivative;

  // Kracht = torque / (2 * armLength)
  float kracht = torque / (2.0f * armLength);

  // Beperk kracht tot motorlimieten
  kracht = constrain(kracht, maxForceBackward, maxForceForward);
  return kracht;
}

void run_state_aruco_orienteren(float xHoek, Blower& stuwMotorLinks ,Blower& stuwMotorRechts) {
  float kracht = berekenOrientatieKrachtArUco(xHoek);
  float krachtGram = kracht / 0.00981;

  // Beide achterventilatoren leveren gelijke kracht
  //setRearLeftFan(krachtGram);
  //setRearRightFan(krachtGram);
  stuwMotorLinks.leverkracht(krachtGram);
  stuwMotorRechts.leverkracht(krachtGram);

  // Debug
  Serial.print("[Afstand ArUco] Orientatie: ");
  Serial.print(xHoek, 2);
  //Serial.print(" cm | Filtered: ");
  //Serial.print(vorigeAfstand, 2);
  Serial.print(" Kracht: ");
  Serial.print(kracht, 3);
  Serial.print(" N | Gram: ");
  Serial.println(krachtGram, 2);
  //_SDCardWriter.log(xHoek, kracht, krachtGram, 15);
}
