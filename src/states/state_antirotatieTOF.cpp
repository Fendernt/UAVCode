#include "state_antirotatieTOF.h"

// PID-waarden voor yaw-hoek stabilisatie
const float Kp = 1.0f;
const float Ki = 0.5f;
const float Kd = 0.1f;

// Systeeminstellingen
const float gewensteYaw = 0.0f;
const float armLength = 0.1f;      // meter (afstand motor tot zwaartepunt)
const float sensorAfstand = 0.142f; // meter tussen voorste en achterste ToF-sensor

const float maxForceForward = 0.2f;    // N
const float maxForceBackward = -0.2f;  // N

// PID-status
static float yaw_integral = 0.0f;
static float yaw_last_error = 0.0f;
static unsigned long yaw_last_time = 0;

// Bepaal yaw-hoek (in radialen) uit verschil ToF-afstanden
float berekenYawHoek() {
    float afstandVoor = tofLvoor.getDistance();     // mm
    float afstandAchter = tofLachter.getDistance(); // mm

    // Vermijd deling door 0
    if (sensorAfstand <= 0.0f) return 0.0f;

    // Verschil in afstand (in meter)
    float delta = (afstandVoor - afstandAchter) / 1000.0f;
    float hoek = atan(delta / sensorAfstand); // in radialen
    return hoek;
}

// Bereken corrigerende kracht (in Newton) op basis van yaw-hoek
float berekenCorrectiekrachtTOF(float yaw) {
    float error = gewensteYaw - yaw;

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

    // Omgerekende kracht
    float kracht = torque / (2.0f * armLength);

    // Begrens
    kracht = constrain(kracht, maxForceBackward, maxForceForward);
    return kracht;
}

// Aansturing van beide zijwaartse motoren
void run_state_antirotatieTOF(Blower& stuwMotorLinks, PWMTranslator& translatorlinks, BlowerDriver& linkerdriver,Blower& stuwMotorRechts, PWMTranslator& translatorrechts,BlowerDriver& rechterdriver) {
    float yaw = berekenYawHoek();
    float kracht = berekenCorrectiekrachtTOF(yaw);

    float krachtLinks  = constrain(+kracht, maxForceBackward, maxForceForward);
    float krachtRechts = constrain(-kracht, maxForceBackward, maxForceForward);

    float krachtLinksGram  = krachtLinks / 0.00981f;
    float krachtRechtsGram = krachtRechts / 0.00981f;

    stuwMotorLinks.leverkracht(krachtLinksGram);
    stuwMotorRechts.leverkracht(krachtRechtsGram);

    // Debug
    Serial.print("[ToF Antirotatie] YawHoek: ");
    Serial.print(yaw, 4);
    Serial.print(" rad | Kracht L: ");
    Serial.print(krachtLinks, 3);
    Serial.print(" N (");
    Serial.print(krachtLinksGram, 2);
    Serial.print(" g), R: ");
    Serial.print(krachtRechts, 3);
    Serial.print(" N (");
    Serial.print(krachtRechtsGram, 2);
    Serial.println(" g)");
}
