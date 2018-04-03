/*Mary West
 * Desert Bloom
 * This arduino sketch is for the Audio Reactive Kitty Ears Headband
 * I made for an instructables tutorial that I never finished. 
 * I found an instructable written by Anouk Wipprecht:
 * http://www.instructables.com/id/ElectronicKittyEars-Headset-DIY/
 * Which inspired me to take my headband in a new direction. 
 * This code is as-is, no warranty, beerware. 
 * Hardware: 
 * MSGEQ7 - 7 Band Audio Equalizer Display Filter
 * Arduino Pro Mini
 * SparkFun MEMS microphone breakout board
 * 14 WS2812 LEDs
 * Supporting passives
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
  {1, 10},
  {2, 9},
  {3, 8},
  {4, 13},
  {5, 12},
  {6, 11}
};

int pixelColorPerBand[7][3] = {
  {255, 0, 0},       // Band 0 Red
  {0, 255, 0},     // Band 1 Yellow 
  {0, 0, 255},       // Band 2 Green
  {255, 255, 255},     // Band 3 Light Blue
  {255, 0, 0},       // Band 4 Blue
  {0, 255, 0},     // Band 5 Magenta
  {0, 0, 255}    // Band 6 White
};

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);


int OUT = A0;//Connect pin 3 of the MSGEQ7, OUT, to any analog pin 


int strobe = 4;//Connect pin 4 of the MSGQE7, Strobe, to any digital pin on the Arduino
//In this sketch, it is pin 6

int reset = 3;//Connect pin 7 of the MSGEQ7, Reset, to any digital pin on the Arduino
//In this sketch, it is pin 5.

unsigned int freqInBand[7]; //Store each of the 7 peaks deatected in each 
//frequency band into an array.



void setup()
{
 pixels.begin(); // This initializes the NeoPixel library.
 //Serial.begin(115200);
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
 //Serial.println("Starting");
 for (int band = 0; band < 7; band++)
 { 
   digitalWrite(strobe, LOW); //enables output 
   delayMicroseconds(18); //Output settling time is 36uS min. Could get bad data without this delay
   unsigned int calc = analogRead(OUT)-125 ;
   freqInBand[band] = (calc > 0  && calc < 1023) ? calc : 5;
   //Serial.println(freqInBand[band]);
   delayMicroseconds(18); //Output settling time is 36uS min. Could get bad data without this delay
   digitalWrite(strobe, HIGH);
   delay(10); //Output settling time is 36uS min. Could get bad data without this delay

   for (int i = 0; i < pixelsPerBand[band]; i++) {
    pixels.setPixelColor(pixelNumsPerBand[band][i], pixels.Color((pixelColorPerBand[band][0] * freqInBand[band]) / 1024, (pixelColorPerBand[band][1] * freqInBand[band]) / 1024, (pixelColorPerBand[band][2] * freqInBand[band]) / 1024));
   }
   pixels.show();
 }
}


