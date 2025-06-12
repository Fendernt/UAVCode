#include "StuwPWMTranslator.h"


StuwPWMTranslator::StuwPWMTranslator(float a_voor, float b_voor, float c_voor, float a_achter, float b_achter, float c_achter){
  a1 = a_voor;
  b1 = b_voor;
  c1 = c_voor;

  a2 = a_achter;
  b2 = b_achter;
  c2 = c_achter;
  
  maxkracht_vooruit = pwmnaarstuwkracht(255);
  maxkracht_achteruit = pwmnaarstuwkracht(-255);

}
float StuwPWMTranslator::pwmnaarstuwkracht(int pwm){
float a,b,c;
float stuwkracht;
float richting;

  if(pwm == 0) return 0;


  if (pwm>0){
    a = a1;
    b = b1;
    c = c1;
    richting = 1.0;

  }
  else{
    a = a2;
    b = b2;
    c = c2;
    richting = -1.0;
    pwm = pwm * -1; //richting word later toegevoegd

  }

stuwkracht = a*pow(pwm,2)+b*pwm+c;
stuwkracht = stuwkracht * richting;

return stuwkracht;  

}
int StuwPWMTranslator::stuwkrachtnaarpwm(float stuwkracht){
float a,b,c;
int pwm =0;
int richting = 0;

  if (stuwkracht == 0) return pwm; //snelle check, bespaart rekenwerk 

    if (stuwkracht>0){
      if(stuwkracht>maxkracht_vooruit) return 255; 
      a = a1;
      b = b1;
      c = c1;

      richting  = 1;


    }
  else{
    if (stuwkracht<maxkracht_achteruit) return -255;
    a = a2;
    b = b2;
    c = c2;

    richting = -1;
    stuwkracht = stuwkracht * -1.0; //stuwkracht moet positief zijn voor de functie om te werken, richting wordt later toegevoegd
  }

pwm = -(b/(2*a) + sqrt(stuwkracht/a - c/a + pow(b,2)/(4*pow(a,2))));
pwm = pwm * richting;
return pwm;
}

void StuwPWMTranslator::printdebug(void){
  Serial.print("maxpwm, maxkracht vooruit = ");

  Serial.print(255);
  Serial.print(" , ");
  Serial.print(maxkracht_vooruit,4);
  Serial.println(" ");

  Serial.print("maxpwm, maxkracht achteruit = ");

  Serial.print(-255);
  Serial.print(" , ");
  Serial.print(maxkracht_achteruit,4);
  Serial.println(" ");
}
void StuwPWMTranslator::pwmnaarstuwkracht_test(int pwm){
    Serial.print("imput pwm:");
    Serial.print(pwm);
    Serial.print(" , ");
    Serial.print(this->pwmnaarstuwkracht(pwm),4);
}

void StuwPWMTranslator::stuwkrachtnaarpwm_test(float stuwkracht){
  Serial.print("imput stuwkracht: ");
  Serial.print(stuwkracht,4);
  Serial.print(" , ");
  Serial.println(this->stuwkrachtnaarpwm(stuwkracht));

}
