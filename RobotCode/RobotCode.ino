#include <MeMCore.h>
#include <Arduino.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include "Adafruit_TCS34725.h"
#include <TimerFreeTone.h>


#define MOTOR_IN1 A0 // Connect to DRV8871 IN1
#define MOTOR_IN2 A1 // Connect to DRV8871 IN2

Adafruit_TCS34725 tcs = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_50MS, TCS34725_GAIN_4X);

MeIR ir;
MeDCMotor motor_9(9);
MeDCMotor motor_10(10);
MeUltrasonicSensor ultraSensor(3);
MeHumiture humiture(1);
MeRGBLed led(7, 2);

#define SPEAKER_PIN 8

// B - 70
// C - 71
// SET - 21
// 4 - 8
// 5 - 28
// 6 - 90
// 7 - 66
// 8 - 82
// 9 - 74 

// Key definitions
#define FORWARD_KEY 64 //up
#define BACKWARDS_KEY 25 //down
#define LEFT_KEY 7 //left
#define RIGHT_KEY 9 //right
#define GRAB_KEY 68 //D
#define RELEASE_KEY 67 //E
#define Ultrasonic_KEY 13 //F
#define Temperature_KEY 94 //3
#define Humidity_KEY 24 //2
#define Color_KEY 22 //0
#define DarthVader_KEY 12 //1
#define Lock_KEY 69 //A

// Notes frequencies
#define NOTE_A4  440
#define NOTE_A4S 466
#define NOTE_B4  494
#define NOTE_C5  523
#define NOTE_C5S 554
#define NOTE_D5  587
#define NOTE_D5S 622
#define NOTE_E5  659
#define NOTE_F5  698
#define NOTE_F5S 740
#define NOTE_G5  784
#define NOTE_G5S 831
#define NOTE_A5  880

// Set the LCD address to 0x27 for a 16 chars and 2 line display
LiquidCrystal_I2C lcd(0x27, 16, 2);

int displayFunction = 0;
bool displayLocked = false;


void setup() {
  Serial.begin(9600);
  Wire.begin();

  // Initialize the TCS34725
  if (tcs.begin()) {
    Serial.println("Found sensor");
  } else {
    Serial.println("No TCS34725 found ... check your connections");
    while (1); // Halt the program
  }
  
  // Initialize the LCD
  lcd.init();
  // Turn on the backlight
  lcd.backlight();

  pinMode(MOTOR_IN1, OUTPUT);
  pinMode(MOTOR_IN2, OUTPUT);
  
  pinMode(SPEAKER_PIN, OUTPUT);

  ir.begin();
}

void loop() {
  if (ir.keyPressed(FORWARD_KEY)) {
    if (!displayLocked){
      if (displayFunction != 1) {
        clearFirstRow();
        displayFunction = 1;
        setDisplay(displayFunction);
      }
      clearSecondRow();
      lcd.setCursor(0, 1);
      lcd.print("Forward");
    }
    Serial.println("Forward");
    move(2, 100);
  } else if (ir.keyPressed(BACKWARDS_KEY)) {
    if (!displayLocked){
      if (displayFunction != 1) {
        lcd.clear();
        displayFunction = 1;
        setDisplay(displayFunction);
      }
      clearSecondRow();
      lcd.setCursor(0, 1);
      lcd.print("Backwards");
    }
    Serial.println("Backwards");
    move(1, 100);
  } else if (ir.keyPressed(LEFT_KEY)) {
    if (!displayLocked){
      if (displayFunction != 1) {
        lcd.clear();
        displayFunction = 1;
        setDisplay(displayFunction);
      }
      clearSecondRow();
      lcd.setCursor(0, 1);
      lcd.print("Left");
    }
    Serial.println("Left");
    move(3, 100);
  } else if (ir.keyPressed(RIGHT_KEY)) {
    if (!displayLocked){
      if (displayFunction != 1) {
        lcd.clear();
        displayFunction = 1;
        setDisplay(displayFunction);
      }
      clearSecondRow();
      lcd.setCursor(0, 1);
      lcd.print("Right");
    }
    Serial.println("Right");
    move(4, 100);
  } else {
    move(0, 0); // Stop if no key is pressed
  }

  if (ir.keyPressed(GRAB_KEY)) {
    analogWrite(MOTOR_IN1, 0);
    analogWrite(MOTOR_IN2, 255);
    Serial.println("Grab");
    analogWrite(MOTOR_IN2, 0); // Stop motor after action
  }

  if (ir.keyPressed(RELEASE_KEY)) {
    analogWrite(MOTOR_IN2, 0);
    analogWrite(MOTOR_IN1, 255);
    Serial.println("Release");
    analogWrite(MOTOR_IN1, 0); // Stop motor after action
  }

  if (ir.keyPressed(Ultrasonic_KEY)) {
    if (!displayLocked){
      if (displayFunction != 2) {
        clearFirstRow();
        displayFunction = 2;
        setDisplay(displayFunction);
      }
      ultrasonic();
    }
  }

  if (ir.keyPressed(Temperature_KEY)) {
    if (!displayLocked){
      if (displayFunction != 3) {
        clearFirstRow();
        displayFunction = 3;
        setDisplay(displayFunction);
      }    
      temperature();
    }

  }

  if (ir.keyPressed(Humidity_KEY)) {
    if (!displayLocked){
      if (displayFunction != 4) {
        clearFirstRow();
        displayFunction = 4;
        setDisplay(displayFunction);
      }
      humidity();
    }
  }

  if (ir.keyPressed(Color_KEY)) {
    if (!displayLocked){
      if (displayFunction != 5) {
        clearFirstRow();
        displayFunction = 5;
        setDisplay(displayFunction);
      }
      colorSensor();
    }
  }

  if (ir.keyPressed(DarthVader_KEY)) {
    if (!displayLocked){
      if (displayFunction != 6) {
        clearFirstRow();
        displayFunction = 6;
        setDisplay(displayFunction);
      }
    playImperialMarch();
    }
  }

    if (ir.keyPressed(Ultrasonic_KEY) || ir.keyPressed(Temperature_KEY) || ir.keyPressed(Humidity_KEY) || ir.keyPressed(Color_KEY)) {
    displayFunction = 0; 
    if (ir.keyPressed(Ultrasonic_KEY)) {
      displayFunction = 2;
    } else if (ir.keyPressed(Temperature_KEY)) {
      displayFunction = 3;
    } else if (ir.keyPressed(Humidity_KEY)) {
      displayFunction = 4;
    } else if (ir.keyPressed(Color_KEY)) {
      displayFunction = 5;
    }
    setDisplay(displayFunction);
  }

  if (ir.keyPressed(Lock_KEY)){
    displayLocked =! displayLocked;
    Serial.println(displayLocked);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Status:");
    lcd.setCursor(0, 1);
    lcd.print(displayLocked);
    delay(1000);
  }

  if (displayLocked) {
    // Update the display based on the current function
    switch (displayFunction) {
      case 2:
        clearFirstRow();
        setDisplay(displayFunction);
        ultrasonic();
        break;
      case 3:
        clearFirstRow();
        setDisplay(displayFunction);
        temperature();
        break;
      case 4:
        clearFirstRow();
        setDisplay(displayFunction);
        humidity();
        break;
      case 5:
        clearFirstRow();
        setDisplay(displayFunction);
        colorSensor();
        break;
      case 6:
        clearFirstRow();
        setDisplay(displayFunction);
        image();
        playImperialMarch();
        break;
    }
  }

}

void clearSecondRow() {
  lcd.setCursor(0, 1);
  lcd.print("                ");
}

void clearFirstRow() {
  lcd.setCursor(0, 0);
  lcd.print("                ");
}

void setDisplay(int function) {
  switch (function) {
    case 1:
      lcd.setCursor(0, 0);
      lcd.print("Direction:");
      break;
    case 2:
      lcd.setCursor(0, 0);
      lcd.print("Distance in cm:");
      clearSecondRow();
      break;
    case 3:
      lcd.setCursor(0, 0);
      lcd.print("Temperature (oC) =");
      break;
    case 4:
      lcd.setCursor(0, 0);
      lcd.print("Humidity (%) =");
      break;
    case 5:
      clearFirstRow();
      clearSecondRow();
      break;
    case 6:
      clearFirstRow();
      clearSecondRow();
      image();
      break;
  }
}

void move(int direction, int speed) {
  int leftSpeed = 0;
  int rightSpeed = 0;

  if (direction == 1) {
    leftSpeed = speed;
    rightSpeed = speed;
  } else if (direction == 2) {
    leftSpeed = -speed;
    rightSpeed = -speed;
  } else if (direction == 3) {
    leftSpeed = -speed;
    rightSpeed = speed;
  } else if (direction == 4) {
    leftSpeed = speed;
    rightSpeed = -speed;
  }
  motor_9.run((9) == M1 ? -(leftSpeed) : (leftSpeed));
  motor_10.run((10) == M1 ? -(rightSpeed) : (rightSpeed));
}

void ultrasonic() {
  Serial.print("Distance : ");
  Serial.print(ultraSensor.distanceCm());
  Serial.println(" cm");
  clearSecondRow();
  lcd.setCursor(0, 1);
  lcd.print(ultraSensor.distanceCm());
  delay(100);

}

void temperature() {
  Serial.print(", Temperature (oC) = ");
  Serial.println(humiture.getTemperature());
  Serial.println("###########################");
  clearSecondRow();
  lcd.setCursor(0, 1);
  lcd.print(humiture.getTemperature());
  delay(100);
  humiture.update();
}

void humidity() {
  Serial.print("Humidity (%) = ");
  Serial.println(humiture.getHumidity());
  Serial.println("###########################");
  clearSecondRow();
  lcd.setCursor(0, 1);
  lcd.print(humiture.getHumidity());
  delay(100);
  humiture.update();
}

void colorSensor() {
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
  lcd.setCursor(0, 0);
  lcd.print("R: "); lcd.print(red);
  lcd.setCursor(8, 0);
  lcd.print("G: "); lcd.print(green);
  lcd.setCursor(0, 1);
  lcd.print("B: "); lcd.print(blue);
  lcd.setCursor(8, 1);

  // Delay to allow the sensor to capture the next reading
  delay(100);
}

void image() {
  // Define custom characters
  uint8_t image24[8] = {B11110, B00001, B10101, B01101, B10101, B00101, B11000, B00111};  
  uint8_t image25[8] = {B01111, B10000, B10101, B10110, B10101, B10100, B00011, B11100};  
  uint8_t image23[8] = {B00010, B00011, B00010, B00010, B00101, B00101, B00111, B00000};  
  uint8_t image26[8] = {B01000, B11000, B01000, B01000, B10100, B10100, B11100, B00000};  
  uint8_t image10[8] = {B00000, B00000, B00000, B00000, B10000, B10000, B10000, B01000};  
  uint8_t image07[8] = {B00000, B00000, B00000, B00000, B00001, B00001, B00001, B00010};  
  uint8_t image08[8] = {B00111, B01101, B11101, B11101, B11101, B00001, B00110, B11110};  
  uint8_t image09[8] = {B11110, B10110, B10111, B10111, B10111, B10001, B01100, B01111};  

  // Create custom characters
  lcd.createChar(0, image24);
  lcd.createChar(1, image25);
  lcd.createChar(2, image23);
  lcd.createChar(3, image26);
  lcd.createChar(4, image10);
  lcd.createChar(5, image07);
  lcd.createChar(6, image08);
  lcd.createChar(7, image09);

  // Print the custom characters to specific positions on the LCD
  lcd.setCursor(7, 1);
  lcd.write(byte(0));
  lcd.setCursor(8, 1);
  lcd.write(byte(1));
  lcd.setCursor(6, 1);
  lcd.write(byte(2));
  lcd.setCursor(9, 1);
  lcd.write(byte(3));
  lcd.setCursor(9, 0);
  lcd.write(byte(4));
  lcd.setCursor(6, 0);
  lcd.write(byte(5));
  lcd.setCursor(7, 0);
  lcd.write(byte(6));
  lcd.setCursor(8, 0);
  lcd.write(byte(7));
}

void playImperialMarch() {
  delay(100);
  // Frequencies of the melody notes in Hertz and their durations in milliseconds
  int melody[][2] = {
    {440, 500}, {440, 500}, {698, 500}, {523, 350}, {440, 150}, {698, 500}, {523, 350}, {440, 150},
    {659, 500}, {659, 500}, {659, 500}, {698, 350}, {554, 150}, {466, 500}, {698, 350}, {523, 150}, {440, 500},
    {880, 350}, {440, 150}, {440, 500}, {880, 350}, {830, 150}, {784, 500}, {740, 350}, {698, 150}, {740, 500}, {0, 500},
    {466, 350}, {622, 150}, {587, 500}, {554, 350}, {523, 150}, {494, 500}, {523, 350}, {0, 150},
    {698, 500}, {698, 350}, {659, 150}, {698, 500}, {554, 350}, {466, 150}, {698, 500}, {523, 350}, {440, 150}
  };

  // Play the melody
  for (int i = 0; i < sizeof(melody) / sizeof(melody[0]); i++) {
    int noteFrequency = melody[i][0];
    int noteDuration = melody[i][1];
    
    if (noteFrequency == 0) {
      delay(noteDuration); // Rest note
    } else {
      TimerFreeTone(SPEAKER_PIN, noteFrequency, noteDuration);
      delay(noteDuration * 1.10); // Shorter pause between notes for smoother melody
    }
  }
}


