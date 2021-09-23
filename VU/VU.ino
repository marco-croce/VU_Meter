/*
   Mechanical VU meter
   Author: Marco Croce
   Version: 1.0 - 09/2021
*/

// library used for the LCD display
#include <LiquidCrystal.h>

/*
   Bit / Pixel Mapping to create the letters
   and the symbols to show on the display
*/

byte c0[8] = {
  B11111,
  B00000,
  B00000,
  B00000,
  B00000,
  B00000,
  B00000,
  B11111
};

byte c1[8] = {
  B11111,
  B00000,
  B10000,
  B10000,
  B10000,
  B10000,
  B00000,
  B11111
};

byte c2[8] = {
  B11111,
  B00000,
  B11000,
  B11000,
  B11000,
  B11000,
  B00000,
  B11111
};

byte c3[8] = {
  B11111,
  B00000,
  B11100,
  B11100,
  B11100,
  B11100,
  B00000,
  B11111
};

byte c4[8] = {
  B11111,
  B00000,
  B11110,
  B11110,
  B11110,
  B11110,
  B00000,
  B11111
};

byte c5[8] = {
  B11111,
  B00000,
  B11111,
  B11111,
  B11111,
  B11111,
  B00000,
  B11111
};

byte C[8] = {
  B11100,
  B11110,
  B11111,
  B11111,
  B11111,
  B11111,
  B11110,
  B11100
};

byte R[8] = {
  B00000,
  B11110,
  B10001,
  B10001,
  B11110,
  B10100,
  B10010,
  B10001
};

byte L[8] = {
  B00000,
  B10000,
  B10000,
  B10000,
  B10000,
  B10000,
  B10000,
  B11111
};

// Set the LCD's pin number through the constructor
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);
// Range of dB values
int valori[] = {-75, -50, -40, -30, -20, 0, 5, 10};

/*
   Variables:
   - valore -> The value of volts read using Arduino
   - i -> A simple counter
   - dbVU -> The value of dbVU obtained by valore
   - livello -> A counter for the rectangles of the VU Meter
 */
int valore = 0, i = 0;
double dbVU;
int livello;

void setup()
{
// set up the LCD's number of columns and rows
  lcd.begin(16, 2);
// set up the pin from which to read the value
  pinMode(A1, INPUT);

/*
   Creation of the custom characters to be used
   on the display
*/
  lcd.createChar(1, c1);
  lcd.createChar(2, c2);
  lcd.createChar(3, c3);
  lcd.createChar(4, c4);
  lcd.createChar(5, c5);
  lcd.createChar(6, c0);
  lcd.createChar(7, C);
  lcd.createChar(8, L);
  lcd.createChar(9, R);

// Loading initial screen
  lcd.setCursor(0, 0);
  lcd.print("    VU METER    ");
  delay(500);

// Loading line
  for ( livello = 0; livello < 16; livello++ )
  {
    lcd.setCursor(livello, 1);
    lcd.write(5);
    delay(100);
  }

  delay(500);

  lcd.clear();
  
  delay(500);

}

void loop()
{
// Letter L - LEFT
  lcd.setCursor(0,0);
  lcd.write(8);
// Letter R - RIGHT
  lcd.setCursor(0,1);
  lcd.write(9);

// Delimitation symbol C
  lcd.setCursor(15,0);
  lcd.write(7);
  lcd.setCursor(15,1);
  lcd.write(7);

  delay(100);

// Voltage reading from the pin A1
  do
  {
    valore = analogRead(A1);
  } while ( valore <= 5 );

/*
   1) valore*0.0049 -> Arduino reads 4.9 mV to unit, so this is 
   the conversion to volt
   More info at: https://www.arduino.cc/reference/it/language/functions/analog-io/analogread/
   2) valore*0.0049/1.2 -> 1.2 represents the value of SOL ( Standard Operative Level ), used
   to indicate the maximum level to avoid the saturation, for VU Meter is 1.2 Volt
   3) 20*log( valore*0.0049/1.2 ) -> This is the formula to converts values from volt to 
   db for VU Meter, dbVU
 */
  dbVU = (log10((double)valore*0.0049/1.2))*20;

  // Setting the new rectangles
  for (int i = 0, livello = 1; i < 7; i++) {
    if (valori[i] < dbVU) {
      lcd.setCursor(livello, 0);
      lcd.write(5);
      lcd.setCursor(livello++, 1);
      lcd.write(5);
      delay(50);
      lcd.setCursor(livello, 0);
      lcd.write(5);
      lcd.setCursor(livello++, 1);
      lcd.write(5);
    }
  }

  // Removing the old rectangles
  for (int i = 7, livello = 14; i > 0; i--) {
    if (valori[i] > dbVU) {
      lcd.setCursor(livello, 0);
      lcd.write(6);
      lcd.setCursor(livello--, 1);
      lcd.write(6);
      delay(50);
      lcd.setCursor(livello, 0);
      lcd.write(6);
      lcd.setCursor(livello--, 1);
      lcd.write(6);
    }
  }

  delay(100);

}
