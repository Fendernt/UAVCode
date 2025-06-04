#ifndef MOTOR_H
#define MOTOR_H

#include <Arduino.h>

class Motor{
/****************************************************************************************************************************************************************************
De class Motor wordt geinitialiseerd door de abc waardes uit de trendlijn van de pwm / stuwkracht meting in te vullen,
a,b,c_voor of a1,b1,c1 refereerd naar de abc coeficienten voor de kracht vooruit
a,b,c_achter of a2,b2,c2 refereerd naar de abc coeficenten voor de stuwkracht achteruit

deze zes waardes zijn als private gedeclareerd omdat alleen de twee memberfuncties deze zullen gebruiken.

De twee memberfuncties, pwmnaarstuwkracht en gramnaarpwm. Regelen de omzetting van stuwkracht naar pwm en vice versa.
in deze memberfuncties zal je steeds de omzetting zien van a1,b1,c1 of a2,b2,c2 naar de lokale waarde a,b,c afhankelijk of vooruit True of False is.
richting true? -> a1,b1,c1 naar a,b,c
richting false? -> a2,b2,c3 naar a,b,c
  
Daarnaast heeft de class een friend functie, dit is een void functie die via serial communicatie (9600 Baud) verwachtte en actuele waardes teruggeeft, handig als controle.
Om deze friend functie te laten werken moet de maximale en minimale stuwkracht als protected gedeclareerd worden.

Bij initialisatie worden via de pwmnaarstuwkracht methode twee stuwkrachten berekend, één bij 255 pwm vooruit en één bij 255 pwm achteruit. 
Deze twee waardes zijn nodig om te weten of een gevraagde kracht ook echt geleverd kan worden.
  
*****************************************************************************************************************************************************************************/
public:
    Motor(float a_voor, float b_voor, float c_voor, float a_achter, float b_achter, float c_achter);

float pwmnaarstuwkracht(int pwm,bool richting);
int gramnaarpwm(float stuwkracht, bool);

  friend void pwmnaargram_unittest(Motor& testmotor);
  
  private:
  float a1,b1,c1; //vooruit
  float a2,b2,c2; //achteruit
  
  protected:
  float maxkracht_vooruit,maxkracht_achteruit;

};


#endif