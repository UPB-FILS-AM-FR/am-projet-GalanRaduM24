#include <Wire.h>
#include <LiquidCrystal_I2C.h>


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

LiquidCrystal_I2C lcd(0x27, 16, 2);  // Set the LCD address to 0x27 for a 16 chars and 2 line display


void setup() {
    lcd.init();                      // Initialize the LCD
  lcd.backlight();                 // Turn on the backlight
  image(); 
}

void loop() {
  playImperialMarch();
  delay(3000); // Pause before repeating the song
}

void playImperialMarch() {
  // Notes of the melody and their durations
  int melody[] = {
    NOTE_A4, NOTE_A4, NOTE_F5, NOTE_C5, NOTE_A4, NOTE_F5, NOTE_C5, NOTE_A4,
    NOTE_E5, NOTE_E5, NOTE_E5, NOTE_F5, NOTE_C5S, NOTE_A4S, NOTE_F5, NOTE_C5, NOTE_A4,
    NOTE_A5, NOTE_A4, NOTE_A4, NOTE_A5, NOTE_G5S, NOTE_G5, NOTE_F5S, NOTE_F5, NOTE_F5S, 0,
    NOTE_A4S, NOTE_D5S, NOTE_D5, NOTE_C5S, NOTE_C5, NOTE_B4, NOTE_C5, 0,
    NOTE_F5, NOTE_F5, NOTE_E5, NOTE_F5, NOTE_C5S, NOTE_A4S, NOTE_F5, NOTE_C5, NOTE_A4,
  };
  int noteDurations[] = {
    500, 500, 500, 350, 150, 500, 350, 150,
    500, 500, 500, 350, 150, 500, 350, 150, 500,
    350, 150, 500, 350, 150, 500, 350, 150, 500, 500,
    350, 150, 500, 350, 150, 500, 350, 150,
    500, 350, 150, 500, 350, 150, 500, 350, 150,
  };

  // Play the melody
  for (int thisNote = 0; thisNote < sizeof(melody) / sizeof(int); thisNote++) {
    int noteDuration = noteDurations[thisNote];
    tone(8, melody[thisNote], noteDuration);
    // Wait for the duration of the note plus 30% to distinguish between notes
    int pauseBetweenNotes = noteDuration * 1.30;
    delay(pauseBetweenNotes);
    // Stop the tone
    noTone(8);
  }
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