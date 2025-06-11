#include "state_antirotatiegyro.h"

// PID-waarden voor yaw-stabilisatie
const float Kp = 0.1f;
const float Ki = 0.05f;
const float Kd = 0.01f;

// Systeeminstellingen
const float gewensteYaw = 0.0f;
const float armLength = 0.05f;   // in meter
const float gravity = 9.81f;     // m/s^2

const float maxForceForward = 0.3f;    // N
const float maxForceBackward = -0.4f;  // N

// PID-status
float yaw_integral = 0.0f;
float yaw_last_error = 0.0f;
unsigned long yaw_last_time = 0;

// Bereken corrigerende kracht (in Newton) op basis van yaw-rate
float berekenCorrectiekracht(float yawRate) {
    float error = gewensteYaw - yawRate;

    unsigned long now = millis();
    float dt = (now - yaw_last_time) / 1000.0f;
    if (dt <= 0.0f) dt = 0.001f;
    yaw_last_time = now;

    yaw_integral += error * dt;
    float derivative = (error - yaw_last_error) / dt;
    yaw_last_error = error;

    float torque = Kp * error + Ki * yaw_integral + Kd * derivative;

    // Kracht omzetten via moment = kracht * armLength * 2 → kracht = torque / (2 * armLength)
    float kracht = torque / (2.0f * armLength);

    // Beperken tot motorlimieten
    kracht = constrain(kracht, maxForceBackward, maxForceForward);
    return kracht;
}

// Aansturing van beide achterventilatoren voor rotatiecontrole
void run_state_antirotatie(Blower& stuwMotorLinks, Blower& stuwMotorRechts) {
    float yawRate = getYawRate(); // moet extern gedefinieerd zijn
    float kracht = berekenCorrectiekracht(yawRate);

    float krachtLinks  = constrain(+kracht, maxForceBackward, maxForceForward);
    float krachtRechts = constrain(-kracht, maxForceBackward, maxForceForward);

    float krachtLinksGram  = krachtLinks / gravity;
    float krachtRechtsGram = krachtRechts / gravity;

    //setRearLeftFan(krachtLinksGram);
    //setRearRightFan(krachtRechtsGram);

    // Debug
    Serial.print("[Antirotatie] YawRate: ");
    Serial.print(yawRate);
    Serial.print(" rad/s | Kracht L: ");
    Serial.print(krachtLinks, 3);
    Serial.print(" N (");
    Serial.print(krachtLinksGram, 2);
    Serial.print(" g), R: ");
    Serial.print(krachtRechts, 3);
    Serial.print(" N (");
    Serial.print(krachtRechtsGram, 2);
    Serial.println(" g)");
}
