#include "GyroSensor.h"

void GyroSensor::init(int calAmount){
    Wire.begin();
    Wire.setClock(400000);

    setupMPU(calAmount);
  }

  void GyroSensor::update(){
    readMpuData();
    updateYawRefined();
  }

  void GyroSensor::setupMPU(int calAmount){
    Serial.println(F("Initializing I2C devices..."));
    mpu->initialize();
    Serial.println(mpu->testConnection() ? F("MPU6050 connection successful") : F("MPU6050 connection failed"));

    Serial.println(F("Initializing DMP..."));
    devStatus = mpu->dmpInitialize();
    mpu->setDMPEnabled(true);

    if (devStatus == 0) {
        mpu->CalibrateAccel(calAmount);
        mpu->CalibrateGyro(calAmount);
        mpu->PrintActiveOffsets();
        Serial.println(F("DMP ready! Waiting for first interrupt..."));
        dmpReady = true;
    } else {
        Serial.print(F("DMP Initialization failed (code "));
        Serial.print(devStatus);
        Serial.println(F(")"));
    }
  }

void GyroSensor::updateYawRefined() {
    static float prevYaw = 0;
    float yaw = this->getYawRaw();  // Dit geeft een waarde van -180 tot 180 aan

    // Bereken verschil tussen vorige en huidige yaw
    float deltaYaw = yaw - prevYaw;

    // Corrigeer sprongen over de ±180 grens
    if (deltaYaw > 180) {
        deltaYaw -= 360;
    } else if (deltaYaw < -180) {
        deltaYaw += 360;
    }
//test
    // Tel bij op bij yawRefined
    yawRefined += deltaYaw;

    // Update vorige yaw
    prevYaw = yaw;
}


  void GyroSensor::readMpuData(){
    if (!dmpReady) return;

    if (mpu->dmpGetCurrentFIFOPacket(fifoBuffer)) {
      mpu->dmpGetQuaternion(&q, fifoBuffer);
      mpu->dmpGetAccel(&aa, fifoBuffer);
      mpu->dmpGetGravity(&gravity, &q);
      mpu->dmpGetYawPitchRoll(ypr, &q, &gravity);
      mpu->dmpGetLinearAccel(&aaReal, &aa, &gravity);

      //Output is in rad/s, dus *180 /3.14 voor graden.
      yaw = (ypr[0] * 180) / 3.14;
      pitch = ypr[1] * 180 / 3.14;
      roll=ypr[2] * 180 / 3.14;

      #ifdef GYRODEBUG
      Serial.print("yaw: ");
      Serial.print(yawRefined);
      Serial.print("Yaw raw: ");
      Serial.print(yaw);

      Serial.print("\t pitch: ");
      Serial.print(pitch);
      Serial.print("\t roll: ");
      Serial.print(roll);
      Serial.println();
      #endif

    #ifdef ACCELDDEBUG
        serial.print("areal\t");
        Serial.print(aaReal.x);
        Serial.print("\t");
        Serial.print(aaReal.y);
        Serial.print("\t");
        Serial.println(aaReal.z);

      #endif

    }
  }
