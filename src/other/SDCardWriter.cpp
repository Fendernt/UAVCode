#include "SDCardWriter.h"


void SDCardWriter::init(){
if (!SD.begin(chipSelect)) {
    Serial.println("\nInitialisatie van de SD-kaart mislukt!");
    return;
  }
  Serial.println("\nInitialisatie van de SD-kaart voltooid.");
}

void SDCardWriter::log(float v1, float v2, float v3, int timer){
    if (stopLogging) return;

    if(currentIndex == 0){
        prevMillis = millis();
        maxWriteTime = timer*1000;
    }

    
    long now = millis();
    if (now - t_lastSample >= samplingInterval) {
        t_lastSample = now;

        buffer[currentIndex].time = now;
        buffer[currentIndex].value1 = v1;
        buffer[currentIndex].value2 = v2;
        buffer[currentIndex].value3 = v3;
        currentIndex++;

        Serial.print("Logging: ");
        Serial.print("Val 1: ");
        Serial.print(v1);
        Serial.print(" Val 2: ");
        Serial.print(v2);
        Serial.print(" Val 3: ");
        Serial.print(v3);
        Serial.print(" Current index: ");
        Serial.print(currentIndex);
        Serial.print(" NumElements: ");
        Serial.print(numElements);
        Serial.print(" Structgrootte: ");
        Serial.println(sizeof(SensorData)); // Verwacht: 16

        if (currentIndex == numElements) {
        writeBufferToFile();
        currentIndex = 0;
        }

        if (now >= loggingDuration) {
        stopLogging = true;
        Serial.println("Gestopt met meetdata wegschrijven.");
        }
    }

    if(millis() - prevMillis >= maxWriteTime){
        finishWriting();
    }
}

void SDCardWriter::finishWriting(){
    stopLogging = 1;
    writeBufferToFile();
    currentIndex = 0;
}

void SDCardWriter::writeBufferToFile(){
    unsigned fileCounter = 0;
    String filename;
    do {
    filename = "data" + String(fileCounter++) + ".bin";
    } while (SD.exists(filename));

    File file = SD.open(filename, FILE_WRITE);
    if (file) {
    Serial.println("Schrijven naar " + filename + "...");
    file.write((byte*)buffer, numElements * sizeof(SensorData));
    file.close();
    Serial.println("Schrijven voltooid.");
    } else {
    Serial.println("Fout bij het openen van bestand!");
    }
}