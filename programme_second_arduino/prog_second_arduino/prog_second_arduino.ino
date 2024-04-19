
#include <Servo.h>
Servo myservo;

int angletotservo 270;
int nbrepts 27;
char tableangleservo[nbrepts]={0};
int paservo=angletotservo/nbrpts;
int indexservo=0;
int testencours=0;

void setup() {
myservo.attach(9);

for(indexservo=0;indexservo<nbrepts;indexservo++) 
{
  tableangleservo[indexservo]=tableangleservo[indexservo-1]+paservo;
}
  indexservo=0;
}

void loop() {
  if(testencours=1){
    myservo.write(tableangleservo[indexservo]);
    delay(500);
    indexservo++;
}

}
