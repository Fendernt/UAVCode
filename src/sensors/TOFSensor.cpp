#include "TOFSensor.h"

TOFSensor::TOFSensor(int shutPin, float offset) {
    this->shutPin = shutPin;
    this->offset = offset;

    this->sensor = new VL53L0X();

    pinMode(shutPin, OUTPUT);
    digitalWrite(shutPin, LOW);
}

void TOFSensor::initAddres(int adress) {
    digitalWrite(shutPin, HIGH);  // Alleen deze sensor aan
    delay(150);
    if (!sensor->init()) {
      Serial.print("Sensor init mislukt op addr: ");
      Serial.println(adress);
      return;
    }
    sensor->setAddress(adress);
    delay(50);
    Serial.print("Sensor actief op: ");
    Serial.println(adress);

    adressInitialised = 1;
}

void TOFSensor::setOffset(float offset){
    this->offset = offset;
}

float TOFSensor::getDistance() {
    if(!adressInitialised) {
        Serial.println("Please set adress.");
        return;
    }
    int distance = sensor->readRangeSingleMillimeters() + this->offset;

    return filterDistance(distance);
}

float TOFSensor::filterDistance(int distance) {
    if(isFirstMeasurement){
        //Als het de eerste measurement is vullen we de hele array met de eerste waarde zodat je niet bijv afstand+0+0+0 / filterLengte krijgt en dus de afstand / filterlengte krijgt.
        for(int i = 0; i < filterLengte; i++){
            bufferArray[i] = distance;
        }
        isFirstMeasurement = 0;
    }


    bufferArray[bufferIndex] = distance;
    bufferIndex = (bufferIndex+1);
    if(bufferIndex >= filterLengte) bufferIndex = 0;

    float som = 0.0;
    for(int i = 0; i < filterLengte; i++){
        som+=bufferArray[i];
    }

    return som/filterLengte;
}