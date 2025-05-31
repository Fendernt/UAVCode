#include "SevenDigitDisplay.h"

SevenDigitDisplay::SevenDigitDisplay(int a, int b, int c, int d, int e, int f, int g, int vcc) {
    this->pinNumbers[0] = a;
    this->pinNumbers[1] = b;
    this->pinNumbers[2] = c;
    this->pinNumbers[3] = d;
    this->pinNumbers[4] = e;
    this->pinNumbers[5] = f;
    this->pinNumbers[6] = g;
    this->pinNumbers[7] = vcc;

    initPinStates();
    drawDigit(0);
}


void SevenDigitDisplay::initPinStates() {
    for (int i = 0; i < 8; i++) {
      pinMode(pinNumbers[i], OUTPUT);
      digitalWrite(pinNumbers[i], HIGH);
    }
}

void SevenDigitDisplay::drawDigit(int digit) {
    for (int i = 0; i < 8; i++) {
      digitalWrite(pinNumbers[i], digits[digit][i]);
    }

    digitalWrite(pinNumbers[7], this->enabled);
}

void SevenDigitDisplay::setEnabled(int enabled) {
    this->enabled = enabled;
}