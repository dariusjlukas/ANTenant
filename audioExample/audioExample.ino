/*
 This example generates a square wave based tone at a specified frequency
 and sample rate. Then outputs the data using the I2S interface to a
 MAX08357 I2S Amp Breakout board.

 Circuit:
 * Arduino/Genuino Zero, MKRZero or MKR1000 board
 * MAX08357:
   * GND connected GND
   * VIN connected 5V
   * LRC connected to pin 0 (Zero) or pin 3 (MKR1000, MKRZero)
   * BCLK connected to pin 1 (Zero) or pin 2 (MKR1000, MKRZero)
   * DIN connected to pin 9 (Zero) or pin A6 (MKR1000, MKRZero)

 created 17 November 2016
 by Sandeep Mistry
 */

#include <I2S.h>
#include <Wire.h>

#define SAMPLE_RATE 44

//Clock config bits
#define USBNORMAL 0
#define BOSR 0
#define SR0 1
#define SR1 0
#define SR2 1
#define SR3 1
#define CLKDIV2 0
#define CLKODIV2 0


#define ADCHPD 0
#define ADCS 2
#define HYST 32
#define LINVOL 23
#define RINVOL 23
#define LHPVOL 121
#define RHPVOL 121

// Microphone control
#define MICBOOST 0
#define MUTEMIC 0
#define INSEL 0
#define BYPASS 1
#define DACSEL 1
#define SIDETONE 0
#define SIDEATT 0

const int frequency = 440; // frequency of square wave in Hz
const int amplitude = 500; // amplitude of square wave
const int sampleRate = 8000; // sample rate in Hz

const int halfWavelength = (sampleRate / frequency); // half wavelength of square wave

short sample = amplitude; // current sample value
int count = 0;

void setup() {
  Serial.begin(9600);
  Serial.println("I2S simple tone");

  // start I2S at the sample rate with 16-bits per sample
  if (!I2S.begin(I2S_PHILIPS_MODE, sampleRate, 16)) {
    Serial.println("Failed to initialize I2S!");
    while (1); // do nothing
  }

  // setup i2c pins and configure codec
  // the new Wire library has trouble with 0x00, so (uint8_t) is added
  Wire.begin();
  Wire.beginTransmission(0x1a);
  Wire.write(0x0c); // power reduction register
  Wire.write((uint8_t)0x00); // turn everything on
  Wire.endTransmission();
  
  Wire.beginTransmission(0x1a);
  Wire.write(0x0e); // digital data format
  Wire.write(0x03); // 16-bit DSP Mode B
  Wire.endTransmission();
  
  Wire.beginTransmission(0x1a);
  Wire.write((uint8_t)0x00); // left in setup register
  Wire.write((uint8_t)LINVOL);
  Wire.endTransmission();
  
  Wire.beginTransmission(0x1a);
  Wire.write(0x02); // right in setup register
  Wire.write((uint8_t)RINVOL);
  Wire.endTransmission();
  
  Wire.beginTransmission(0x1a);
  Wire.write(0x04); // left headphone out register
  Wire.write((uint8_t)LHPVOL);
  Wire.endTransmission();
  
  Wire.beginTransmission(0x1a);
  Wire.write(0x06); // right headphone out register
  Wire.write((uint8_t)RHPVOL);
  Wire.endTransmission();

  Wire.beginTransmission(0x1a);
  Wire.write(0x0a); // digital audio path configuration
  Wire.write((uint8_t)ADCHPD);
  Wire.endTransmission();
  
  Wire.beginTransmission(0x1a);
  Wire.write(0x08); // analog audio pathway configuration
  Wire.write((uint8_t)((SIDEATT << 6)|(SIDETONE << 5)|(DACSEL << 4)|(BYPASS << 3)|(INSEL << 2)|(MUTEMIC << 1)|(MICBOOST << 0)));
  Wire.endTransmission();

  Wire.beginTransmission(0x1a);
  Wire.write(0x10); // WM8731 CORE CLOCK config
  Wire.write((uint8_t)((CLKODIV2<<7)|(CLKDIV2<<6)|(SR3<<5)|(SR2<<4)|(SR1<<3)|(SR0<<2)|(BOSR<<1)|(USBNORMAL<<0)));
  Wire.endTransmission();

  Wire.beginTransmission(0x1a);
  Wire.write(0xc); // LININ, CLKOUT power down
  Wire.write(0x41);
  Wire.endTransmission();

  Wire.beginTransmission(0x1a);
  Wire.write(0x12); // codec enable
  Wire.write(0x01);
  Wire.endTransmission();
}

void loop() {
  if (count % halfWavelength == 0) {
    // invert the sample every half wavelength count multiple to generate square wave
    sample = -1 * sample;
  }

  // write the same sample twice, once for left and once for the right channel
  I2S.write(sample);
  I2S.write(sample);

  // increment the counter for the next sample
  count++;
}
