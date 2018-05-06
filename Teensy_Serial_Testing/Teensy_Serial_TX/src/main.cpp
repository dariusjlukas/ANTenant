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

#define __MK20DX128__
#include <Arduino.h>




#include <Audio.h>

#include <i2c_t3.h>
#include <SD.h>
#include <SPI.h>

#include <HardwareSerial.h>

#include "play_queue.h"
#include "record_queue.h"





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

void setup() {
    AudioMemory(90);



    pinMode(2, INPUT);

    delay(300);
    Serial.begin(115200);
    //

//    while (!Serial.available()) {
//        digitalWrite(13, !digitalRead(13));
//        delay(100);
//    }


    transmit_mode = digitalRead(2);
    if (transmit_mode == HIGH) {
        Serial.println("TX Begin");

    } else {
        Serial.println("RX Begin");

    }


    Serial1.begin(921600);
   // Serial1.begin(230400);


    amp1.gain(5);
    queue1.begin();

}


uint32_t last_millis = 0;

#define AUDIO_BYTE_BUFFER  256 //AUDIO_BLOCK_SAMPLES*2  // 16 bit buffer to byte buffer
#define SKIP  4
#define SMALL_BUFFER   AUDIO_BYTE_BUFFER/SKIP

byte small_buffer[SMALL_BUFFER];
byte serial_buffer[AUDIO_BYTE_BUFFER];


uint32_t counter =0;
void loop() {


   if (transmit_mode == HIGH) {

        if(queue1.available() >= 1){
            memcpy( serial_buffer,queue1.readBuffer(), AUDIO_BYTE_BUFFER );
            queue1.freeBuffer();





             // write ever other pair of 2 bytes (16 bits) to hardware serial 1
            if (Serial1.availableForWrite() ) {

                uint8_t b = 0;
                for( int i = 0 ; i < AUDIO_BYTE_BUFFER ; i=i+(2*SKIP) ){
                    small_buffer[b] = serial_buffer[i];
                    small_buffer[b+1] = serial_buffer[i+1];
                    b = b +2;
                }
                counter += Serial1.write(small_buffer,SMALL_BUFFER);



            }

            // Play audio on local headphones
            if(queueOut.available()){
            int16_t *out_buffer = queueOut.getBuffer();
            memcpy(out_buffer,serial_buffer,AUDIO_BYTE_BUFFER);
            queueOut.playBuffer();

          }

       }

    } else {
        // This is the reciever code litening on Hardware Serial 1

        int b=0;
        if(  (b = Serial1.available())  >= 64   ) {
            for( int i = 0 ; i < b; ++i ){
                small_buffer[i]= Serial1.read();
                ++counter;
            }

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

        }

    }




    if (millis() - last_millis > 1000) {

        Serial.print("AudioMemoryUsage: ");
        Serial.println(AudioMemoryUsageMax());
       Serial.print("counter : ");
       Serial.println(counter);
       counter =0;
        last_millis = millis();

       AudioMemoryUsageMaxReset();

    }


}

extern "C" int main(void) {

    setup();


    while (1) {
        loop();

    }



}
