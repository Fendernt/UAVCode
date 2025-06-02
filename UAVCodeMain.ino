#include "src/sensors/GyroSensor.h"
#include "src/sensors/TOFSensor.h"
#include "src/other/SevenDigitDisplay.h"
#include "src/motors/StuwMotorDriver.h"
#include "src/motors/SideMotorDriver.h"
#include "Wire.h" 

#define state_startup 0
#define state_afmeren 1
#define state_vooruit 2
#define state_draaien 3
#define state_arucomarker 4


// Relay Pinnen
const int thrusterPin = 3;  // Pin D3
const int pinD4 = 4;  // Pin D4

// Pins voor gemeenschappelijke anodes ("lapjes")
const int anodePins[2] = {
  34, // Digit 1 (tientallen)
  35  // Digit 2 (eenheden)
};
const uint8_t interruptPin = 2;  // moet naar GND schakelen voor trigger
unsigned long previousDisplayMillis;
const unsigned long displayInterval = 200; // 5 ms per digit


// --- Statusvariabele ---
volatile bool systemStopped = false;
volatile int UAVState = 0;



SevenDigitDisplay sevenDigitDisplay(
  37, // A 
  39, // B 
  36, // C 
  32, // D 
  30, // E 
  33, // F 
  31, // G 
  38  // H 
);


TOFSensor tofLinks(48);
TOFSensor tofRechts(47);
TOFSensor tofVoor(49);

GyroSensor gyro(0x68);

StuwMotorDriver stuwMotorDriver(13,12,5,6);
SideMotorDriver sideMotorDriver(8,9,7);


void setup() {
  // put your setup code here, to run once:
  Wire.begin();
  Serial.begin(115200);

  initRelays();
  digitalWrite(pinD4, HIGH);
  //digitalWrite(thrusterPin, HIGH);


  initAmperageControl();


  initTOFSensors();
  gyro.init(5);


  switchState(state_startup);
}

int state = 0;

void loop() {
  //Always dislay amperage.
  //displayAmperage();

  return;


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
  pinMode(thrusterPin, OUTPUT);
  pinMode(pinD4, OUTPUT);

  //Zet pinnen D3 en D4 meteen uit
  digitalWrite(thrusterPin, LOW);
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

  //analogReference(INTERNAL1V1);

  // Stel interruptPin in met interne pull-up
  pinMode(interruptPin, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(interruptPin), onderspanningISR, FALLING);
}

// --- ISR: wordt aangeroepen bij FALLING op interruptPin ---
void onderspanningISR() { 
  systemStopped = true;
  digitalWrite(thrusterPin, LOW);   // relais 1 UIT
  digitalWrite(pinD4, LOW);   // relais 2 UIT
  Serial.println("Interrupt aangeroepen: systeem uitgeschakeld, relais uit");
}


void displayAmperage(){
  static int currentDigit = 0; // wisselt tussen 0 en 1
  
  int raw = analogRead(A0);
  float voltage = (raw / 1023.0) * 5.0;
  float current = (voltage - 2.5) / 0.1;  // Ampère
  int displayValue = constrain(round(abs(current * 10)), 0, 99); // *10 om 1 decimaal te tonen

  Serial.print("Raw: ");
  Serial.print(raw);
  Serial.print(" Voltage: ");
  Serial.print(voltage);
  Serial.print(" Current: ");
  Serial.print(current);
  Serial.print(" Display val:");
  Serial.println(displayValue);

  unsigned long currentMillis = millis();

  // --- Multiplexing display (wissel digits om de 5 ms) ---
  if (currentMillis - previousDisplayMillis >= displayInterval) {
    previousDisplayMillis = currentMillis;

    // Zet beide anodes uit
    for (int i = 0; i < 2; i++) {
      digitalWrite(anodePins[i], HIGH); // uit
    }

    int digitToShow = (currentDigit == 0) ? (displayValue / 10) : (displayValue % 10);

    //Draw digit 
    sevenDigitDisplay.drawDigit(digitToShow);

    // Zet juiste anode aan
    digitalWrite(anodePins[currentDigit], LOW);

    // Wissel naar volgende digit
    currentDigit = (currentDigit + 1) % 2;
  }
}