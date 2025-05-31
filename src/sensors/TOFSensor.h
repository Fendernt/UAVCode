#ifndef TOFSENSOR_H
#define TOFSENSOR_H

#include <Arduino.h>
#include <Wire.h>
#include <VL53L0X.h>

class TOFSensor {
    public:
      TOFSensor(int shutPin, float offset = 0);
      float getDistance();
      void initAddres(int adress);
      void setOffset(float offset);

    private:
      float filterDistance(int distance);

      VL53L0X* sensor;
      const int filterLengte = 4; //Als dit veranderd word, verander ook de bufferArray size
      float bufferArray[4] = {0};
      int bufferIndex = 0;

      float offset;
      int shutPin;

      int isFirstMeasurement = 1;
      int adressInitialised = 0;


};


#endif