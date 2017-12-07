#include "spaceInvaders.h"

void setup() {
  lcd.begin(16, 2);
  pinMode(3, OUTPUT);
  analogWrite(3, 80);
  randomSeed(analogRead(0));
  pinMode(SW, INPUT);
  digitalWrite(SW, HIGH);
  lc.shutdown(0, false);
  lc.setIntensity(0, 8);
  lc.clearDisplay(0);
  lcd.setCursor(0, 0);
  lcd.print("    Welcome!");
  lcd.setCursor(0, 1);
  lcd.print("Press to start.");
  while (digitalRead(SW) != 0) {
    lc.clearDisplay(0); 
  }
  delay(500);
  lcd.clear();
  lc.setLed(0, 2, 0, true);
  lc.setLed(0, 3, 0, true);
  lc.setLed(0, 4, 0, true);
  lc.setLed(0, 3, 1, true);
}


void loop() {
  
  int X = analogRead(xPin);
  game();

  //repozitionare 
  
  lc.setLed(0, row, 0, false);
  lc.setLed(0, row - 1, 0, false);
  lc.setLed(0, row + 1, 0, false);
  lc.setLed(0, row, 1, false);

  if (X < 400)
    row--;
  if (X > 850)
    row++;
  if (row > 6)
    row = 6;
  if (row < 1)
    row = 1;

  lc.setLed(0, row, 0, true);
  lc.setLed(0, row - 1, 0, true);
  lc.setLed(0, row + 1, 0, true);
  lc.setLed(0, row, 1, true);
  delay(100);

  checkGameLost();

  if (digitalRead(SW) == 0) {
    shoot();
  }
}

