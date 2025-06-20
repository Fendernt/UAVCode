#include "state_vooruit.h"

// PID-waarden muurstopper
const float muur_Kp = 0.147f;
const float muur_Ki = 0.006f;
const float muur_Kd = 0.377f;

// PID-waarden antirotatie
const float rot_Kp = 0.2f;
const float rot_Ki = 0.01f;
const float rot_Kd = 0.2f;

// Setpoints en limieten
const float gewensteYaw = 0.0f;
const float setpointAfstand = 30.0f; // cm
const float maxForce = 0.2f; // N
const float minForce = -0.2f; // N
const float armLength = 0.1f; // meter
const float sensorAfstand = 0.142f; // meter

// Interne PID-status
static float yaw_integral = 0.0f;
static float yaw_last_error = 0.0f;
static unsigned long yaw_last_time = 0;

static float afstand_integral = 0.0f;
static float afstand_last_error = 0.0f;
static float vorigeAfstand = setpointAfstand;
static unsigned long afstand_last_time = 0;

// Hoek berekenen uit zijwaartse TOF-sensoren (in radialen)
float berekenYawHoekvooruit() {
    float afstandVoor = tofLvoor.getDistance();     // mm
    float afstandAchter = tofLachter.getDistance(); // mm
    if (sensorAfstand <= 0.0f) return 0.0f;

    float delta = (afstandVoor - afstandAchter) / 1000.0f; // m
    float hoek = atan(delta / sensorAfstand);
    return hoek;
}

// Corrigerende kracht (N) om yaw terug naar 0 te brengen
float berekenYawKrachtvooruit(float yaw) {
    float error = gewensteYaw - yaw;

    unsigned long now = millis();
    float dt = (now - yaw_last_time) / 1000.0f;
    if (dt <= 0.0f) dt = 0.001f;
    yaw_last_time = now;

    yaw_integral += error * dt;
    yaw_integral = constrain(yaw_integral, -10.0f, 10.0f);

    float derivative = (error - yaw_last_error) / dt;
    yaw_last_error = error;

    float torque = rot_Kp * error + rot_Ki * yaw_integral + rot_Kd * derivative;
    float kracht = torque / (2.0f * armLength); // kracht per motor
    return constrain(kracht, minForce, maxForce);
}

// Corrigerende kracht (N) om op afstand te blijven
float berekenAfstandKrachtvooruit(float afstandCM) {
    float afstandFiltered = afstandCM * 0.3f + vorigeAfstand * 0.7f;
    vorigeAfstand = afstandFiltered;

    float error = afstandFiltered - setpointAfstand;

    unsigned long now = millis();
    float dt = (now - afstand_last_time) / 1000.0f;
    if (dt <= 0.0f) dt = 0.001f;
    afstand_last_time = now;

    afstand_integral += error * dt;
    float derivative = (error - afstand_last_error) / dt;
    afstand_last_error = error;

    derivative = constrain(derivative, -50.0f, 50.0f);

    float kracht = muur_Kp * error + muur_Ki * afstand_integral + muur_Kd * derivative;
    return constrain(kracht, minForce, maxForce);
}

// === HOOFDAANSTURING ===
void run_state_combined_muur_rotatie(
    Blower& stuwMotorLinks,
    PWMTranslator& translatorlinks,
    BlowerDriver& linkerdriver,
    Blower& stuwMotorRechts,
    PWMTranslator& translatorrechts,
    BlowerDriver& rechterdriver
) {
    // Sensormetingen
    float afstandMM = tofVoor.getDistance(); // mm
    float afstandCM = afstandMM / 10.0f;
    float yaw = berekenYawHoekvooruit();

    // Bereken afzonderlijke correcties
    float krachtAfstand = berekenAfstandKrachtvooruit(afstandCM);
    float krachtYaw = berekenYawKrachtvooruit(yaw);

    // Combineer krachten per motor
    float yawInvloedFactor = 0.8f; // experimenteel getal
    float krachtInvloedFactor = 1.2f; // experimenteel getal
    float krachtLinks  = (krachtAfstand * krachtInvloedFactor + yawInvloedFactor * krachtYaw)/2;
    float krachtRechts = (krachtAfstand * krachtInvloedFactor - yawInvloedFactor * krachtYaw)/2;


    // Beperk motoroutput
    krachtLinks  = constrain(krachtLinks, minForce, maxForce);
    krachtRechts = constrain(krachtRechts, minForce, maxForce);

    // Omrekenen naar gram
    float krachtLinksGram  = krachtLinks / 0.00981f;
    float krachtRechtsGram = krachtRechts / 0.00981f;

    // Stuur motoren aan
    stuwMotorLinks.leverkracht(krachtLinksGram);
    stuwMotorRechts.leverkracht(krachtRechtsGram);

    // Debug output
    Serial.print("[COMBI] Afstand: ");
    Serial.print(afstandCM, 2);
    Serial.print(" cm | Yaw: ");
    Serial.print(yaw, 3);
    Serial.print(" rad | Kracht L: ");
    Serial.print(krachtLinks, 3);
    Serial.print(" N (");
    Serial.print(krachtLinksGram, 2);
    Serial.print(" g), R: ");
    Serial.print(krachtRechts, 3);
    Serial.print(" N (");
    Serial.print(krachtRechtsGram, 2);
    Serial.println(" g)");

    _SDCardWriter.log(afstandCM, yaw, krachtLinks, 20);

}
