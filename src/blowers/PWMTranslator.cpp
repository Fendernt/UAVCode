#include "PWMTranslator.h"


PWMTranslator::PWMTranslator(float a_voor, float b_voor, float c_voor, float a_achter, float b_achter, float c_achter){
  a1 = a_voor;
  b1 = b_voor;
  c1 = c_voor;

  a2 = a_achter;
  b2 = b_achter;
  c2 = c_achter;
  
  maxkracht_vooruit = stuwkrachtnaarpwm(255);
  maxkracht_achteruit = stuwkrachtnaarpwm(-255);

}
float PWMTranslator::pwmnaarsuwkracht(int pwm){
float a,b,c;
float stuwkracht;

  if(pwm == 0) return 0;


  if (pwm>0){
    a = a1;
    b = b1;
    c = c1;

  }
  else{
    a = a2;
    b = b2;
    c = c2;

  }

pwm = abs(pwm);
stuwkracht = a*pow(pwm,2)+b*pwm+c;

return stuwkracht;  

}
int PWMTranslator::stuwkrachtnaarpwm(float stuwkracht){
float a,b,c;
int pwm =0;
int richting = 0;

  if (stuwkracht <= 0) return pwm; //snelle check, bespaart rekenwerk 

    if (stuwkracht>0){
    a = a1;
    b = b1;
    c = c1;

    richting  = 1;

    if(stuwkracht>=maxkracht_vooruit) return 255; 

    }
  else{
    a = a2;
    b = b2;
    c = c2;

    richting = -1;
    
    if (stuwkracht<=maxkracht_achteruit) return -255;

  }

pwm = abs(b/(2*a) + sqrt(stuwkracht/a - c/a + pow(b,2)/(4*pow(a,2)))) * richting *0.00981;



return pwm;
}        