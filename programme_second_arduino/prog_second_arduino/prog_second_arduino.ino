
#include <Servo.h>
Servo myservo;

#define pinServo 9
#define nbSteps 25
#define angleMaxServo 120
#define pinTx 11
#define pinRx 12
#define delayPalierMesure 4000

int tableangleservo[nbSteps]={0};
int pasServo=angleMaxServo/(nbSteps-1);
uint8_t indexServo=0;
uint8_t testencours=0;

uint8_t valueTx=0;
uint8_t lastRx=0;
int readRx;

void setup(){
  myservo.attach(pinServo);
  myservo.write(0);
  for(indexServo=0;indexServo<nbSteps;indexServo++) 
  {
    tableangleservo[indexServo]=indexServo*pasServo;
  }
  pinMode(pinTx,OUTPUT);
  pinMode(pinRx,INPUT);
  indexServo=0;
  Serial.begin(9600);
  Serial.println("test");
  //Serial.println(pasServo);
}

void loop() {
  if((testencours==1)&&(indexServo<nbSteps)){
    delay(delayPalierMesure);
    myservo.write(tableangleservo[indexServo]);
    //Serial.println((int)tableangleservo[indexServo]);
    indexServo++;
  }
  else{
    indexServo=0;
    testencours=0;
    myservo.write(0);
  }
  syncTest();
}

void syncTest()
{
  //uint8_t readRx;
  readRx=digitalRead(pinRx);
  Serial.print("Comm : T-R ");
  Serial.print(valueTx);
  Serial.print(' ');
  Serial.println(readRx);
  Serial.print("Actua : i t ");
  Serial.print(indexServo);
  Serial.print(' ');
  Serial.println(testencours);
  if(readRx==1){
    testencours=1;
    valueTx=1;
  }
  if(testencours==0){
    valueTx=0;
  }
  digitalWrite(pinTx,valueTx);
}