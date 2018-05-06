#include <Arduino.h>
#include "main.h"

char data = 'n';
char dataOut = "p";
bool dataAvail = false;

void setup() {
    // put your setup code here, to run once:
    Serial.begin(9600);
    //Serial1.begin(230400);
    Serial1.begin(115200);
    Serial.println("Ready");
}

void loop() {
    // put your main code here, to run repeatedly:
    sendData();
    readData();
    displayData();
}

void readData(){
  if(Serial1.available() > 0){
    data = Serial1.read();
    dataAvail = true;
  }
}

void displayData(){
  if(dataAvail == true){
  Serial.println(data);
  dataAvail = false;
}
}

void sendData(){
  Serial1.write(dataOut);
}
