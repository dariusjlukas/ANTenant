// #include <Arduino.h>
//
// void setup() {
//     // put your setup code here, to run once:
//     Serial.begin(9600);
//     Serial1.begin(9600);
// }
//
// uint8_t *readLine() {
//   uint8_t buffer[250];
//   uint8_t byte = Serial1.read();
//   int count = 0;
//   while (byte != '\n') {
//     buffer[count] = byte;
//     byte = Serial1.read();
//     Serial.println(byte);
//     count += 1;
//   }
//   return buffer;
// }
//
// void loop() {
//     // put your main code here, to run repeatedly:
//     //Serial.println("Stay calm!");
//    Serial1.print("Stay calm!\n");
//     Serial.println((char*)readLine());
//     delay(500);
// }

#include <Arduino.h>
#include "main.h"

char data = 'n';
bool dataAvail = false;
bool sendOrRecieve = true;
//true is send

void setup() {
    // put your setup code here, to run once:
    Serial.begin(9600);
    Serial1.begin(230400);
    Serial.println("Ready");
}

void loop() {
    // put your main code here, to run repeatedly:
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
