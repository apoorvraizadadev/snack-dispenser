#include <LiquidCrystal.h>
#include <Servo.h>

/* pins:

2: rotary encoder button
3: rotary encoder clk pin
4: rotary encoder dt pin
5: none
6: servo
7: led - red
8: led - green
9: RFID pin 7
10: RFID pin 1
11: RFID pin 3
12: RFID pin 4
13: RFID pin 2

*/

int sw = 2;
int clk = 3;
int dt = 4;
int optionSelected = 0;
int lastClk;

int buttonState;
int lastButtonState = LOW;

unsigned long lastDebounceTime = -51;
unsigned long debounceDelay = 50;

Servo servo1;
Servo servo2;

const int rs = 12, en = 11, d4 = 8, d5 = 7, d6 = 6, d7 = 5;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

// unsigned long lastCheckedCard = -500;

void setup() 
{
  // put your setup code here, to run once:
  Serial.begin(9600);
  
  // SPI.begin();
  // myRFID.PCD_Init();

  pinMode(sw, INPUT_PULLUP);
  pinMode(clk, INPUT);
  pinMode(dt, INPUT);
  lastClk = digitalRead(clk);

  servo1.attach(9);
  servo1.write(0);
  servo2.attach(10);
  servo2.write(0);

  lcd.begin(16, 2);
  displayOption();
}

void loop() 
{
  // put your main code here, to run repeatedly:
  checkRotaryEncoder();

  if (getEncoderButton())
  {
    if (optionSelected == 0)
    {
      turnServo(servo1);
    }

    else
    {
      turnServo(servo2);
    }
  }
}

void checkRotaryEncoder()
{
  int clkVal = digitalRead(clk);

  // clkVal == 1 prevents double counting a turn
  if (clkVal != lastClk && clkVal == 1)
  {
    // increment by 1 if rotated clockwise, otherwise decrease by 1
    optionSelected += digitalRead(dt) == clkVal ? 1 : -1;
    optionSelected = (optionSelected < 0) ? 1 : (optionSelected % 2);
    
    Serial.print("Encoder Count: ");
    Serial.println(optionSelected);

    displayOption();
  } 
    
  lastClk = clkVal;
}

bool getEncoderButton() 
{
  int reading = digitalRead(sw);
  bool result = false;

  if (reading != lastButtonState) 
  {
    lastDebounceTime = millis();
  }

  if ((millis() - lastDebounceTime) > debounceDelay) 
  {
    if (reading != buttonState)
    {
      buttonState = reading;

      if (buttonState == LOW) 
      {
        result = true;
      }
    }
  }
  
  lastButtonState = reading;
  return result;
}

void turnServo(Servo servo)
{
  servo.write(0);
  delay(100);
  servo.write(90);
  delay(150);
  servo.write(0);
}

void displayLine(String s)
{
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(s);
}

void displayOption()
{
  if (optionSelected == 0)
  {
    displayLine("Option 1");
  }

  else
  {
    displayLine("Option 2");
  }
}