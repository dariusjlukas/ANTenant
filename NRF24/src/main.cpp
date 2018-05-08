/*
TMRh20 2014

 This program is free software; you can redistribute it and/or
 modify it under the terms of the GNU General Public License
 version 2 as published by the Free Software Foundation.
 */

/** General Data Transfer Rate Test
 * This example demonstrates basic data transfer functionality with the
 updated library. This example will display the transfer rates acheived using
 the slower form of high-speed transfer using blocking-writes.
 */


#include <SPI.h>
#include <RF24.h>
#include <Arduino.h>

/*************  USER Configuration *****************************/
                                          // Hardware configuration
RF24 radio(7,A6);                     // Set up nRF24L01 radio on SPI bus plus pins 7 & 8

/***************************************************************/

byte addresses[][6] = {"1Node","2Node"};

byte data[32];                           //Data buffer for testing data transfer speeds

unsigned long counter, rxTimer;          //Counter and timer for keeping track transfer info
unsigned long startTime, stopTime;
bool TX=1,RX=0,role=0;

bool radioNumber = 0;

void setup(void) {

  pinMode(2, INPUT);
  pinMode(A6,OUTPUT);

  SPI.setMOSI(11);
  SPI.setMISO(12);
  SPI.setSCK(A0);

  Serial.begin(115200);

    pinMode(13,OUTPUT);
    int w = 50;
    while (!Serial.available() and --w > 0) {
        digitalWrite(13, !digitalRead(13));
        delay(100);
    }
    digitalWrite(13, HIGH);

  radio.begin();                           // Setup and configure rf radio
  radio.setChannel(1);
  radio.setPALevel(RF24_PA_MAX);
  radio.setDataRate(RF24_2MBPS);

  radio.setAutoAck(1);                     // Ensure autoACK is enabled
  //radio.setAutoAck(0);

  //radio.setRetries(2,15);                  // Optionally, increase the delay between retries & # of retries
  radio.setRetries(2,5);                  // Optionally, increase the delay between retries & # of retries

  radio.setCRCLength(RF24_CRC_8);          // Use 8-bit CRC for performance
  radio.openWritingPipe(addresses[0]);
  radio.openReadingPipe(1,addresses[1]);

    role = digitalRead(2);
    if (role == HIGH) {
        Serial.println("TX Begin");

    } else {
        Serial.println("RX Begin");

    }

  // Open a writing and reading pipe on each radio, with opposite addresses
  if(role == TX){
    radio.openWritingPipe(addresses[1]);
    radio.openReadingPipe(1,addresses[0]);
    radio.stopListening();
  }else{
    radio.openWritingPipe(addresses[0]);
    radio.openReadingPipe(1,addresses[1]);
    radio.startListening();                 // Start listening
  }


  radio.printDetails();                   // Dump the configuration of the rf unit for debugging

  Serial.println(F("\n\rRF24/examples/Transfer/"));
  Serial.println(F("*** PRESS 'T' to begin transmitting to the other node"));

  randomSeed(analogRead(0));              //Seed for random number generation

  for(int i=0; i<32; i++){
     data[i] = random(255);               //Load the buffer with random data
  }
  radio.powerUp();                        //Power up the radio
}

void loop(void){


  if(role == TX){

    //delay(500);

    Serial.println(F("Initiating Basic Data Transfer"));


    unsigned long cycles = 10000; //Change this to a higher or lower number.

    startTime = millis();
    unsigned long pauseTime = millis();

    for(unsigned long i=0; i<cycles; i++){        //Loop through a number of cycles
      data[0] = i;                      //Change the first byte of the payload for identification
      if(!radio.writeFast(&data,32)){   //Write to the FIFO buffers
        counter++;                      //Keep count of failed payloads
      }

      //This is only required when NO ACK ( enableAutoAck(0) ) payloads are used
//      if(millis() - pauseTime > 3){
//        pauseTime = millis();
//        radio.txStandBy();          // Need to drop out of TX mode every 4ms if sending a steady stream of multicast data
//        //delayMicroseconds(130);     // This gives the PLL time to sync back up
//      }

    }

   stopTime = millis();
                                         //This should be called to wait for completion and put the radio in standby mode after transmission, returns 0 if data still in FIFO (timed out), 1 if success
   if(!radio.txStandBy()){ counter+=3; } //Standby, block only until FIFO empty or auto-retry timeout. Flush TX FIFO if failed
   //radio.txStandBy(1000);              //Standby, using extended timeout period of 1 second

   float numBytes = cycles*32;
   float rate = numBytes / (stopTime - startTime);

   Serial.print("Transfer complete at "); Serial.print(rate); Serial.println(" KB/s");
   Serial.print(counter); Serial.print(" of "); Serial.print(cycles); Serial.println(" Packets Failed to Send");
   counter = 0;

   }



if(role == RX){
     while(radio.available()){
      radio.read(&data,32);
      counter++;
     }
   if(millis() - rxTimer > 1000){
     rxTimer = millis();
     unsigned long numBytes = counter*32;
     Serial.print(F("Rate KB/s: "));
     //Prevent dividing into 0, which will cause issues over a period of time
     Serial.println(numBytes > 0 ? numBytes/1000.0:0);
     Serial.print(F("Payload Count: "));
     Serial.println(counter);
     counter = 0;
   }
  }
  //
  // Change roles
  //

  if ( Serial.available() )
  {
    char c = toupper(Serial.read());
    if ( c == 'T' && role == RX )
    {
      Serial.println(F("*** CHANGING TO TRANSMIT ROLE -- PRESS 'R' TO SWITCH BACK"));
      radio.openWritingPipe(addresses[1]);
      radio.openReadingPipe(1,addresses[0]);
      radio.stopListening();
      role = TX;                  // Become the primary transmitter (ping out)
    }
    else if ( c == 'R' && role == TX )
    {
      radio.openWritingPipe(addresses[0]);
      radio.openReadingPipe(1,addresses[1]);
      radio.startListening();
      Serial.println(F("*** CHANGING TO RECEIVE ROLE -- PRESS 'T' TO SWITCH BACK"));
      role = RX;                // Become the primary receiver (pong back)
    }
  }
}



extern "C" int main(void)
{

        setup();


	while (1) {
            loop();

	}



}
