#include <PinChangeInt.h>
#include <QuickStats.h>
QuickStats stats;
#include <DualVNH5019MotorShield.h>
DualVNH5019MotorShield md;

//Infrared Sensors Variables
//Each number refers to Analog Port number
int IRmiddle = 4;
int IRleft = 5;
int IRright = 1;
int IRfrontright = 3;
int IRfrontleft = 2;
int IRLong = 0;
//Variable for Distance
volatile double distMid = 0;                 
volatile double distLeft = 0;
volatile double distRight = 0;
volatile double distFrontRight = 0;
volatile double distFrontLeft = 0;
volatile double distLong=0;
volatile double distance=0;
//Variable for Instaneous voltage
volatile double vMid = 0;
volatile double vLeft = 0;
volatile double vRight = 0;
volatile double vFrontRight = 0;
volatile double vFrontLeft = 0;
volatile double vLong=0;
//Variable for Average voltage
volatile double vMidAv=0;
volatile double vLeftAv=0;
volatile double vRightAv=0;
volatile double vFrontRightAv=0;
volatile double vFrontLeftAv=0;
volatile double vLongAv=0;
//Arrays used to store instaneous voltage
float arrleft[30];
float arrright[30];
float arrmid[30];
float arrfrontleft[30];
float arrfrontright[30];
float arrlong[30];

//Variables for Motor
#define PinCLK_pin5 5
volatile int ticks = 0;
volatile int initi = 0;
volatile int ticks2 = 0;
volatile int ticksTemp = 0;
volatile int ticks2Temp = 0;
volatile double speedRightNow = 0;
volatile double speedLeftNow = 200;
volatile double targetRPM = 100;
volatile int start_time = 0;  
volatile double RPMleftNow = 0;
volatile double RPMrightNow = 0;
volatile double totalticks =0;
volatile double totaltickstemp=0;
volatile double correction = 0;
volatile double targetTicks=0;

void setup() {
  Serial.begin(115200);
  md.init();
  attachInterrupt(1, rising, RISING);
  attachPinChangeInterrupt(PinCLK_pin5, rising2, RISING);
  // put your setup code here, to run once:

}

void loop() {
  //continuously gets IR sensor reading
  countVoltage();
  countDistance();
  /*forward();
  forward();
  forward();
  forward();
  forward();
  forward();
  forward();
  forward();*/
  //forwardDistance(50);
  rotateLeft(45);
  forwardDistance(50);
  rotateLeft(90);
  forwardDistance(50);
  brake();
  
  delay(10000);
  // put your main code here, to run repeatedly:

}

//This section covers IR Sensor Detection Methods

void countVoltage(){
  for(int i=0;i<30;i++){
    arrleft[i]=analogRead(IRleft);
    arrright[i]=analogRead(IRright);
    arrmid[i]=analogRead(IRmiddle);
    arrfrontleft[i]=analogRead(IRfrontleft);
    arrfrontright[i]=analogRead(IRfrontright);
    arrlong[i]=analogRead(IRLong);
  }
  stats.bubbleSort(arrleft,30);
  stats.bubbleSort(arrright,30);
  stats.bubbleSort(arrmid,30);
  stats.bubbleSort(arrfrontleft,30);
  stats.bubbleSort(arrfrontright,30);
  stats.bubbleSort(arrlong,30);
  
  vMidAv=stats.median(arrmid,30)*0.0048828125;
  vLeftAv=stats.median(arrleft,30)*0.0048828125;
  vRightAv=stats.median(arrright,30)*0.0048828125;
  vFrontRightAv=stats.median(arrfrontright,30)*0.0048828125;
  vFrontLeftAv=stats.median(arrfrontleft,30)*0.0048828125;
  vLongAv= stats.median(arrlong,30)*0.0048828125;
  vMid=0;
  vLeft=0;
  vRight=0;
  vFrontRight=0;
  vFrontLeft=0;
  vLong=0;
  distance=0;
}
//Get the distance IR measured by taking the Average voltage output
void countDistance(){
  distMid = ShortDistance(vMidAv);
  distLeft = ShortDistance(vLeftAv);
  distRight = ShortDistance(vRightAv);
  distFrontRight = ShortDistance(vFrontRightAv);
  distFrontLeft = ShortDistance(vFrontLeftAv);
  distLong = LongDistance(vLongAv);
}

double LongDistance(double voltage){
  if(voltage<=2.759 && voltage>=2.5){
    distance =-24.111*voltage + 81.522;
  }
  else if(voltage<2.5 && voltage>=2){
    distance = -47.471*voltage + 128.91;
  }
  else if(voltage<2 && voltage>=1.552){
    distance = -21.276*voltage + 72.648;
  }
  else if(voltage<1.552 && voltage>=1.241){
    distance = -33.226*voltage + 90.985;
  }
  else if(voltage<1.241 && voltage>=1.052){
    distance = -50.246*voltage + 112.41;
  }
  else if(voltage<1.052 && voltage>=0.897){
    distance = -72.569*voltage + 135.69;
  }
  else if(voltage<0.897 && voltage>=0.81){
    distance = -109.77*voltage + 169.36;
  }
  else if(voltage<0.81 && voltage>=0.707){
    distance = -109.84*voltage + 169.42;
  }
  else if(voltage<0.707 && voltage>=0.638){
    distance = -150.58*voltage + 198.57;
  }
  else if(voltage<0.638 && voltage>=0.534){
    distance = -168.27*voltage + 209.86;
  }
  else if(voltage<0.534&&voltage>=0.431){
    distance = -291.26*voltage + 275.53;
  }
  else{
    distance = -1;
  }
}


double ShortDistance(double voltage){
  if(voltage<=3 && voltage>=2.239){
    distance =-5.2562*voltage + 21.769;
  }
  else if(voltage<2.239 && voltage>=1.63){
    distance = -6.5681*voltage + 24.706;
  }
  else if(voltage<1.63 && voltage>=1.2893){
    distance = -17.291*voltage + 42.184;
  }
  else if(voltage<1.2893 && voltage>=1.065){
    distance = -20.642*voltage + 46.484;
  }
  else if(voltage<1.065 && voltage>=0.8913){
    distance = -31.664*voltage + 58.222;
  }
  else if(voltage<0.8913 && voltage>=0.7174){
    distance = -57.504*voltage + 81.254;
  }
  else if(voltage<0.7174 && voltage>=0.587){
    distance = -76.687*voltage + 95.015;
  }
  /*else if(voltage<0.587 && voltage>=0.4783){
    distance = -91.996*voltage + 104;
  }
  else if(voltage<0.4783 && voltage>=0.3913){
    distance = -114.94*voltage + 114.98;
  }
  else if(voltage<0.3913 && voltage>=0.3696){
    distance = -1;
  }*/
  else{
    distance = -1;
  }
}
//End of IR Method
//Methods for Motor Control

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

void brake(){
  md.setBrakes(400,400);
}

//correction factor between left and right motors
double correct(double diff){
  return (diff+21.33)/0.4268;
}
//start the motors to move forward
void initializeForward(){
  initi=1;
  md.setM1Speed(200);
  delay(60);
  md.setM2Speed(200);
  speedLeftNow=200;
  RPMleftNow = (ticks2 / 562.25) * 1200;

}
//start the motors to move backward
void initializeBackward(){
  initi=1;
  md.setM1Speed(-200);
  delay(30);
  md.setM2Speed(-200);
  speedLeftNow=-200;
}
//Robot goes forward for specified distance
void forwardDistance(double distance){
  targetTicks = countTicks(distance);
  ticks=0;
  ticks2=0;
  initializeForward();
  while(ticks<targetTicks && ticks2<targetTicks){
    straightLineMotion(1); 
    Serial.println("ticks: ");
    Serial.println(ticks);
  }
  brake();
  RPMrightNow = 0;
  RPMleftNow = 0;
  ticks=0;
  ticks2=0;
  
}
//Robot goes backward for specified distance
void backwardDistance(double distance){
  targetTicks = countTicks(distance);
  ticks=0;
  ticks2=0;
  initializeBackward();
  while(ticks<targetTicks && ticks2<targetTicks){
    straightLineMotion(0);
  }
  brake();
  RPMrightNow = 0;
  RPMleftNow = 0;
  ticks=0;
  ticks2=0;
  
}
//Robot goes forward for 10cm
void forward(){
  targetTicks = countTicks(10);
  Serial.println(targetTicks);
  ticks=0;
  ticks2=0;
  //initializeForward();
  md.setSpeeds(60,88);
  while(ticks<targetTicks && ticks2<targetTicks){
    
    md.setSpeeds(186,200);
    //delay(100);
    //md.setM1Speed(200);
    //md.setSpeeds(190,200);
  }
  brake();
  ticks=0;
  ticks2=0;
  RPMrightNow = 0;
  RPMleftNow = 0;
}

//Robot goes backward for 10cm
void backward(){
  targetTicks = countTicks(10);
  ticks=0;
  ticks2=0;
  initializeBackward();
  while(ticks<targetTicks && ticks2<targetTicks){
    md.setSpeeds(-190,-200);
  }
  brake();
  ticks=0;
  ticks2=0;
  RPMrightNow = 0;
  RPMleftNow = 0;
  
}

void left(){
  targetTicks = countTicksDegree(90);
  totaltickstemp=totalticks;
  ticks=0;
  ticks2=0;
  while(ticks<targetTicks && ticks2<targetTicks){
    md.setSpeeds(200,-200);
  }
  brake();
  totalticks=totaltickstemp;
  ticks=0;
  ticks2=0;
}


void right(){
  targetTicks = countTicksDegree(90);
  totaltickstemp=totalticks;
  ticks=0;
  ticks2=0;
  while(ticks<targetTicks && ticks2<targetTicks){
    md.setSpeeds(-200,200);
  }
  brake();
  totalticks=totaltickstemp;
  ticks=0;
  ticks2=0;
}

void rotateLeft(double degree){
  targetTicks = countTicksDegree(degree);
  totaltickstemp=totalticks;
  ticks=0;
  ticks2=0;
  while(ticks<targetTicks && ticks2<targetTicks){
    md.setSpeeds(200,-200);
  }
  brake();
  totalticks=totaltickstemp;
  ticks=0;
  ticks2=0;
}

void rotateRight(double degree){
  targetTicks = countTicksDegree(degree);
  Serial.println(targetTicks);
  totaltickstemp=totalticks;
  ticks=0;
  ticks2=0;
  while(ticks<targetTicks && ticks2<targetTicks){
    md.setSpeeds(-200,200);
  }
  brake();
  totalticks=totaltickstemp;
  ticks=0;
  ticks2=0;
}

void straightLineMotion(int option) {
  ticksTemp=ticks;
  ticks2Temp=ticks2;
  ticks=0;
  ticks2=0;
  targetRPM = RPMleftNow;


  /*if (RPMleftNow == 0) {
    targetRPM = (200*0.4268)-21.33; 
  }
  else {
    targetRPM = RPMleftNow;
  }*/
  //speedRightNow=200;
  speedRightNow = ((targetRPM + 21.33) / 0.4268)+correction-35.4;


  if(option==1){
    
    md.setSpeeds(speedRightNow,speedLeftNow);
  }
  else{
    md.setSpeeds(-speedRightNow,-speedLeftNow);
  }

  stopIfFault();
  delay(50);

  RPMrightNow = (ticks / 562.25) * 1200;
  RPMleftNow = (ticks2 / 562.25) * 1200;


  correction=correct(RPMleftNow-RPMrightNow);

  ticks = ticks+ticksTemp;
  ticks2 = ticks2+ticksTemp;

}

//Count the number of ticks needed for a straight line motion
double countTicks(int distance){
  return (distance*1.0/(2*3.141592654*3))*562.25;
}
//Count the ticks needed to manuvere input degree
double countTicksDegree(double setDegree){
  return (setDegree*1.0/360)*(10/3)*562.25+10;
}
//Returns the distance travelled so far
double distanceTravelled(){
  return (totalticks*1.0/562.25)*(2*3.141592654*3);
}
//Interrupt Service Routine is triggerred every rising edge of the encoder square wave
void rising() {
  ticks++;
  totalticks++;
}
void rising2() {
  ticks2++;
}
