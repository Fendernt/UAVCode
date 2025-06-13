#ifndef SDCARDWRITER_H
#define SDCARDWRITER_H

#include <SPI.h>
#include <SD.h>
#include <Arduino.h>

// Definieer maximale grootte van buffer
#define maxBufferSize 4096

#pragma pack(push, 1)
struct SensorData {
  unsigned long time;
  float value1;
  float value2;
  float value3;
};
#pragma pack(pop)


class SDCardWriter {
    public:
        SDCardWriter(uint8_t csPin) : chipSelect(csPin), currentIndex(0), stopLogging(false), t_lastSample(0) {
            numElements = maxBufferSize / sizeof(SensorData);
        }

        void init();
        void log(float v1, float v2, float v3, int timer=40);
        void finishWriting();
        
    private:
        uint8_t chipSelect;
        unsigned numElements;
        SensorData buffer[maxBufferSize / sizeof(SensorData)];
        int currentIndex;
        bool stopLogging;
        long t_lastSample;

        unsigned long startingMillis;
        unsigned long prevMillis;
        int maxWriteTime = 0;

        //static const unsigned maxBufferSize = 4096;
        static const unsigned samplingInterval = 100;    // ms
        static const unsigned loggingDuration = 60000;   // ms

        void writeBufferToFile();
};


#endif