#ifndef ANTIROTATIEGYRO_H
#define ANTIROTATIEGYRO_H

class AntiRotatieGyro {
public:
    AntiRotatieGyro();
    void begin();
    void update();

private:
    float kp, ki, kd;
    float dt;

    float maxForceForward, maxForceBackward;

    float armLength;
    float gravity;

    float integral;
    float prevError;

    void setRearLeftFan(float forceInGrams);
    void setRearRightFan(float forceInGrams);
    float readYawRate();
};

#endif
