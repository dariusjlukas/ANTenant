#include <Arduino.h>
#include "main.h"

char data = 'a';
bool dataAvail = false;

void setup() {
    // put your setup code here, to run once:
    Serial.begin(115200);
}

void loop() {
    // put your main code here, to run repeatedly:
  readData();
  sendData();
}

void sendData(){
  if(dataAvail == true){
    data = data + 1;
  Serial.write(data);
  dataAvail = false;
}
}

void readData(){
  if(Serial.available() > 0){
    data = Serial.read();
    dataAvail = true;
  }
}
