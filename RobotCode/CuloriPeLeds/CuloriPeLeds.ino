#include <Wire.h>
#include "Adafruit_TCS34725.h"
#include "MeOrion.h"

// Create an instance of the TCS34725
Adafruit_TCS34725 tcs = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_614MS, TCS34725_GAIN_1X);

// Create an instance of the RGB LED
MeRGBLed led(PORT_3, 15);  // Adjust the number of LEDs accordingly

void setup() {
  Serial.begin(9600);
  if (tcs.begin()) {
    Serial.println("Found sensor");
  } else {
    Serial.println("No TCS34725 found ... check your connections");
    while (1); // halt!
  }
}

void loop() {
  uint16_t r, g, b, c;
  float red, green, blue;

  // Read the RGB color from the sensor
  tcs.getRawData(&r, &g, &b, &c);
  
  // Normalize the RGB values
  red = (float)r / 65536.0 * 255.0;
  green = (float)g / 65536.0 * 255.0;
  blue = (float)b / 65536.0 * 255.0;
  
  // Print the values to the Serial Monitor
  Serial.print("R: "); Serial.print(red); 
  Serial.print(" G: "); Serial.print(green); 
  Serial.print(" B: "); Serial.println(blue);

  // Set the color of the LEDs
  setAllLEDs(red, green, blue);
  
  // Small delay before the next reading
  delay(500);
}

void setAllLEDs(uint8_t red, uint8_t green, uint8_t blue) {
  for (int i = 0; i < led.getNumber(); i++) {
    led.setColorAt(i, red, green, blue);
  }
  led.show();
}
