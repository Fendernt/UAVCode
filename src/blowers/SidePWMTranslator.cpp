#include "SidePWMTranslator.h"


SidePWMTranslator::SidePWMTranslator(float a_voor, float b_voor, float c_voor, float a_achter, float b_achter, float c_achter){

  a1 = a_voor;
  b1 = b_voor;
  c1 = c_voor;

  a2 = a_achter;
  b2 = b_achter;
  c2 = c_achter;
  
  minpwm_vooruit = 20;
  maxpwm_vooruit = 230;

  minkracht_vooruit = pwmnaarstuwkracht(minpwm_vooruit);
  maxkracht_vooruit = pwmnaarstuwkracht(maxpwm_vooruit);

  minpwm_achteruit = -20;
  maxpwm_achteruit = -230;
  minkracht_achteruit = pwmnaarstuwkracht(minpwm_achteruit);
  maxkracht_achteruit = pwmnaarstuwkracht(maxpwm_achteruit);

}
float SidePWMTranslator::pwmnaarstuwkracht(int pwm){
float a,b,c;
float stuwkracht;
float richting = 0.0;

  if(pwm == 0) return 0;


  if (pwm>0){
    //if (pwm < minpwm_vooruit) return minkracht_vooruit;
    //if (pwm > maxpwm_vooruit) return maxkracht_vooruit;
    a = a1;
    b = b1;
    c = c1;
    richting = 1.0;

  }
  else{
   // if (pwm > minpwm_achteruit) return minkracht_achteruit; // alles negatief dus signs omgedraaid
   // if (pwm < maxpwm_achteruit) return maxkracht_achteruit; //alles negatief dus signs omgedraaid
    a = a2;
    b = b2;
    c = c2;
    richting = -1.0;
    pwm = pwm * -1; //pwm moet positief zijn voor de functie, richting wordt later toegevoegd.
  }

stuwkracht = (a*pow(pwm,2)+b*pwm+c)* richting;

return stuwkracht;  

}
int SidePWMTranslator::stuwkrachtnaarpwm(float stuwkracht){
float a,b,c;
int pwm =0;
int richting = 0;

  if (stuwkracht == 0) return 0; //snelle check, bespaart rekenwerk 

    if (stuwkracht>0){ //vooruit
      a = a1;
      b = b1;
      c = c1;
      if (stuwkracht <= minkracht_vooruit) return minpwm_vooruit;
      if(stuwkracht >= maxkracht_vooruit) return maxpwm_vooruit;

      pwm = abs(b/(2*a)) + sqrt(stuwkracht/a - c/a + pow(b,2)/(4*pow(a,2))) - 106;
      return pwm;
    }
  else{
    a = a2;
    b = b2;
    c = c2;
    
    if(stuwkracht >= minkracht_achteruit) return minpwm_achteruit;
    if(stuwkracht <= maxkracht_achteruit) return maxpwm_achteruit;

    stuwkracht = abs(stuwkracht); //stuwkracht moet postief voor de functie, richting wordt later toegevoegd.
    pwm = abs(b/(2*a)) + sqrt(stuwkracht/a - c/a + pow(b,2)/(4*pow(a,2))) - 220.2;
    pwm = pwm * -1;
    return pwm;


  }


}

void SidePWMTranslator::printdebug(void){
  Serial.print("minpwm, maxpwm, minkracht, maxkracht vooruit = ");
  Serial.print(minpwm_vooruit);
  Serial.print(" , ");
  Serial.print(maxpwm_vooruit);
  Serial.print(" , ");
  Serial.print(minkracht_vooruit,4);
  Serial.print(" , ");
  Serial.print(maxkracht_vooruit,4);
  Serial.println(" ");

  Serial.print("minpwm, maxpwm, minkracht, maxkracht achteruit = ");
  Serial.print(minpwm_achteruit);
  Serial.print(" , ");
  Serial.print(maxpwm_achteruit);
  Serial.print(" , ");
  Serial.print(minkracht_achteruit,4);
  Serial.print(" , ");
  Serial.print(maxkracht_achteruit,4);
  Serial.println(" ");
}
void SidePWMTranslator::pwmnaarstuwkracht_test(int pwm){
    Serial.print("input pwm:");
    Serial.print(pwm);
    Serial.print(" , ");
    Serial.print(this->pwmnaarstuwkracht(pwm),4);
}

void SidePWMTranslator::stuwkrachtnaarpwm_test(float stuwkracht){
  Serial.print("input stuwkracht: ");
  Serial.print(stuwkracht,4);
  Serial.print(" , ");
  Serial.println(this->stuwkrachtnaarpwm(stuwkracht));

}