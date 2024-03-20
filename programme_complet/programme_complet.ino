#include <SoftwareSerial.h>
#include <Adafruit_SSD1306.h>
#include <SPI.h>

#define flexPin A1
#define flexR_DIV 39000
#define flexflatResistance 25000
#define flexbendResistance 100000

#define bluetoothRxPin 8
#define bluetoothTxPin 7
#define baudrate 9600
#define sizeBuffer 16
SoftwareSerial mySerial(bluetoothRxPin,bluetoothTxPin);
char bufferBluetoothOutput[sizeBuffer]={0};
char bufferBluetoothInput[sizeBuffer]={0};

#define ampliPin A0

#define upButton 2
int upState;
int lastUpState=1;
long upTimer;
#define downButton 4
int downState;
int lastDownState=1;
long downTimer;
#define selectButton 12
int selectState;
int lastSelectState=1;
long selectTimer;
#define debounceDelay 50

#define nombreDePixelsEnLargeur 128         // Taille de l'écran OLED, en pixel, au niveau de sa largeur
#define nombreDePixelsEnHauteur 64          // Taille de l'écran OLED, en pixel, au niveau de sa hauteur
#define brocheResetOLED         -1
#define adresseI2CecranOLED     0x3C
Adafruit_SSD1306 ecranOLED(nombreDePixelsEnLargeur, nombreDePixelsEnHauteur, &Wire, brocheResetOLED);

#define nbOptionsPotar 5
#define nbOptionsUnite 3
const int tabChoixPotar[nbOptionsPotar]={1,10,100,1000,10000};
int choixPotar=4;
int choixUnite=0;
int positionMenu=0;
int selection=0;

#define MCP_NOP 0b000000000
#define MCP_WRITE 0b00010001
#define MCP_SHTDWN 0b00100001

const int ssMCPin = 10;
const int adresseChoixPotar[nbOptionsPotar]={1,102,153,204,255};

const float VCC=5;

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
}

void loop() {
  //if(choixPotar==4){choixPotar=3;}
  //else{choixPotar=4;}

   //~~~~~~~~~~~~~~~~ Flex Sensor ~~~~~~~~~~~~~~~~//
  int temp=analogRead(flexPin);
  if(temp==0)temp=1;//eviter division par 0 dans la formule suivante
  float flexRes=flexR_DIV*(1023.0/temp-1.0);
  float flexAngle=map(flexRes,flexflatResistance,flexbendResistance,0,90.0);

  //Serial.println("Cadeau, res : "+String(flexRes)+" ; angle : "+String(flexAngle)+"°.");
  
  //~~~~~~~~~~~~~~~~ Ampli - capteur graphite ~~~~~~~~~~~~~~~~//
  int ampliVolt=analogRead(ampliPin);

  //~~~~~~~~~~~~~~~~ Bluetooth ~~~~~~~~~~~~~~~~//
  commBluetooth(bufferBluetoothOutput,bufferBluetoothInput);
  sendMsg('d',millis()+0.1234567,4);
  
  //~~~~~~~~~~~~~~~~ Boutons ~~~~~~~~~~~~~~~~//
  debouncingButtons();

  //~~~~~~~~~~~~~~~~ OLED ~~~~~~~~~~~~~~~~//
  updateOLED(positionMenu);
  Serial.println(F("Hello"));
  Serial.print(F("Potentiometre(0-4) : "));
  Serial.print(choixPotar);
  Serial.print(F(" ; Unite(0-2) : "));
  Serial.println(choixUnite);
  Serial.print(F(" ; selection(V/F) : "));
  Serial.print(selection);
  Serial.print(F(" ; positionMenu(0-2) : "));
  Serial.println(positionMenu);
  Serial.println(F("bye\n"));
  
  //~~~~~~~~~~~~~~~~ Potentiometre digital ~~~~~~~~~~~~~~~~//
  updatePotar();

  delay(20);
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
  bufferBluetoothOutput[0]=command;
  char temp[sizeBuffer-1];
  dtostrf(data,0,precision,temp);
  strcpy(&bufferBluetoothOutput[1],temp);
}

void updateOLED(int valeur)
{
  ecranOLED.clearDisplay();
  ecranOLED.setTextSize(1,1);
  ecranOLED.setCursor(0, 0);
  ecranOLED.setTextColor(SSD1306_WHITE);

  ecranOLED.print(F(" ~ Controle mesure ~\n\n"));
  ecranOLED.print(F("Potentiometre :\n         "));
  for(int i=6;i>compteNbChiffres(tabChoixPotar[choixPotar]);i--)ecranOLED.print(' ');
  miseEnFormeMenu(1,1);
  ecranOLED.print(tabChoixPotar[choixPotar]);
  miseEnFormeMenu(1,0);
  ecranOLED.print(F(" Ohm"));

  ecranOLED.print(F("Unite : "));
  miseEnFormeMenu(2,1);
  if(choixUnite==0)ecranOLED.print(F("Volt"));
  if(choixUnite==1)ecranOLED.print(F("Ohm"));
  if(choixUnite==2)ecranOLED.print(F("Ampere"));
  miseEnFormeMenu(2,2);
  ecranOLED.print(F("\n\nMesure :\n         "));
  ecranOLED.print(compteNbChiffres(tabChoixPotar[choixPotar]));
  ecranOLED.print(' ');
  if(choixUnite==0)ecranOLED.print('V');
  if(choixUnite==1)ecranOLED.print('O');
  if(choixUnite==2)ecranOLED.print('A');

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

int compteNbChiffres(int cible)
{
  uint8_t compte=1;
  uint32_t comparateur=10;
  while(cible>comparateur-1){
    compte++;
    comparateur*=10;
  }
  return(compte);
}