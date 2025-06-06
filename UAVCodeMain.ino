#include "src/sensors/GyroSensor.h"
#include "src/sensors/TOFSensor.h"
#include "src/other/SevenDigitDisplay.h"
#include "src/states/test_tofsensoren.h"
#include "src/blowers/Blower.h"
#include "src/blowers/BlowerDriver.h"
#include "src/blowers/PWMTranslator.h"
#include "src/blowers/StuwBlowerDriver.h"
#include "src/blowers/SideBlowerDriver.h"
#include "src/states/test_sidemotor.h"
#include "src/states/state_afmeren.h"

#include "Wire.h" 



//States voor de UAV voor code control.
#define state_startup 0
#define state_idle 1
#define state_afmeren 2
#define state_vooruit 3
#define state_draaien 4
#define state_arucomarker 5
#define state_test_stuwmotoren 6
#define state_test_tofsensoren 7
#define state_test_sidemotor 8

/*
    TODO:

    Maak een class die de motors aanstuurd dmv newton input. 
    (dus gebruik van de translator class)

    Shared newton system, meerdere regelaars hun newton bij elkaar optellen
    (voorbeeld naar voren rijden & bijsturen.)

 */


// Relay Pinnen
const int thrusterPin = 3;  // Pin D3
const int pinD4 = 4;  // Pin D4

// Pins voor gemeenschappelijke anodes ("lapjes")
//Idk wat dit doet, gecopieerd van Rens.
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


//Seven segment display.
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


//Tofsensor objects.
TOFSensor tofLvoor(47);
TOFSensor tofLachter(48);
TOFSensor tofVoor(49);

//Gyro object.
GyroSensor gyro(0x68);

//Motor control objects.
StuwBlowerDriver linkerBlowerDriver(13,12); //(int PWMA_1, int PWMA_2)
StuwBlowerDriver rechterBlowerDriver(5,6); //(int PWMA_1, int PWMA_2)
SideBlowerDriver sideBlowerDriver(8,9,7,20,230,-20,-230); //(int pinIn1, int pinIn2, int pwmPin,int pwmmin_vooruit,int pwmmax_vooruit,int pwmmin_achteruit,int pwmmax_achteruit)

PWMTranslator linkerpwmtranslator(-0.00035,0.18,0.51,-0.000229,0.12,0.26); //(vooruit van hovercraft abc, achteruit tov hovercraft abc)
PWMTranslator rechterpwmtranslator(-0.000339,0.17,0.48,-0.000229,0.12,0.26);//(vooruit van hovercraft abc, achteruit tov hovercraft abc)
PWMTranslator sidepwmtranslator(-0.000282,0.03,-0.3,0.000139,0.03,-0.6); //(vooruit tov propellor abc, achteruit tov propellor abc)

Blower linkerblower(&linkerBlowerDriver,&linkerpwmtranslator);
Blower rechterblower(&rechterBlowerDriver, &rechterpwmtranslator);
Blower sideblower(&sideBlowerDriver, &sidepwmtranslator);


void setup() {
  // put your setup code here, to run once:
  Wire.begin();
  Serial.begin(115200); //Serial Monitor
  Serial1.begin(115200); //Pi Communication

  initRelays();
  digitalWrite(pinD4, HIGH);
  //digitalWrite(thrusterPin, HIGH);


  initAmperageControl();


  initTOFSensors();
  gyro.init(5);


  switchState(state_afmeren);

  resetWebsiteVariables();
}

int state = 0;

void loop() {

  //Listen for Pi Webserver input
  checkPiServerInput();

  //Always dislay amperage.
  displayAmperage();


  if(systemStopped) {
    //systemStopped is voor als de batterij onderspanning heeft.
    return;
  }


  /*
    Hierin komt de code voor de states
    Vermoedelijk in een functie of ander bestand anders word het wel heel vol en onleesbaar.
  */
  switch(UAVState){
    case state_startup:

      break;
    case state_afmeren:
      run_state_afmeren();

      break;

    case state_vooruit:

      break;

    case state_draaien:

      break;

    case state_arucomarker:

      break;
    case state_test_tofsensoren:
        test_tofsensoren(tofLachter, tofLvoor, tofVoor);
      break;
    case state_test_sidemotor:
      test_sidemotor(sideBlowerDriver);
      break;
  }

}


/*
  De nut van deze functie is dat je gecontrolleerd variabelen 1x kan aanpassen tijdens het veranderen van een state, en alles gecontrolleerd op 1 plek gebeurd.
  Als je wilt dat er wat extra gebeurd als je van state veranderd kan je dat hier toevoegen.
*/
void switchState(int state){
  switch(state){
    case state_startup:
        //idk what to do here yet.
      break;
    case state_idle:

      break;
    case state_afmeren:

      break;

    case state_vooruit:

      break;

    case state_draaien:

      break;

    case state_arucomarker:

      break;
    case state_test_tofsensoren:
      break;

    case state_test_sidemotor:
    break;


  }




    UAVState = state;
}



/*
  Init relay pinnen.
*/
void initRelays(){
  //Relay pinnen instellen.
  pinMode(thrusterPin, OUTPUT);
  pinMode(pinD4, OUTPUT);

  //Zet pinnen D3 en D4 meteen uit
  digitalWrite(thrusterPin, LOW);
  digitalWrite(pinD4, LOW);
}

/*
  Init TOF sensors.
  Ik heb de offsets gestolen van Julia's code.
  Idk of ze nog kloppen.
*/
void initTOFSensors(){
  tofLachter.initAddres(0x30);
  tofLachter.setOffset(0);

  tofLvoor.initAddres(0x31);
  tofLvoor.setOffset(0);

  tofVoor.initAddres(0x32);
  tofVoor.setOffset(0);
}

/*
  Initializers voor de ampere control modules.
*/
void initAmperageControl(){
// Anodepinnen instellen
  for (int i = 0; i < 2; i++) {
    pinMode(anodePins[i], OUTPUT);
    digitalWrite(anodePins[i], HIGH); // Anode uit (common anode)
  }

  //Dit moest van school idk why
  //Idk of het werkt of iets doet.
  analogReference(INTERNAL1V1);

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
 

//Functie om de amperage op de seven segment display te krijgen.
void displayAmperage(){
  static int currentDigit = 0; // wisselt tussen 0 en 1
  
  int raw = analogRead(A0);
  float voltage = (raw / 1023.0) * 5.0;
  float current = (voltage - 2.5) / 0.1;  // Ampère
  int displayValue = constrain(round(abs(current * 10)), 0, 99); // *10 om 1 decimaal te tonen

  // Serial.print("Raw: ");
  // Serial.print(raw);
  // Serial.print(" Voltage: ");
  // Serial.print(voltage);
  // Serial.print(" Current: ");
  // Serial.print(current);
  // Serial.print(" Display val:");
  // Serial.println(displayValue);

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


/*
  Check of de website een command heeft gestuurd en switch de state accordingly.
*/
void checkPiServerInput(){
  if (Serial1.available()) {
    String msg = Serial1.readStringUntil('\n');
    Serial.print("Pi Message received: ");

    if (msg == "1") {
      switchState(state_idle);
      Serial.println("Setstate: Idle");
    }
    else if (msg == "2") {
      switchState(state_afmeren);
      Serial.println("Setstate: Afmeren");
    }
    else if (msg == "3") {
      switchState(state_vooruit);
      Serial.println("Setstate: Vooruit");
    }
    else if (msg == "4") {
      switchState(state_draaien);
      Serial.println("Setstate: Draaien");
    }
    else if (msg == "5") {
      switchState(state_arucomarker);
      Serial.println("Setstate: Arucomarker");
    }
    else if (msg == "-1") {
      Serial.println("Toggle Thrusters.");
      digitalWrite(thrusterPin, !digitalRead(thrusterPin));
    }
  }
}

void resetWebsiteVariables(){
  Serial1.println("RESET");
  delay(10);
  Serial1.println("RESET");
  delay(10);
  
  Serial.println("Raspberry reset message send.");
}

