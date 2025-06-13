#include "state_90gradendraai.h"

// PID-waarden voor yaw-stabilisatie (verlaagd)
const float Kp = 0.08f;
const float Ki = 0.01f;
const float Kd = 0.01f;

// Systeeminstellingen
const float gewensteYaw = 0.0f;      // in rad/s
const float armLength = 0.1f;        // in meter
const float gravity = 9.81f;         // m/s^2

const float maxForceForward = 0.2f;     // N
const float maxForceBackward = -0.2f;   // N

// PID-status
static float yaw_integral = 0.0f;
static float yaw_last_error = 0.0f;
static unsigned long yaw_last_time = 0;

// Bereken corrigerende kracht (in Newton) op basis van yaw-rate (in rad/s)
float berekenCorrectiekrachtdraaien(float yawRate_rad) {
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

// Aansturing van beide achterventilatoren voor rotatiecontrole
void run_state_draaien(Blower& stuwMotorLinks, PWMTranslator& translatorlinks, BlowerDriver& linkerdriver,
                               Blower& stuwMotorRechts, PWMTranslator& translatorrechts, BlowerDriver& rechterdriver,
                               GyroSensor& gyro) {
    // Gyro geeft graden/s → omrekenen naar rad/s
    float yawRate_deg = gyro.getYaw();  
    float yawRate_rad = yawRate_deg * (PI / 180.0f);

    float kracht = berekenCorrectiekrachtdraaien(yawRate_rad);

    float krachtLinks  = constrain(+kracht, maxForceBackward, maxForceForward);
    float krachtRechts = constrain(-kracht, maxForceBackward, maxForceForward);

    float krachtLinksGram  = krachtLinks / 0.00981f;   // convert N to gram-force
    float krachtRechtsGram = krachtRechts / 0.00981f;

    stuwMotorLinks.leverkracht(krachtLinksGram);
    stuwMotorRechts.leverkracht(krachtRechtsGram);

    // Debug mag je zelf weer aanzetten indien nodig
        Serial.print("[draaien] YawRate: ");
    Serial.print(yawRate_deg, 4);
    Serial.print(" graden | Kracht L: ");
    Serial.print(krachtLinks, 3);
    Serial.print(" N (");
    Serial.print(krachtLinksGram, 2);
    Serial.print(" g), R: ");
    Serial.print(krachtRechts, 3);
    Serial.print(" N (");
    Serial.print(krachtRechtsGram, 2);
    Serial.print(" g) | PWM L/R: ");
    Serial.print(linkerdriver.drive(translatorlinks.stuwkrachtnaarpwm(krachtLinksGram)));
    Serial.print(", ");
    Serial.println(rechterdriver.drive(translatorrechts.stuwkrachtnaarpwm(krachtRechtsGram)));
}

