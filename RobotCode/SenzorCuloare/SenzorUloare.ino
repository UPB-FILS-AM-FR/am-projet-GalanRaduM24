#include <Wire.h>
#include "Adafruit_TCS34725.h"
#include <LiquidCrystal_I2C.h>
#include "MeOrion.h"

// Initialize the RGB sensor
Adafruit_TCS34725 tcs = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_50MS, TCS34725_GAIN_4X);

// Initialize the RGB LED strip on PORT_3 with 15 LEDs
MeRGBLed led(PORT_3, 2);

// Initialize the LCD display with I2C address 0x27 and 16x2 size
LiquidCrystal_I2C lcd(0x27, 16, 2);

void setup() {
  // Start serial communication
  Serial.begin(9600);

  // Initialize the TCS34725
  if (tcs.begin()) {
    Serial.println("Found sensor");
  } else {
    Serial.println("No TCS34725 found ... check your connections");
    while (1); // Halt the program
  }

  // Initialize the LCD
  lcd.init();
  lcd.backlight();
}

void loop() {
  uint16_t r, g, b, c;
  
  // Get raw color data from the sensor
  tcs.getRawData(&r, &g, &b, &c);
  
  // Print out the values for debugging
  Serial.print("R: "); Serial.print(r); Serial.print(" ");
  Serial.print("G: "); Serial.print(g); Serial.print(" ");
  Serial.print("B: "); Serial.print(b); Serial.print(" ");
  Serial.print("C: "); Serial.print(c); Serial.print(" ");
  Serial.println();

  // Normalize the RGB values
  uint8_t red = map(r, 0, 65535, 0, 255);
  uint8_t green = map(g, 0, 65535, 0, 255);
  uint8_t blue = map(b, 0, 65535, 0, 255);

  // Set the color of all LEDs to the detected color
  for (uint8_t i = 0; i < led.getNumber(); i++) {
    led.setColorAt(i, red, green, blue);
  }
  led.show();


  // Display the main color and name on the LCD
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("R: "); lcd.print(red);
  lcd.setCursor(8, 0);
  lcd.print("G: "); lcd.print(green);
  lcd.setCursor(0, 1);
  lcd.print("B: "); lcd.print(blue);
  lcd.setCursor(8, 1);

  // Delay to allow the sensor to capture the next reading
  delay(500);
}


