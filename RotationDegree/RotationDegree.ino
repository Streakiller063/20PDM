#include <PinChangeInt.h>

#include <DualVNH5019MotorShield.h>

volatile int ticks1 = 0;
volatile int ticks2 = 0;
volatile int setDegree =900;
volatile int targetTicks=0;
DualVNH5019MotorShield md;
int PinCLK_pin5 = 5;




void setup() {

  Serial.begin(115200);
  targetTicks= (setDegree*1.0/360)*(10/3)*562.25+20;
  Serial.println(targetTicks);
  md.init();
//  Serial.println(targetTicks);
  attachInterrupt(1, rising, RISING);
  attachPinChangeInterrupt(PinCLK_pin5, rising2, RISING); //deactive when doing stepTest
  
  // put your setup code here, to run once:

}

/*int countTargetTicks(int setDegree){
  Serial.println(setDegree);
  float returnValue = (setDegree/360)*(0.1)/(0.03);
  float rr = setDegree/360;
  Serial.println(rr);
  return returnValue*562.25;
}*/
void loop() {
  // put your main code here, to run repeatedly:
  if(ticks1<targetTicks && ticks2<targetTicks){
    md.setSpeeds(-200,200);
  }
  else{
    md.setBrakes(400,400);
  }

}

void rising(){
  ticks1++;
}

void rising2(){
  ticks2++;
}

