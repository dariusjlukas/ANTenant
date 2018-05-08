/* Teensyduino Core Library
 * http://www.pjrc.com/teensy/
 * Copyright (c) 2017 PJRC.COM, LLC.
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 *
 * 1. The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * 2. If the Software is incorporated into a build system that allows
 * selection among a list of target devices, then similar target
 * devices manufactured by PJRC.COM must be included in the list of
 * target devices and selectable in the same manner.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS
 * BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
 * ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

 //This is the Teensy transmit demonstration code. It is based on
 //several example programs modified for our purposes

#define TRUE 1
#define FALSE 0
#define DEBUG TRUE
#define __MK20DX128__
#include <Arduino.h>




#include <Audio.h>

#include <i2c_t3.h>
#include <SD.h>
#include <SPI.h>

#include <HardwareSerial.h>

#include "play_queue.h"
#include "record_queue.h"

//Radio Includes//
#include <RF24.h>



AudioInputI2S            i2s_mic;

AudioRecordQueue         queue1;         //xy=281,63

AudioPlayQueue queueOut; //xy=352,226

AudioAmplifier amp1;

AudioOutputI2S i2s2;

AudioConnection patchCord_input_0(i2s_mic, 0,queue1 , 0);


AudioConnection patchCord_q_amp(queueOut, amp1);
AudioConnection patchCord_output_0(amp1, 0, i2s2, 0);
AudioConnection patchCord_output_1(amp1, 0, i2s2, 1);




int count = 1;

bool transmit_mode = LOW;

//Radio Setup//
/*************  USER Configuration *****************************/
// Hardware configuration
RF24 radio(7,A6);                     // Set up nRF24L01 radio on SPI bus plus pins 7 & 8

/***************************************************************/

byte addresses[][6] = {"1Node","2Node"};

byte data[32];                           //Data buffer for testing data transfer speeds

//unsigned long counter, rxTimer;          //Counter and timer for keeping track transfer info
unsigned long startTime, stopTime;
bool TX=0,RX=1,role=1;

bool radioNumber = 0;

void setup() {
    AudioMemory(90);



    pinMode(2, INPUT_PULLUP);

    delay(300);
    if(DEBUG == TRUE){
    Serial.begin(115200);
    }
    //

//    while (!Serial.available()) {
//        digitalWrite(13, !digitalRead(13));
//        delay(100);
//    }


    transmit_mode = digitalRead(2);
    if (transmit_mode == LOW) {
        if(DEBUG == TRUE){Serial.println("TX Begin");}
        role = TX;
    } else {
        if(DEBUG == TRUE){Serial.println("RX Begin");}
        role = RX;
    }


    //Serial1.begin(921600);
   // Serial1.begin(230400);


    amp1.gain(5);
    queue1.begin();


    //Radio setup//
    //pinMode(2, INPUT); //Already defined above
    pinMode(A6,OUTPUT);

    SPI.setMOSI(11);
    SPI.setMISO(12);
    SPI.setSCK(A0);

    //Serial.begin(115200); //Already running

      // pinMode(13,OUTPUT);
      // int w = 50;
      // while (!Serial.available() and --w > 0) {
      //     digitalWrite(13, !digitalRead(13));
      //     delay(100);
      // }
      // digitalWrite(13, HIGH);

    radio.begin();                           // Setup and configure rf radio
    radio.setChannel(1);
    radio.setPALevel(RF24_PA_MAX);
    radio.setDataRate(RF24_2MBPS);

    radio.setAutoAck(1);                     // Ensure autoACK is enabled
    //radio.setAutoAck(0);

    //radio.setRetries(2,15);                  // Optionally, increase the delay between retries & # of retries
    radio.setRetries(2,5);                  // Optionally, increase the delay between retries & # of retries

    radio.setCRCLength(RF24_CRC_8);          // Use 8-bit CRC for performance
    // radio.openWritingPipe(addresses[0]);
    // radio.openReadingPipe(1,addresses[1]);

      //role = digitalRead(2);
      // if (role == HIGH) {
      //     Serial.println("TX Begin");
      //
      // } else {
      //     Serial.println("RX Begin");
      //
      // }

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

    // Serial.println(F("\n\rRF24/examples/Transfer/"));
    // Serial.println(F("*** PRESS 'T' to begin transmitting to the other node"));

  //  randomSeed(analogRead(0));              //Seed for random number generation

  //   for(int i=0; i<32; i++){
  //      data[i] = random(255);               //Load the buffer with random data
  //   }
    radio.powerUp();                        //Power up the radio

}


uint32_t last_millis = 0;

#define AUDIO_BYTE_BUFFER  256 //AUDIO_BLOCK_SAMPLES*2  // 16 bit buffer to byte buffer
#define SKIP  2
#define SMALL_BUFFER   AUDIO_BYTE_BUFFER/SKIP

byte small_buffer[SMALL_BUFFER];
byte audio_buffer[AUDIO_BYTE_BUFFER];


uint32_t counter =0;
void loop() {


   if (role == TX) {

        if(queue1.available() >= 1){
            memcpy( audio_buffer,queue1.readBuffer(), AUDIO_BYTE_BUFFER );
            queue1.freeBuffer();





             // write ever other pair of 2 bytes (16 bits) to hardware serial 1
             //We are not using serial
            // if (Serial1.availableForWrite() ) {
              //if(radio.available()){
                 uint8_t b = 0;
                 for( int i = 0 ; i < AUDIO_BYTE_BUFFER ; i=i+(2*SKIP) ){
                     small_buffer[b] = audio_buffer[i];
                     small_buffer[b+1] = audio_buffer[i+1];
                     b = b +2;
                 }
                 //counter += Serial1.write(small_buffer,SMALL_BUFFER);
                //
                for(long bufferIndex = 0; bufferIndex < (SMALL_BUFFER); bufferIndex += 32){
                    if(radio.writeFast(&(small_buffer[bufferIndex]),32)){
                        counter++;
                    }   //Write to the FIFO buffers
                }
              //}

              //for(int i = 0; i < 4; i++){
               //if(radio.writeFast(&data,32)){   //Write to the FIFO buffers
                // counter++;                      //Keep count of failed payloads
              // }
             //}





            // Play audio on local headphones
            if(queueOut.available()){
            int16_t *out_buffer = queueOut.getBuffer();
            memcpy(out_buffer,audio_buffer,AUDIO_BYTE_BUFFER);
            queueOut.playBuffer();

             }

           }
        // }

    } else {
        // This is the reciever code litening on Hardware Serial 1

        //A: This is the transmit code. B: We are not using serial
        int b=0;
      //  if(  (b = Serial1.available())  >= 64   ) {
        int lastTime = millis();
        while(b < SMALL_BUFFER && (millis() - lastTime < 10)){
          if(radio.available()){
            radio.read(&(small_buffer[b]),32);
            b += 32;
            counter ++;
          }
        }

            // for( int i = 0 ; i < b; ++i ){
            //     small_buffer[i]= Serial1.read();
            //     ++counter;
            // }

            byte new_buffer[AUDIO_BYTE_BUFFER];
            for(uint16_t s = 0; s < b; s = s + 2 ){

                for(uint16_t i = 0; i < SKIP; ++i){
                    new_buffer[(s*SKIP)+(i*SKIP)] =  small_buffer[s];
                    new_buffer[(s*SKIP)+(i*SKIP)+1] =  small_buffer[s+1];
                }
            }

            int16_t *out_buffer = queueOut.getBuffer();
            memcpy(out_buffer,new_buffer,b*SKIP);
            queueOut.playBuffer();

      //  }
      // if(millis() - rxTimer > 1000){
      //   rxTimer = millis();
      //   unsigned long numBytes = counter*32;
      //   Serial.print(F("Rate KB/s: "));
      //   //Prevent dividing into 0, which will cause issues over a period of time
      //   Serial.println(numBytes > 0 ? numBytes/1000.0:0);
      //   Serial.print(F("Payload Count: "));
      //   Serial.println(counter);
      //   counter = 0;
      // }

    }




    if (millis() - last_millis > 1000) {
      if(DEBUG == TRUE){
        if (role == TX) Serial.println("TX");
        else Serial.println("RX");
        Serial.print("AudioMemoryUsage: ");
        Serial.println(AudioMemoryUsageMax());
       Serial.print("counter : ");
       Serial.println(counter);
       counter =0;
        last_millis = millis();

       AudioMemoryUsageMaxReset();
     }
     else{
       counter =0;
       last_millis = millis();
       AudioMemoryUsageMaxReset();
     }

    }

    // Open a writing and reading pipe on each radio, with opposite addresses
    bool currentRole = digitalRead(2);
    if(currentRole != role){
      role = currentRole;
      if(role == TX){
        if (DEBUG == TRUE){Serial.println("Opening Pipes TX");}
        radio.openWritingPipe(addresses[1]);
        radio.openReadingPipe(1,addresses[0]);
        if (DEBUG == TRUE){Serial.println("Pipes Open");}
        delay(20);
        radio.stopListening();
      }else{
        if (DEBUG == TRUE){Serial.println("Opening Pipes RX");}
        radio.openWritingPipe(addresses[0]);
        radio.openReadingPipe(1,addresses[1]);
        if (DEBUG == TRUE){Serial.println("Pipes Open");}
        radio.startListening();                 // Start listening
      }
    if (DEBUG == TRUE){Serial.println(role);}
  }


}

extern "C" int main(void) {

    setup();


    while (1) {
        loop();

    }



}
