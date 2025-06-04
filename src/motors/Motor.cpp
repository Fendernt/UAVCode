#include "Motor.h"

float Motor::pwmnaarstuwkracht(int pwm, bool richting){
  float a,b,c;
  float stuwkracht;

  if(pwm == 0) return 0;


  if (richting){
    a = a1;
    b = b1;
    c = c1;

  }
  else{
    a = a2;
    b = b2;
    c = c2;

  }

stuwkracht = a*pow(pwm,2)+b*pwm+c;
return stuwkracht;

}
int Motor::gramnaarpwm(float stuwkracht, bool richting){
  float a,b,c;
  int pwm =0;

  if (stuwkracht <= 0) return pwm; //snelle check, bespaart rekenwerk 

    if (richting){
    a = a1;
    b = b1;
    c = c1;
    
    if(stuwkracht>=maxkracht_vooruit) return 255; 

    }
  else{
    a = a2;
    b = b2;
    c = c2;

    if (stuwkracht>=maxkracht_achteruit) return 255;
  }

  pwm = abs(b/(2*a) + sqrt(stuwkracht/a - c/a + pow(b,2)/(4*pow(a,2))));

  return pwm;
}

void pwmnaargram_unittest(Motor& testmotor){
  Serial.println("Motor unittest!");

  //test nul gram voor en achteruit
  Serial.print("0.0 gram vooruit = ");
  Serial.print(testmotor.gramnaarpwm(0.0, true));
  Serial.println(" verwachtte 0 pwm");
  
  Serial.print("0.0 gram achteruit = ");
  Serial.print(testmotor.gramnaarpwm(0.0, false));
  Serial.println(" verwachtte 0 pwm");

  //test -10 gram voor en achteruit
  Serial.print("-10.0 gram vooruit = ");
  Serial.print(testmotor.gramnaarpwm(-10.0, true));
  Serial.println(" verwachtte 0 pwm");
  
  Serial.print("-10.0 gram achteruit = ");
  Serial.print(testmotor.gramnaarpwm(-10.0, false));
  Serial.println(" verwachtte 0 pwm");

  //test 1000 gram voor en achteruit
  Serial.print("1000.0 gram vooruit = ");
  Serial.print(testmotor.gramnaarpwm(1000.0, true));
  Serial.println(" verwachtte 255");


  Serial.print("1000.0 gram achteruit = ");
  Serial.print(testmotor.gramnaarpwm(1000.0, false));
  Serial.println(" verwachtte 255");

  //test van 1.0 gram tot en met 24.0 gram vooruit
  for(float i=1.0 ;i<25.0;i+=1.0){
  Serial.print(i);
  Serial.print(" gram vooruit = ");
  Serial.print(testmotor.gramnaarpwm(i, true));
  Serial.print(" pwm, dit komt overeen met ");
  Serial.print(testmotor.pwmnaarstuwkracht(testmotor.gramnaarpwm(i,true), true));
  Serial.println(" gram");
  }
  
  //test van 1.0 gram tot en met 24.0 gram achteruit
  for(float i=1.0 ;i<25.0;i+=1.0){
  Serial.print(i);
  Serial.print(" gram achteruit = ");
  Serial.print(testmotor.gramnaarpwm(i, false));
  Serial.print(" pwm, dit komt overeen met ");
  Serial.print(testmotor.pwmnaarstuwkracht(testmotor.gramnaarpwm(i,false),false));
  Serial.println(" gram");
  }

}