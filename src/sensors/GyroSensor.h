  #ifndef GYROSENSOR_H
#define GYROSENSOR_H

#include "I2Cdev.h"
#include "MPU6050_6Axis_MotionApps20.h"
#include "Wire.h"


//Comment dit uit om data in de serial monitor te krijgen.
#define GYRODEBUG

class GyroSensor {
    public:
      GyroSensor(int adress){
        //In de constructor het I2C adress meegeven, en de setup functie aanroepen.
        this->mpu = new MPU6050(adress);
      }

      //Init en update functies, init in setup, update in loop.
      void init(int calAmount = 30);
      void update();

      //Get gyro waardes van de gyro.
      float getYaw() {return this->yawRefined;} //Yaw = rotatie
      float getYawRaw() {return this->yaw;}
      float getPitch() {return this->pitch;} //Pitch = x-as tilt
      float getRoll() {return this->roll;} //Roll is y-as tilt

      //Get acceleration waardes.
      int getAccelX() {return -aaReal.x; }
      int getAccelY() {return -aaReal.y; }
      int getAccelZ() {return aaReal.z; } 

    private:
      void readMpuData(); //Lees de waardes uit.
      void setupMPU(int calAmount); //Setup de mpu.
      

      //Storarage variables
      MPU6050* mpu;
      float ypr[3];
      float yaw;
      float pitch;
      float roll;
      float yprOffset[3];


      //Gyro leest uit tussen 0->360, en switched tussen die 2 als je er overheen gaat. (dus 461 word 1, en -2 word 358)
      //yawRefined zorged ervoor dat je oneindig kan op/af tellen.
      float yawRefined = 0;
      void updateYawRefined();

      //Control variables om te kijken of de mpu ready is.
      bool dmpReady = false;
      uint8_t devStatus;
      uint8_t fifoBuffer[64];


      //Variables om mee te rekenen.
      Quaternion q;
      VectorInt16 aa;
      VectorInt16 aaReal;
      VectorFloat gravity;

      



  };

  #endif