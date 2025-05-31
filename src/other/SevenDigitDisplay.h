#ifndef SEVENDIGITDISPLAY_H
#define SEVENDIGITDISPLAY_H

#include <Arduino.h>

class SevenDigitDisplay {
    public:
        SevenDigitDisplay(int a, int b, int c, int d, int e, int f, int h, int vcc);
        void drawDigit(int digit);
        void setEnabled(int enabled);

    private:
        void initPinStates();

        int currentNumber;
        int pinNumbers[8] = {};

        int enabled = 1;

        const byte digits[10][8] = {
        {LOW, LOW, LOW, LOW, LOW, LOW, HIGH, HIGH},    // 0
        {HIGH, LOW, LOW, HIGH, HIGH, HIGH, HIGH, HIGH}, // 1
        {LOW, LOW, HIGH, LOW, LOW, HIGH, LOW, HIGH},    // 2
        {LOW, LOW, LOW, LOW, HIGH, HIGH, LOW, HIGH},    // 3
        {HIGH, LOW, LOW, HIGH, HIGH, LOW, LOW, HIGH},   // 4
        {LOW, HIGH, LOW, LOW, HIGH, LOW, LOW, HIGH},    // 5
        {LOW, HIGH, LOW, LOW, LOW, LOW, LOW, HIGH},     // 6
        {LOW, LOW, LOW, HIGH, HIGH, HIGH, HIGH, HIGH},  // 7
        {LOW, LOW, LOW, LOW, LOW, LOW, LOW, HIGH},      // 8
        {LOW, LOW, LOW, LOW, HIGH, LOW, LOW, HIGH}      // 9
        };
};


#endif