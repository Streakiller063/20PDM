#include <PinChangeInt.h>

#include <DualVNH5019MotorShield.h>

DualVNH5019MotorShield md;
#define PinCLK_pin5 5
volatile int ticks = 0;
volatile int initi = 0;
volatile int ticks2 = 0;
volatile double speedRightNow = 0;
volatile double speedLeftNow = 0;
volatile double targetRPM = 100;
volatile int start_time = 0;  
volatile double RPMleftNow = 0;
volatile double RPMrightNow = 0;
volatile double ticks120 = 3579.39;
volatile double ticks130 = 3877.68;
volatile double ticks140 = 4474.24334;
volatile double totalticks =0;
volatile double correction = 0;
void stopIfFault() {
  if (md.getM1Fault()) {
    Serial.println("M1 fault");
    while (1);
  }
  if (md.getM2Fault()) {
    Serial.println("M2 fault");
    while (1);
  }
}
void straightLineMotion() {
  if (RPMleftNow == 0) {
    targetRPM = (speedLeftNow*0.4268)-21.33; 
  }
  else {
    targetRPM = RPMleftNow;
  }
  speedRightNow = ((targetRPM + 21.33) / 0.4268)+correction-42.5;

  /*if(initi==0){
    while(1){
    initialize();
    }
  }*/
  if(totalticks<ticks120){
    md.setM1Speed(speedRightNow);
    md.setM2Speed(speedLeftNow);
  }
  else if(totalticks>=ticks120&&totalticks<ticks130){
    md.setM1Speed(speedRightNow);
    md.setM2Speed(speedLeftNow);
    //md.setBrakes(200,200);
  }
  else if(totalticks>=ticks130 && totalticks<ticks140){
    md.setM1Speed(speedRightNow);
    md.setM2Speed(speedLeftNow);
    //md.setBrakes(300,300);
    
  }
  else{
    md.setBrakes(400,400);
  }
  stopIfFault();
  delay(50);

  RPMrightNow = (ticks / 562.25) * 1200;
  RPMleftNow = (ticks2 / 562.25) * 1200;
  Serial.println("Right = "  );
  Serial.println(RPMrightNow);
  Serial.println("Left = "  );
  Serial.println(RPMleftNow);
  correction=correct(RPMleftNow-RPMrightNow);
  ticks = 0;
  ticks2 = 0;
}
void initialize(){
  initi=1;
  md.setM1Speed(200);
  delay(30);
  md.setM2Speed(200);
  speedLeftNow=200;
  //delay(50);
  //double diff=((ticks2 / 562.25) * 1200)-((ticks / 562.25) * 1200);
  //correction= (diff+21.33)/0.4268;
  //ticks=0;
  //ticks2=0;
  //Serial.println("correction");
  //Serial.println(correction);
}
double correct(double diff){
  return (diff+21.33)/0.4268;
}
void setup() {
  Serial.begin(115200);
  md.init();
  attachInterrupt(1, rising, RISING);
  attachPinChangeInterrupt(PinCLK_pin5, rising2, RISING); //deactive when doing stepTest
  // put your setup code here, to run once:
  speedLeftNow = 200;
  initialize();
}

void loop() {
  straightLineMotion();
}

void rising() {
  ticks++;
  totalticks++;
}
void rising2() {
  ticks2++;
}
