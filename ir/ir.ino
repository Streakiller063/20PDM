#include <QuickStats.h>

#include <DualVNH5019MotorShield.h>
DualVNH5019MotorShield md;
int IRmiddle = 4;
int IRleft = 5;// analog pin used to connect the sharp sensor
int IRright = 1;//pin 5 left
int IRfrontright = 3;//pin 4 right
int IRfrontleft = 2;//pin 3 front right
int IRLong = 0;
//pin 2 front left
volatile double distMid = 0;                 // variable to store the values from sensor(initially zero)
volatile double distLeft = 0;
volatile double distRight = 0;
volatile double distFrontRight = 0;
volatile double distFrontLeft = 0;
volatile double distLong=0;
volatile double vMid = 0;
volatile double vLeft = 0;
volatile double vRight = 0;
volatile double vFrontRight = 0;
volatile double vFrontLeft = 0;
volatile double vLong=0;
volatile double vMidAv=0;
volatile double vLeftAv=0;
volatile double vRightAv=0;
volatile double vFrontRightAv=0;
volatile double vFrontLeftAv=0;
volatile double vLongAv=0;
volatile double distance=0;

double v=0;
QuickStats stats;
float arrleft[30];
float arrright[30];
float arrmid[30];
float arrfrontleft[30];
float arrfrontright[30];
float arrlong[30];

    

void setup()
{
  Serial.begin(115200);               // starts the serial monitor
  //md.init();
}

bool equal(double x, double y){
  return fabs(x-y) < 1e-9;
}

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
// 13 25 
void loop()
{
  //md.setSpeeds(200,200);
  countVoltage();       // reads the value of the sharp sensor
  countDistance();
  Serial.println("mid distance: ");
  Serial.println(distMid);
  Serial.println("left distance: ");
  Serial.println(distLeft);
  Serial.println("right distance: ");
  Serial.println(distRight);
  Serial.println("front left distance: ");
  Serial.println(distFrontLeft);
  Serial.println("front right distance: ");// prints the value of the sensor to the serial monitor
  Serial.println(distFrontRight);
  Serial.println("Long distance: ");
  Serial.println(distLong);
  delay(1000);                    // wait for this much time before printing next value
} 
