#include "antirotatiegyro.h"
#include <Arduino.h>

AntiRotatieGyro::AntiRotatieGyro() {
    kp = 0.1f;
    ki = 0.05f;
    kd = 0.01f;

    dt = 0.01f;

    maxForceForward = 0.3f;
    maxForceBackward = -0.4f;

    armLength = 0.05f;
    gravity = 9.81f;

    integral = 0.0f;
    prevError = 0.0f;
}

void AntiRotatieGyro::begin() {
    Serial.begin(9600);
    // gyro.begin(); // indien nodig
}

void AntiRotatieGyro::update() {
    delay((int)(dt * 1000));

    float currentYawRate = readYawRate();
    float desiredYawRate = 0.0f;

    float error = desiredYawRate - currentYawRate;
    integral += error * dt;
    float derivative = (error - prevError) / dt;
    prevError = error;

    float torque = kp * error + ki * integral + kd * derivative;

    float force = torque / (2.0f * armLength);

    float forceLeft = constrain(+force, maxForceBackward, maxForceForward);
    float forceRight = constrain(-force, maxForceBackward, maxForceForward);

    float forceLeftGrams  = forceLeft / gravity;
    float forceRightGrams = forceRight / gravity;

    setRearLeftFan(forceLeftGrams);
    setRearRightFan(forceRightGrams);

    Serial.print("YawRate [rad/s]: "); Serial.print(currentYawRate);
    Serial.print("\tF_left [N]: "); Serial.print(forceLeft);
    Serial.print("\tF_right [N]: "); Serial.print(forceRight);
    Serial.print("\tF_left [g]: "); Serial.print(forceLeftGrams);
    Serial.print("\tF_right [g]: "); Serial.println(forceRightGrams);
}

float AntiRotatieGyro::readYawRate() {
    return gyro.getYaw();
}

void AntiRotatieGyro::setRearLeftFan(float forceInGrams) {
    ::setRearLeftFan(forceInGrams);
}

void AntiRotatieGyro::setRearRightFan(float forceInGrams) {
    ::setRearRightFan(forceInGrams);
}
