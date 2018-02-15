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
#define PIN 9
#define  NUMPIXELS 29
#define BAND_0_PIXELS 3
#define BAND_1_PIXELS 4
#define BAND_2_PIXELS 4
#define BAND_3_PIXELS 4
#define BAND_4_PIXELS 4
#define BAND_5_PIXELS 6
#define BAND_6_PIXELS 4

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
  {13, 14, 15},
  {11, 12, 16, 17},
  {9, 10, 18, 19},
  {7, 8, 20, 21},
  {5, 6, 22, 23},
  {2, 3, 4, 24, 25, 26},
  {0, 1, 27, 28}
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


int OUT = A0;//Connect pin 3 of the MSGEQ7, OUT, to any analog pin 


int strobe = 3;//Connect pin 4 of the MSGQE7, Strobe, to any digital pin on the Arduino
//In this sketch, it is pin 6

int reset = 5;//Connect pin 7 of the MSGEQ7, Reset, to any digital pin on the Arduino
//In this sketch, it is pin 5.

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


