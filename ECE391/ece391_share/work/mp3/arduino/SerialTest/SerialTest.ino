#include <SPI.h>
#include <LCD.h>                  // LCD library
#include <LiquidCrystal_I2C.h>    // another LCD library
#include <Wire.h>  

// random LCD stuff
LiquidCrystal_I2C  lcd(0x3F,2,1,0,4,5,6,7); // 0x27 is the I2C bus address for an unmodified module
// SDA -> A4
// SCL -> A5

char serialIn;   // creating character variable to hold input signals
int i,j;    // iterating variables
int buttonRelease;  // variable to make sure we only send once per button press

void setup() 
{
  //start serial connection
  Serial.begin(9600);

  //configure pin2 as an input and enable the internal pull-up resistor
  pinMode(8, INPUT_PULLUP);
  pinMode(13, OUTPUT);

  i=0;
  j=0;

  LCDstart();
}

void loop() 
{
  //read the pushbutton value into a variable
  int sensorVal = digitalRead(8);
  
  // Keep in mind the pullup means the pushbutton's
  // logic is inverted. It goes HIGH when it's open,
  // and LOW when it's pressed. Turn on pin 13 when the
  // button's pressed, and off when it's not:
  if (sensorVal == HIGH) 
  {
    digitalWrite(13, LOW);
    buttonRelease = 1;
    delay(50);
  }
  else if((sensorVal == LOW)&&(buttonRelease))
  {
    digitalWrite(13, HIGH);
    Serial.print("Husky ");
    Serial.println(i);
    delay(50);
    i++;
    buttonRelease = 0;
  }
  if (Serial.available() > 0) // check if we have serial input ready to come in
  {
    LCDupdate();
  }
}

// LCDstart function
void LCDstart()
{
  lcd.begin(16, 2);     // initialize LCD
  lcd.setBacklightPin(3,POSITIVE);      // initialize backlight
  lcd.backlight(); // turn backlight on
  
  lcd.clear();      // clear
  lcd.setCursor(1,0);       // display title and stuff
  lcd.print("Welcome to the");
  lcd.setCursor(0,1);
  lcd.print("391 Ardu-Tester");
  delay(2000);

  lcd.clear();      // clear and reset for next display
  lcd.home();

  // the first round of updates
  LCDupdate();
  return;
}

void LCDupdate()
{  

  // reset location and clear everything
  lcd.clear(); 
  lcd.home();
  j=0;
  
  while(Serial.available() > 0)   // loop over until no more serial input remains
  {
    if(j>15)    // move to next line if necessary
    {
      j=0;
      lcd.setCursor(0,1);
    }
    
    serialIn = Serial.read();
    lcd.print(serialIn);  // print out whatever char we receive
    j++;
  }
}
