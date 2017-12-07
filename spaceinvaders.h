#ifndef SPACEINVADERS_H
#define SPACEINVADERS_H

#include <LiquidCrystal.h>
#include "LedControl.h" 

LedControl lc = LedControl(12, 11, 10, 2);
#define xPin A0
#define yPin A1
#define SW 9
#define RS 1
#define ENABLE 2
#define D4 4
#define D5 5
#define D6 6
#define D7 7
LiquidCrystal lcd(RS, ENABLE, D4, D5, D6, D7);

int row = 3, lives = 3;
int count = 90, shots = 0; //count numara inamicii distrusi pt avansarea in nivel; shots e pt numararea loviturilor in BigBoss
int ok = 0;  //se face 1 dupa aparitia lui Big Boss

       //variabilele folosite pentru generarea, respectiv caderea blocurilor la un interval de timp
unsigned long prevTimeGenerate = 0;
unsigned long prevTimeFall = 0;
unsigned long prevBigBossTime = 0;

//intervalele de generare, respectiv cadere a blocurilor pentru fiecare nivel
const long intervalGenerate1 = 2500;
const long intervalFall1 = 1000;

const long intervalGenerate2 = 1500;
const long intervalFall2 = 800;

const long intervalGenerate3 = 2000;
const long intervalFall3 = 1000;

const long intervalGenerate4 = 3000;
const long intervalFall4 = 1500;

const long intervalShootBigBoss = 3000;
const long intervalFallBigBoss = 1000;

int lvl = 1;
byte m[8][8];

//doua imagini create cu un generator de pe internet
const uint64_t bigBossComes[] = {   //imaginea ce apare la inceputul nivelului 5, intrarea lui BigBoss
  0x0000000000000000,
  0x0000000080000000,
  0x00000080c0800000,
  0x000080c0e0c08000
};
const int bigBossComes_LEN = sizeof(bigBossComes) / 8;

const uint64_t gameWon[] = {    //imaginea ce apare dupa ce BigBoss este distrus    
  0x000080c0e0c08000,
  0x000080c0e0c08000,
  0x802080d080208000,
  0x802080d080208000,
  0x802080d080208000,
  0x108000a800801080,
  0x108000a800801080,
  0x108000a800801080
};
const int gameWon_LEN = sizeof(gameWon) / 8;


//functiile de generare a blocurilor cu diferite marimi
void generate1() {
  int x = random(1, 7);
  while (m[x][7] == 1) {
    x = random(1, 7);
  }
  lc.setLed(0, x, 7, true);
  m[x][7] = 1;
}

void generate2() {
  int x = random(1, 7), y;
  while (m[x][7] == 1) {
    x = random(1, 7);
  }
  if (x == 1) {
    y = x + 1;
  }
  else {
    y = x - 1;
  }
  lc.setLed(0, x, 7, true);
  lc.setLed(0, y, 7, true);
  m[x][7] = 1;
  m[y][7] = 1;
}


void generate3() {
  int x = random(2, 5), y, z;
  while (m[x][7] == 1) {
    x = random(1, 7);
  }
  y = x - 1;
  z = x + 1;
  lc.setLed(0, y, 7, true);
  lc.setLed(0, x, 6, true);
  lc.setLed(0, z, 7, true);
  m[y][7] = 1;
  m[x][6] = 1;
  m[z][7] = 1;
}


//functia in care BigBoss trage
void bigBossShoots() {
  int x = random(1, 6);
  while (m[x][6] == 1 || m[x][5] == 1 || m[x][4] == 1)
    x = random(1, 6);
  if (x == 1) {
    lc.setLed(0, x, 6, true);
    m[x][6] = 1;
  }
  if (x == 2) {
    lc.setLed(0, x, 5, true);
    m[x][5] = 1;
  }
  if (x == 3) {
    lc.setLed(0, x, 4, true);
    m[x][4] = 1;
  }
  if (x == 4) {
    lc.setLed(0, x, 5, true);
    m[x][5] = 1;
  }
  if (x == 5) {
    lc.setLed(0, x, 6, true);
    m[x][6] = 1;
  }
}


//functia pentru caderea blocurilor generate
void fall() {
  for (int i = 0; i <= 7; i++)
    for (int j = 0; j <= 7; j++) {
      if (m[i][j] == 1) {
        lc.setLed(0, i, j - 1, true);
        lc.setLed(0, i, j, false);
        m[i][j - 1] = 1;
        m[i][j] = 0;
      }
    }
}

void displayImage(uint64_t image) {
  for (int i = 0; i < 8; i++) {
    byte r = (image >> i * 8) & 0xFF;
    for (int j = 0; j < 8; j++) {
      lc.setLed(0, i, j, bitRead(r, j));
    }
  }
}

//funcita in apare BigBoss, aici se foloseste imaginea pentru aparitia lui la inceputul nivelului 5   
//se apeleaza o singura data cu ajutorul variabilei "ok" 
void bigBoss() {
  int i, j;
  for (i = 0; i <= 7; i++)
    for (j = 0; j <= 7; j++)
      m[i][j] = 0;
  i = 0;
  while (i < bigBossComes_LEN) {
    displayImage(bigBossComes[i]);
    i++;
    delay(500);
  }
  ok = 1;
}


void shoot() {
  for (int i = 2; i <= 7; i++)
  {
    lc.setLed(0, row, i, true);
    if (m[row][i + 1] == 1) {
      lc.setLed(0, row, i + 1, false);
      m[row][i + 1] = 0;
      count++;
    }
    delay(20);
    lc.setLed(0, row, i, false);
  }
}


void checkGameLost() {
  if (m[row][0] == 1) {
    lives--;
    m[row][0] = 0;
        if (lives == 2) {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("  2 LIVES LEFT"); 
    }
    if (lives == 1) {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("  1 LIFE LEFT"); 
    }
  }

  if (m[row + 1][0] == 1) {
    lives--;
    m[row + 1][0] = 0;
       if (lives == 2) {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("  2 LIVES LEFT");
    }
    if (lives == 1) {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("  1 LIFE LEFT"); 
    }
  }

  if (m[row - 1][0] == 1) {
    lives--;
    m[row - 1][0] = 0;
    if (lives == 2) {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("  2 LIVES LEFT");
    }
    if (lives == 1) {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("  1 LIFE LEFT"); 
    }
  }

  if (m[row][1] == 1) {
    lives--;
    m[row][1] = 0;
        if (lives == 2) {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("  2 LIVES LEFT"); 
    }
    if (lives == 1) {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("  1 LIFE LEFT");
    }
  }

  if (lives == 0) {
    lc.clearDisplay(0);
    lc.setRow(0, 3, B00011000);
    lc.setRow(0, 4, B00011000);
    delay(500);
    lc.setRow(0, 2, B00100100);
    lc.setRow(0, 5, B00100100);
    delay(500);
    lc.setRow(0, 1, B01000010);
    lc.setRow(0, 6, B01000010);
    delay(500);
    lc.setRow(0, 0, B10000001);
    lc.setRow(0, 7, B10000001);
    delay(100);

      
      lc.clearDisplay(0);
      lcd.setCursor(0, 0);
      lcd.print("Game over! Lvl:");
      lcd.print(lvl);
      lcd.setCursor(0, 1);
      lcd.print("Press to start");
      while (digitalRead(SW) != 0) {
        lc.clearDisplay(0); 
      }
      lcd.clear();
      prevTimeGenerate = 0;
      prevTimeFall = 0;
      lvl = 1;
      lives = 3;
      count = 0;
      lc.clearDisplay(0);
      for (int i = 0; i <= 7; i++)
        for (int j = 0; j <= 7; j++) {
          lc.setLed(0, i, j, false);
          m[i][j] = 0;
        }
      delay(500);
  }
}


void game() {
    
  //resetez timpii pentru generarea, respectiv caderea blocului la fiecare apel al functiei game()
  unsigned long currentTimeGenerate = millis();
  unsigned long currentTimeFall = millis();
  unsigned long currentBigBossTime = millis();
  unsigned long currentMoveTime = millis();

  //in functie de cati inamici am impuscat creste nivelul
  if (count > 15 && count <= 30) {
    lvl = 2;
  }
  else  if (count > 30 && count <= 60) {
    lvl = 3;
  }
  else if (count > 60 && count <= 90) {
    lvl = 4;
  }
  else if (count > 90) {
    lvl = 5;
  }

  switch (lvl) {
  case  1: {
    if (currentTimeGenerate - prevTimeGenerate >= intervalGenerate1) {
      prevTimeGenerate = currentTimeGenerate;
      generate1();
    }
    if (currentTimeFall - prevTimeFall >= intervalFall1) {
      prevTimeFall = currentTimeFall;
      fall();
    }
    break;
  }
  case 2: {
    if (currentTimeGenerate - prevTimeGenerate >= intervalGenerate2) {
      prevTimeGenerate = currentTimeGenerate;
      generate1();
    }
    if (currentTimeFall - prevTimeFall >= intervalFall2) {
      prevTimeFall = currentTimeFall;
      fall();
    }
    break;
  }
  case 3: {
    if (currentTimeGenerate - prevTimeGenerate >= intervalGenerate3) {
      prevTimeGenerate = currentTimeGenerate;
      generate2();
    }
    if (currentTimeFall - prevTimeFall >= intervalFall3) {
      prevTimeFall = currentTimeFall;
      fall();
    }
    break;
  }
  case 4: {
    if (currentTimeGenerate - prevTimeGenerate >= intervalGenerate4) {
      prevTimeGenerate = currentTimeGenerate;
      generate3();
    }
    if (currentTimeFall - prevTimeFall >= intervalFall4) {
      prevTimeFall = currentTimeFall;
      fall();
    }
    break;
  }
  case 5: {
    if (ok == 0) {
      bigBoss();
    }
    //setez imaginea lui BigBoss pe ecran  
    lc.setRow(0, 1, B00000001);
    lc.setRow(0, 2, B00000011);
    lc.setRow(0, 3, B00000111);
    lc.setRow(0, 4, B00000011);
    lc.setRow(0, 5, B00000001);

    if (currentBigBossTime - prevBigBossTime >= intervalShootBigBoss) {
      prevBigBossTime = currentBigBossTime;
      bigBossShoots();
    }

    if (currentTimeFall - prevTimeFall >= intervalFallBigBoss) {
      prevTimeFall = currentTimeFall;
      fall();
    }

    if (digitalRead(SW) == 0 && row != 6)   //row diferit de 6 pentru ca acolo loveste pe langa BigBoss
      shots++;

    //conditia de infrangere a lui BigBoss
    if (shots == 15) {
      int i, j;
      for (i = 0; i <= 7; i++)
        for (j = 0; j <= 7; j++) {
          m[i][j] = 0;
        }
      i = 0;
      while (i < gameWon_LEN) {
        displayImage(gameWon[i]);
        i++;
        delay(200);
      }

      lcd.setCursor(0, 0);
      lcd.print("You won! Press");
      lcd.setCursor(0, 1);
      lcd.print("to play again.");

      while (digitalRead(SW) != 0) {
        lc.clearDisplay(0);
      }
      delay(500);
      lcd.clear();
      lvl = 1;
      count = 0;
      lives = 3;
      shots = 0;
    }
  }
  }
}

#endif // SPACEINVADERS_H
