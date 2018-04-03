/*Mary West - Lazerade
  Ryan Mortenson 
This arduino sketch is for the Audio Reactive Kitty Ears Headband
I made for an instructables tutorial. This code is as-is, no warranty, beerware. 

Hardware: 
MSGEQ7 - 7 Band Audio Equalizer Display Filter
Arduino Pro Mini
SparkFun MEMS microphone breakout board
0.1uF Cap x 3
200K OHM Resistor

My headband hols 29 LEDs, I want the lights to be
symmetrical about the top center which means I only
have 14 LEDs to represent all 7 Frequency Bands
 



*/

#include <Adafruit_NeoPixel.h>
#define PIN 6
#define  NUMPIXELS 14
#define BAND_0_PIXELS 2
#define BAND_1_PIXELS 2
#define BAND_2_PIXELS 2
#define BAND_3_PIXELS 2
#define BAND_4_PIXELS 2
#define BAND_5_PIXELS 2
#define BAND_6_PIXELS 2

int pixelsPerBand[7] = {
  BAND_0_PIXELS,
  BAND_1_PIXELS,
  BAND_2_PIXELS,
  BAND_3_PIXELS,
  BAND_4_PIXELS,
  BAND_5_PIXELS,
  BAND_6_PIXELS,
};

int pixelNumsPerBand[7][6] = {
  {0, 7},
  {1,10},
  {2, 9},
  {3, 8},
  {4,13},
  {5, 12},
  {6, 11}
};

int pixelColorPerBand[7][3] = {
  {255, 0, 0},       // Band 0 Red
  {255, 255, 0},     // Band 1 Yellow 
  {0, 255, 0},       // Band 2 Green
  {0, 255, 255},     // Band 3 Light Blue
  {0, 0, 255},       // Band 4 Blue
  {255, 0, 255},     // Band 5 Magenta
  {255, 255, 255}    // Band 6 White
};

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);


int OUT = A0;

int strobe = 4;

int reset = 3;

unsigned int freqInBand[7]; //Store each of the 7 peaks deatected in each 
//frequency band into an array.



void setup()
{
 pixels.begin(); // This initializes the NeoPixel library.
 pinMode(strobe, OUTPUT);
 pinMode(reset, OUTPUT); 
 digitalWrite(reset,LOW); //Reset Low enables strobe pin 
 digitalWrite(strobe,HIGH); //After the first strobe leading edge, 63 Hz output is on output pin (
}


void loop()
{
 digitalWrite(reset, HIGH);
 delayMicroseconds(1); //Reset Pluse width is 100nS min
 digitalWrite(reset, LOW); //reset low enables strobe Pin
 for (int band = 0; band < 7; band++)
 { 
   digitalWrite(strobe, LOW); //enables output 
   delayMicroseconds(18); //Output settling time is 36uS min. Could get bad data without this delay
   unsigned int calc = analogRead(OUT) - 75;
   freqInBand[band] = (calc > 0  && calc < 1023) ? calc : 5;
   delayMicroseconds(18); //Output settling time is 36uS min. Could get bad data without this delay
   digitalWrite(strobe, HIGH);
   delay(10); //Output settling time is 36uS min. Could get bad data without this delay

   for (int i = 0; i < pixelsPerBand[band]; i++) {
    pixels.setPixelColor(pixelNumsPerBand[band][i], pixels.Color((pixelColorPerBand[band][0] * freqInBand[band]) / 1024, (pixelColorPerBand[band][1] * freqInBand[band]) / 1024, (pixelColorPerBand[band][2] * freqInBand[band]) / 1024));
   }
   pixels.show();
 }
}


