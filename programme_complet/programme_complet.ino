#include <SoftwareSerial.h>
#include <Adafruit_SSD1306.h>
#include <SPI.h>
//#include <Servo.h>

//#define pinServo 2
//Servo myservo;

#define flexPin A1
#define flexR_DIV 39000
#define flexflatResistance 25000
#define flexbendResistance 100000

#define bluetoothRxPin 7
#define bluetoothTxPin 8
#define baudrate 9600
#define sizeBuffer 16
SoftwareSerial mySerial(bluetoothRxPin,bluetoothTxPin);
char bufferBluetoothOutput[sizeBuffer]={0};
char bufferBluetoothInput[sizeBuffer]={0};

#define ampliPin A0

#define upButton 3
uint8_t upState;
uint8_t lastUpState=1;
long upTimer;
#define downButton 2
uint8_t downState;
uint8_t lastDownState=1;
long downTimer;
#define selectButton 4
uint8_t selectState;
uint8_t lastSelectState=1;
long selectTimer;
#define debounceDelay 50

#define nombreDePixelsEnLargeur 128         // Taille de l'écran OLED, en pixel, au niveau de sa largeur
#define nombreDePixelsEnHauteur 64          // Taille de l'écran OLED, en pixel, au niveau de sa hauteur
#define brocheResetOLED         -1
#define adresseI2CecranOLED     0x3C
Adafruit_SSD1306 ecranOLED(nombreDePixelsEnLargeur, nombreDePixelsEnHauteur, &Wire, brocheResetOLED);

#define nbOptionsPotar 9
#define nbOptionsUnite 3
const uint32_t tabChoixPotar[nbOptionsPotar]={340,1000,2000,5000,10200,20500,30600,40900,51000};
uint8_t choixPotar=4;
uint8_t choixUnite=0;
uint8_t positionMenu=0;
uint8_t selection=0;

#define MCP_NOP 0b000000000
#define MCP_WRITE 0b00010001
#define MCP_SHTDWN 0b00100001

const uint8_t ssMCPin = 10;
const int adresseChoixPotar[nbOptionsPotar]={1,4,9,50,101,102,153,204,255};

const uint8_t VCC=5;

uint8_t testEnCours=1;

void setup() {
  Serial.begin(baudrate);

  //~~ Flex Sensor ~~//
  pinMode(flexPin,INPUT);

  //~~ Ampli - capteur graphite ~~//
  pinMode(ampliPin,INPUT);

  //~~ Bluetooth ~~//
  pinMode(bluetoothRxPin,INPUT);
  pinMode(bluetoothTxPin,OUTPUT);
  mySerial.begin(baudrate);
  bufferBluetoothOutput[0]='\0';
  bufferBluetoothOutput[sizeBuffer-1]='\0';

  //~~ Boutons ~~//
  pinMode(upButton,INPUT_PULLUP);
  pinMode(downButton,INPUT_PULLUP);
  pinMode(selectButton,INPUT_PULLUP);

  //~~ OLED ~~//
  ecranOLED.begin(SSD1306_SWITCHCAPVCC, adresseI2CecranOLED);

  //~~ Potar digital ~~//
  pinMode(ssMCPin,OUTPUT);
  digitalWrite(ssMCPin,HIGH);
  SPI.begin();

  //~~ Controle Servomoteur ~~//
  //myservo.attach(pinServo);
}

void loop() {
  //if(choixPotar==4){choixPotar=3;}
  //else{choixPotar=4;}

   //~~~~~~~~~~~~~~~~ Flex Sensor ~~~~~~~~~~~~~~~~//
  int flexVolt=analogRead(flexPin);
  if(flexVolt==0)flexVolt=1;//eviter division par 0 dans la formule suivante
  float flexRes=flexR_DIV*(1023.0/flexVolt-1.0);
  float flexAngle=map(flexRes,flexflatResistance,flexbendResistance,0,90.0);

  //Serial.println("Cadeau, res : "+String(flexRes)+" ; angle : "+String(flexAngle)+"°.");
  
  //~~~~~~~~~~~~~~~~ Ampli - capteur graphite ~~~~~~~~~~~~~~~~//
  int ampliVolt=analogRead(ampliPin);

  //~~~~~~~~~~~~~~~~ Bluetooth ~~~~~~~~~~~~~~~~//
  commBluetooth(bufferBluetoothOutput,bufferBluetoothInput);
  if(bufferBluetoothInput[0]=='d'){//si l'app envoie une demande de "data"
    char unit='V';
    if(choixUnite==1)unit='O';
    if(choixUnite==2)unit='D';
    //un message composé de l'unité et de la mesure est envoyé
    sendMsg(unit,conversionMesure(ampliVolt),6);
  }
  if(bufferBluetoothInput[0]=='V'){
    choixUnite=0;
  }
  if(bufferBluetoothInput[0]=='O'){
    choixUnite=1;
  }
  if(bufferBluetoothInput[0]=='D'){
    choixUnite=2;
  }
  bufferBluetoothInput[0]='\0';
  
  //~~~~~~~~~~~~~~~~ Boutons ~~~~~~~~~~~~~~~~//
  debouncingButtons();

  //~~~~~~~~~~~~~~~~ OLED ~~~~~~~~~~~~~~~~//
  updateOLED(conversionMesure(ampliVolt),choixMesureFlex(flexVolt,flexRes,flexAngle));
  //Serial.println(ampliVolt);
  /*Serial.print(F("Potentiometre(0-4) : "));
  Serial.print(choixPotar);
  Serial.print(F(" ; Unite(0-2) : "));
  Serial.println(choixUnite);
  Serial.print(F(" ; selection(V/F) : "));
  Serial.print(selection);
  Serial.print(F(" ; positionMenu(0-2) : "));
  Serial.println(positionMenu);*/
  
  //~~~~~~~~~~~~~~~~ Potentiometre digital ~~~~~~~~~~~~~~~~//
  updatePotar();

  //~~~~~~~~~~~~~~~~ Servomoteur ~~~~~~~~~~~~~~~~//
  //protocoleTest();

  //delay(20);
}

void debouncingButtons()
{
  int readUp=digitalRead(upButton);
  int readDown=digitalRead(downButton);
  int readSelect=digitalRead(selectButton);

  if(readUp!=lastUpState){
    upTimer=millis();
    //Serial.println("ici : "+String(millis()));
  }
  if((millis()-upTimer)>debounceDelay){
    if((upState!=readUp)&&!readUp){
      upButtonAction();
      //Serial.println("ACTION UP : "+String(millis()));
    }
    upState=readUp;
  }
  lastUpState=readUp;

  if(readDown!=lastDownState){
    downTimer=millis();
  }
  if((millis()-downTimer)>debounceDelay){
    if((downState!=readDown)&&!readDown){
      downButtonAction();
      //Serial.println("ACTION DOWN : "+String(millis()));
    }
    downState=readDown;
  }
  lastDownState=readDown;

  if(readSelect!=lastSelectState){
    selectTimer=millis();
  }
  if((millis()-selectTimer)>debounceDelay){
    if((selectState!=readSelect)&&!readSelect){
      selectButtonAction();
      //Serial.println("ACTION SELECT : "+String(millis()));
    }
    selectState=readSelect;
  }
  lastSelectState=readSelect;
}

void upButtonAction()
{
  if(selection==0){
    positionMenu=(positionMenu-1+3)%3;
  }
  else{
    if(positionMenu==1){
      choixPotar=(choixPotar+1)%5;
    }
    if(positionMenu==2){
      choixUnite=(choixUnite+1)%nbOptionsUnite;
    }
  }
}

void downButtonAction()
{
  if(selection==0){
    positionMenu=(positionMenu+1)%3;
  }
  else{
    if(positionMenu==1){
      choixPotar=(choixPotar-1+nbOptionsPotar)%nbOptionsPotar;
    }
    if(positionMenu==2){
      choixUnite=(choixUnite-1+nbOptionsUnite)%nbOptionsUnite;
    }
  }
}

void selectButtonAction()
{
  if(positionMenu!=0)selection=(selection+1)%2;
}

void commBluetooth(char* bufferOutput, char* bufferInput)
{
  int i=0;
  while(mySerial.available()){
    do{
      bufferInput[i++]=(char)mySerial.read();
      delay(3);
    }while(mySerial.available()>0);
    Serial.println(bufferInput);
  }
  if(bufferOutput[0]!='\0'){
    mySerial.println(bufferOutput);
    bufferOutput[0]='\0';
  }
}

void sendMsg(char command,float data,int precision)
{//ATTENTION : cette fonction écrase le buffer output du bluetooth, toute donnée encore en attente sera perdue
  for(int i=1;i<sizeBuffer-1;i++){
    bufferBluetoothOutput[i]='/';
  }
  bufferBluetoothOutput[0]=command;
  char temp[sizeBuffer-1];
  dtostrf(data,0,precision,temp);
  strcpy(&bufferBluetoothOutput[1],temp);//1
  for(int i=1;i<sizeBuffer-1;i++){
    if(bufferBluetoothOutput[i]=='\0')bufferBluetoothOutput[i]='/';
  }
}

void updateOLED(float valeurCapGraph,float valeurFlexSensor)
{
  ecranOLED.clearDisplay();
  ecranOLED.setTextSize(1,1);
  ecranOLED.setCursor(0, 0);
  ecranOLED.setTextColor(SSD1306_WHITE);

  ecranOLED.print(F(" ~ Controle mesure ~\n\n"));
  ecranOLED.print(F("Potentio.:"));
  for(int i=5;i>compteNbCaract(tabChoixPotar[choixPotar]);i--)ecranOLED.print(' ');
  miseEnFormeMenu(1,1);
  ecranOLED.print(tabChoixPotar[choixPotar]);
  miseEnFormeMenu(1,0);
  ecranOLED.print(F(" Ohm"));

  ecranOLED.print(F("Unite : "));
  miseEnFormeMenu(2,1);
  if(choixUnite==0)ecranOLED.print(F("Volt"));
  if(choixUnite==1)ecranOLED.print(F("Ohm"));
  if(choixUnite==2)ecranOLED.print(F("Degre"));
  miseEnFormeMenu(2,2);
  ecranOLED.print(F("\n- - - - - - - - - - -"));
  ecranOLED.print(F("\nMesure :\nCap.Graph. "));
  ecranOLED.print(valeurCapGraph);
  ecranOLED.print(' ');
  if(choixUnite==0)ecranOLED.print('V');
  if(choixUnite==1)ecranOLED.print(F("MO"));
  if(choixUnite==2)ecranOLED.print(F("deg"));
  ecranOLED.print(F("\nFlexCapt.  "));
  ecranOLED.print(valeurFlexSensor);
  ecranOLED.print(' ');
  if(choixUnite==0)ecranOLED.print('V');
  if(choixUnite==1)ecranOLED.print(F("O"));
  if(choixUnite==2)ecranOLED.print(F("deg"));

  ecranOLED.display();
}

void miseEnFormeMenu(int position, int ouverture)
{
  if(position!=positionMenu){
    ecranOLED.print(" ");
  }
  else{
    if(selection==0){
      ecranOLED.print("-");
    }
    else{
      if(ouverture==1){
        ecranOLED.print("<");
      }
      else{
        ecranOLED.print(">");
      }
    }
  }
}

void SPIWrite(uint8_t cmd, uint8_t data, uint8_t ssPin)
{
  SPI.beginTransaction(SPISettings(14000000,MSBFIRST,SPI_MODE0));
  digitalWrite(ssPin,LOW);
  SPI.transfer(cmd);
  SPI.transfer(data);
  digitalWrite(ssPin,HIGH);
  SPI.endTransaction();
}

void updatePotar()
{
  SPIWrite(MCP_WRITE,adresseChoixPotar[choixPotar],ssMCPin);
}

int compteNbCaract(float cible)
{
  uint8_t compte=1;
  uint32_t comparateur=10;
  while(cible>=comparateur){
    compte++;
    comparateur*=10;
  }
  return(compte);
}

float conversionMesure(float mesure)
{
  if(choixUnite==0)return(mesure*5.0/1023.0);
  if(choixUnite==1)return(((1+100000.0/tabChoixPotar[choixPotar])*100000.0*(5.0/mesure)-110000.0)/1000000.0);
  return(-1);
}

float choixMesureFlex(float flexVolt, float flexRes, float flexAngle)
{
  if(choixUnite==0){
    return(5.0*flexVolt/1023.0);
  }
  if(choixUnite==1){
    return(flexRes);
  }
  if(choixUnite==2){
    return(flexAngle);
  }
}

/*
void protocoleTest()
{
  int pos=30*choixPotar;
  Serial.print(F("\nPos : "));
  Serial.println(pos);
  if(testEnCours)myservo.write(pos);
}*/