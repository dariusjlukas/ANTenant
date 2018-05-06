// // /*
// //  * Blink
// //  * Turns on an LED on for one second,
// //  * then off for one second, repeatedly.
// //  */
// //
// // #include <Arduino.h>
// //
// // void setup()
// // {
// //   // initialize LED digital pin as an output.
// //   pinMode(LED_BUILTIN, OUTPUT);
// // }
// //
// // void loop()
// // {
// //   // turn the LED on (HIGH is the voltage level)
// //   digitalWrite(LED_BUILTIN, HIGH);
// //   // wait for a second
// //   delay(100);
// //   // turn the LED off by making the voltage LOW
// //   digitalWrite(LED_BUILTIN, LOW);
// //    // wait for a second
// //   delay(1000);
// // }
//
// /* Teensyduino Core Library
//  * http://www.pjrc.com/teensy/
//  * Copyright (c) 2017 PJRC.COM, LLC.
//  *
//  * Permission is hereby granted, free of charge, to any person obtaining
//  * a copy of this software and associated documentation files (the
//  * "Software"), to deal in the Software without restriction, including
//  * without limitation the rights to use, copy, modify, merge, publish,
//  * distribute, sublicense, and/or sell copies of the Software, and to
//  * permit persons to whom the Software is furnished to do so, subject to
//  * the following conditions:
//  *
//  * 1. The above copyright notice and this permission notice shall be
//  * included in all copies or substantial portions of the Software.
//  *
//  * 2. If the Software is incorporated into a build system that allows
//  * selection among a list of target devices, then similar target
//  * devices manufactured by PJRC.COM must be included in the list of
//  * target devices and selectable in the same manner.
//  *
//  * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
//  * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
//  * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
//  * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS
//  * BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
//  * ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
//  * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
//  * SOFTWARE.
//  */
//
// #define __MK20DX128__
// #include <Arduino.h>
//
//
//
// // Advanced Microcontroller-based Audio Workshop
// //
// // http://www.pjrc.com/store/audio_tutorial_kit.html
// // https://hackaday.io/project/8292-microcontroller-audio-workshop-had-supercon-2015
// //
// // Part 1-2: Test Hardware
// //
// // Simple beeping is pre-loaded on the Teensy, so
// // it will create sound and print info to the serial
// // monitor when plugged into a PC.
// //
// // This program is supposed to be pre-loaded before
// // the workshop, so Teensy+Audio will beep when
// // plugged in.
//
// #include <Audio.h>
// //#include <Wire.h>
// #include <i2c_t3.h>
// #include <SD.h>
// #include <SPI.h>
// #include <SerialFlash.h>
// //#include <Bounce.h>
//
// //#include "AudioSampleGong.h"
// #include "play_queue.h"
// #include "mixer.h"
// #include "effect_delay.h"
//
//
// // GUItool: begin automatically generated code
//  AudioInputI2S            i2s_mic;
//
// //AudioPlayMemory          sound0;
//
// AudioRecordQueue         queue1;         //xy=281,63
//
//
//
// AudioPlayQueue           queueOut;         //xy=352,226
//
// AudioAmplifier           amp1;
// AudioEffectDelay         delayOut;
// AudioOutputI2S           i2s2;
//
// AudioConnection          patchCord1(i2s_mic, 0, queue1, 0);
// //AudioConnection          patchCord2(i2s_mic, 1, queue1, 1);
//
//
// AudioConnection          patchCord2(queueOut,  amp1);
//
//
// //AudioConnection          patchCord_mix_2(queueOut, 0, mix1, 1);
//
// //AudioConnection          patchCord_output_0(amp1,0, i2s2,0);
// //AudioConnection          patchCord_amp_1(amp1,0,  delayOut,0);
// //AudioConnection          patchCord_output_0(amp1,0, i2s2,0);
// AudioConnection          patchCord_output_1(amp1, 0, i2s2, 1);
//
//
// //AudioConnection c1(sound0, 0, mix1, 0);
// //AudioConnection c2(sound0, 1, mix1, 1);
//
// //AudioConnection          patchCord3(sound0,0, i2s2, 0);
// //AudioConnection          patchCord4(sound0,0, i2s2, 1);
//
// // GUItool: end automatically generated code
//
//
//
// // #define RADIO_MOSI_PIN  7
// // #define RADIO_SCK_PIN   14
//
//
//
// //AudioSynthWaveform    waveform1;
// //AudioOutputI2S        i2s1;
// //AudioConnection       patchCord1(waveform1, 0, i2s1, 0);
// //AudioConnection       patchCord2(waveform1, 0, i2s1, 1);
//
// //AudioControlSGTL5000  sgtl5000_1;
//
// //Bounce button0 = Bounce(0, 15);
// //Bounce button1 = Bounce(1, 15);
// //Bounce button2 = Bounce(2, 15);
//
// int count=1;
//
//
// void setup() {
//   AudioMemory(90);
//
//   Serial.begin(115200);
//
//  // while(!Serial.available()){}
//   Serial.println("Begin");
//
//   //SPI.setMOSI(RADIO_MOSI_PIN);
//   //SPI.setSCK(RADIO_SCK_PIN);
//
//
//    // pinMode(13, INPUT);
//    // digitalWrite(13,HIGH);
//
// //    mix1.gain(0, 1);
// //    mix1.gain(1, 1);
//
//   //  delayOut.delay(0,80);
//     amp1.gain(12);
//     queue1.begin();
//
// }
//
//
// uint32_t last_millis = 0;
//
//
// void loop() {
//
// //    byte buffer[128];
// //    uint8_t bcount= 0;
// //    while(Serial.available() and bcount < 128){
// //
// //      buffer[bcount++] = Serial.read();
// //
// //      int16_t *p = queueOut.getBuffer();
// //      memcpy(p, buffer, bcount);
// //      queueOut.playBuffer();
// //    }
//
//     if (queue1.available() >= 1){
//
// //         byte buffer[128];
// //
// //         memcpy(buffer, queue1.readBuffer(), 128);
// //         queue1.freeBuffer();
// //
// // //        for( int i = 0; i < 128; ++i){
// // //             Serial.write(buffer[i]);
// // //        }
// //
// //         //Serial.write(buffer,128);
// //
// //         int16_t *p = queueOut.getBuffer();
// //         memcpy(p, buffer, 128);
// //         queueOut.playBuffer();
// //
// //     }else {
// //
// //     }
//
//
//     if(millis() - last_millis > 3000 ){
//
//        Serial.print("AudioMemoryUsage: ");
//        Serial.println(AudioMemoryUsageMax());
//        last_millis = millis();
//
//        AudioMemoryUsageMaxReset();
//      //  sound0.play(AudioSampleGong);
//
//     }
//
//
// }
// }
//
//
//
// // extern "C" int main(void)
// // {
// //
// //         setup();
// //
// //
// // 	while (1) {
// //             loop();
// //
// // 	}
// //
// //
// //
// // }
