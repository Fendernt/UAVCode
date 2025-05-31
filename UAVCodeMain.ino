#include "src/sensors/GyroSensor.h"
#include "src/sensors/TOFSensor.h"
#include "src/other/SevenDigitDisplay.h"
#include "src/motors/StuwMotorDriver.h"
#include "Wire.h" 

#define state_startup 0



// Relay Pinnen
const int pinD3 = 3;  // Pin D3
const int pinD4 = 4;  // Pin D4

// Pins voor gemeenschappelijke anodes ("lapjes")
const int anodePins[2] = {
  34, // Digit 1 (tientallen)
  35  // Digit 2 (eenheden)
};
const uint8_t interruptPin = 2;  // moet naar GND schakelen voor trigger
unsigned long previousDisplayMillis;
const unsigned long displayInterval = 200; // 5 ms per digit
int currentDigit = 0; // wisselt tussen 0 en 1

// --- Statusvariabele ---
volatile bool systemStopped = false;
volatile int UAVState = 0;



SevenDigitDisplay sevenDigitDisplay(
    32, // A
    30, // B
    33, // C
    37, // D
    39, // E
    36, // F
    38, // G
    31  // DP
  );


  TOFSensor tofLinks(48);
  TOFSensor tofRechts(47);
  TOFSensor tofVoor(49);


void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Wire.begin();

  initAmperageControl();
  initTOFSensors();

  initRelays();

  switchState(state_startup);

}

void loop() {
  //Always dislay amperage.
  displayAmperage();

  if(systemStopped) {
    //systemStopped is voor als de batterij onderspanning heeft.
    return;
  }



  switch(UAVState){
    case state_startup:

      break;
  }



}

void switchState(int state){
  switch(state){
    case state_startup:
        //idk what to do here yet.
      break;
  }




    UAVState = state;
}



void initRelays(){
  //Relay pinnen instellen.
  pinMode(pinD3, OUTPUT);
  pinMode(pinD4, OUTPUT);

  //Zet pinnen D3 en D4 meteen uit
  digitalWrite(pinD3, LOW);
  digitalWrite(pinD4, LOW);
}

void initTOFSensors(){
  tofRechts.initAddres(0x30);
  tofRechts.setOffset(-10.5);

  tofLinks.initAddres(0x31);
  tofRechts.setOffset(-19.5);

  tofVoor.initAddres(0x32);
  tofVoor.setOffset(-4);
}

void initAmperageControl(){
// Anodepinnen instellen
  for (int i = 0; i < 2; i++) {
    pinMode(anodePins[i], OUTPUT);
    digitalWrite(anodePins[i], HIGH); // Anode uit (common anode)
  }


  // Stel interruptPin in met interne pull-up
  pinMode(interruptPin, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(interruptPin), onderspanningISR, FALLING);
}

// --- ISR: wordt aangeroepen bij FALLING op interruptPin ---
void onderspanningISR() { 
  systemStopped = true;
  digitalWrite(pinD3, LOW);   // relais 1 UIT
  digitalWrite(pinD4, LOW);   // relais 2 UIT
  Serial.println("Interrupt aangeroepen: systeem uitgeschakeld, relais uit");
}


void displayAmperage(){
  int raw = analogRead(A0);
  float voltage = (raw / 1023.0) * 5.0;
  float current = (voltage - 2.5) / 0.1;  // Ampère
  int displayValue = constrain(round(abs(current * 10)), 0, 99); // *10 om 1 decimaal te tonen

  unsigned long currentMillis = millis();

  // --- Multiplexing display (wissel digits om de 5 ms) ---
  if (currentMillis - previousDisplayMillis >= displayInterval) {
    previousDisplayMillis = currentMillis;

    // Zet beide anodes uit
    for (int i = 0; i < 2; i++) {
      digitalWrite(anodePins[i], HIGH); // uit
    }

    if(currentDigit == 2) sevenDigitDisplay.setEnabled(0);
    else sevenDigitDisplay.setEnabled(1);

    int digitToShow = (currentDigit == 0) ? (displayValue / 10) : (displayValue % 10);

    //Draw digit 
    sevenDigitDisplay.drawDigit(digitToShow);

    // Zet juiste anode aan
    digitalWrite(anodePins[currentDigit], LOW);

    // Wissel naar volgende digit
    currentDigit = (currentDigit + 1) % 3;
  }
}