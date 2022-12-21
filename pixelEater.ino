#include <LiquidCrystal.h>
#include "LedControl.h"
#include <EEPROM.h>

#define ASCII_CODE_A 65
#define ASCII_CODE_Z 90

#define LCD_BRIGHTNESS_ADDRESS 100
#define LCD_CONTRAST_ADDRESS 200
#define MTX_BRIGHTNESS_ADDRESS 300
#define BUZZER_SOUND_ADDRESS 400
#define HIGHSCORE_ADDRESS 800

#define FIRST_HIGHSCORE 1
#define FIRST_HIGHSCORE_CH1 11
#define FIRST_HIGHSCORE_CH2 12
#define FIRST_HIGHSCORE_CH3 13
#define FIRST_MULTI256 14

#define SECOND_HIGHSCORE 2
#define SECOND_HIGHSCORE_CH1 21
#define SECOND_HIGHSCORE_CH2 22
#define SECOND_HIGHSCORE_CH3 23
#define SECOND_MULTI256 24

#define THIRD_HIGHSCORE 3
#define THIRD_HIGHSCORE_CH1 31
#define THIRD_HIGHSCORE_CH2 32
#define THIRD_HIGHSCORE_CH3 33
#define THIRD_MULTI256 34

#define FOURTH_HIGHSCORE 4
#define FOURTH_HIGHSCORE_CH1 41
#define FOURTH_HIGHSCORE_CH2 42
#define FOURTH_HIGHSCORE_CH3 43
#define FOURTH_MULTI256 44

#define FIFTH_HIGHSCORE 5
#define FIFTH_HIGHSCORE_CH1 51
#define FIFTH_HIGHSCORE_CH2 52
#define FIFTH_HIGHSCORE_CH3 53
#define FIFTH_MULTI256 54

//shapes array
const byte images[][8][8] = {
  { { 0, 0, 0, 0, 0, 0, 0, 0 },
    { 0, 0, 1, 0, 1, 0, 0, 0 },
    { 0, 1, 1, 1, 1, 1, 0, 0 },
    { 0, 1, 1, 1, 1, 1, 0, 0 },
    { 0, 0, 1, 1, 1, 0, 0, 0 },
    { 0, 0, 0, 1, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0 }

  },
  {
    { 0, 0, 0, 0, 0, 0, 0, 0 },
    { 0, 0, 1, 1, 1, 1, 1, 0 },
    { 0, 0, 0, 1, 0, 0, 1, 0 },
    { 0, 0, 0, 0, 1, 0, 1, 0 },
    { 0, 0, 0, 1, 0, 1, 1, 0 },
    { 0, 0, 1, 0, 0, 0, 1, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0 },
  },
  {

    { 0, 0, 0, 0, 0, 0, 0, 0 },
    { 0, 1, 0, 0, 0, 0, 1, 0 },
    { 0, 0, 1, 0, 0, 1, 0, 0 },
    { 0, 1, 1, 0, 0, 1, 1, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0 },
    { 0, 0, 1, 1, 1, 1, 0, 0 },
    { 0, 1, 0, 0, 0, 0, 1, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0 },

  },
  {

    { 0, 0, 0, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 1, 1, 0 },
    { 0, 1, 1, 0, 0, 1, 1, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 1, 1, 0, 0, 0 },
    { 0, 0, 1, 1, 1, 1, 0, 0 },
    { 0, 0, 0, 1, 1, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0 },

  }
};

byte scoreMatrix[8][8] = {
  { 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0 },
};

int letter = 0;
unsigned long scrollingTime = 0;

String aboutStuff = "                Pixel Eater by github accounts ralucsandu & FiloteSerban   ";
String howToPlay = "                Move the joystick to control the blinking pixel. Eat all of the non blinking pixels!    ";

bool aboutOK = false;
bool howToPlayOK = false;

//matrix pins
const byte dinPin = 12;
const byte clockPin = 11;
const byte loadPin = 10;

LedControl lc = LedControl(dinPin, clockPin, loadPin, 1);

const byte matrixLedsOn = B11111111;

//buzzer pins
const byte buzzerPin = A2;
const int buzzerDuration = 100;
const int soundScrollDuration = 100;

const int scroll_sound = 4186;
const int left_right_sound = 4000;
const int eat_sound = 835;

//lcd pins
const byte rs = 13;
const byte en = 8;
const byte d4 = 7;
const byte d5 = 3;
const byte d6 = 5;
const byte d7 = 4;

const byte pinA = 6;
const byte pinV0 = 9;

//joystick pins
const byte pinSW = 2;
const byte pinX = A0;
const byte pinY = A1;

const bool switchOff = 1;
int xValue = 0;
int yValue = 0;

int xValueGame = 0;
int yValueGame = 0;

const int minThreshold = 300;
const int maxThreshold = 800;

bool joyMovedy = false;
bool joyMovedx = false;
bool joyMoved = false;

//menu stuff
int menuPosition = 0;
int menuSettingsPosition = 0;

const int numberOfMenuOptions = 6;
const int numberOfMenuSettingsOptions = 5;

unsigned int highscore = 0;

unsigned int level = 1;

unsigned int lcdContrast = 7;
unsigned int lcdBrightness = 8;
unsigned int matrixBrightness = 0;
const int maxContrastBrightness = 255;

bool sound = true;

bool introOK = true;

char menuStructure[numberOfMenuOptions][20] = {
  " Restart HS",
  " Start Game",
  " HighScore",
  " Settings",
  " About",
  " How to play"
};

char menuSettingsStructure[numberOfMenuSettingsOptions][20] = {
  "Level     ",
  "LCD Contrast",
  "LCD Brightness",
  "Mtx Brightness",
  "Sound ON/OFF",
};

bool gameMenuSelected = true;
bool settingsMenuSelected = true;

unsigned long lastDebounce = 0;
unsigned long minReactionTime = 100;

const int matrixSize = 8;
const int invalidPosition = -1;
const int lcdLength = 16;

byte down[matrixSize] = {
  B00000,
  B00000,
  B00000,
  B00000,
  B00000,
  B11111,
  B01110,
  B00100,
};

byte up[matrixSize] = {
  B00100,
  B01110,
  B11111,
  B00000,
  B00000,
  B00000,
  B00000,
};

byte full[matrixSize] = {
  B11111,
  B11111,
  B11111,
  B11111,
  B11111,
  B11111,
  B11111,
  B11111,
};

byte heart[matrixSize] = {
  B00000,
  B11011,
  B11111,
  B11111,
  B01110,
  B00100,
  B00000,
  B00000
};

byte hungryFace[matrixSize] = {
  B00000,
  B00111,
  B01010,
  B11100,
  B11000,
  B11100,
  B01110,
  B00111
};

byte upDown[matrixSize] = {
  B00100,
  B01110,
  B11111,
  B00000,
  B00000,
  B11111,
  B01110,
  B00100
};

bool buttonDecider = 0;
bool buttonSettingsDecider = 0;

LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

int lives = 3;

int score = 0;       //calculates the points per round
int scoreTotal = 0;  //calculates the sum of scores

int timerLevel1 = 60;
int timerLevel2 = 30;
int timerLevel3 = 20;
int timerLevel4 = 15;
int timer = 0;

unsigned long sec = 0;  //increases second by second

bool timeOK = true;
bool messageOK = true;
bool backOK = false;

bool matrix[matrixSize][matrixSize];

int playerX = 1;  //variables used for breaking the walls
int playerY = 1;

char name[3] = { 65, 65, 65 };  //AAA
int letterPosition = 0;

int randomNr = 0;

void playSound(int soundType) {
  if (sound == true)
    tone(buzzerPin, soundType, soundScrollDuration);
  else
    noTone(buzzerPin);
}

void enterName() {
  lcd.setCursor(0, 0);
  lcd.print("  ENTER NAME:  ");
  lcd.setCursor(0, 1);
  lcd.print("     <");
  lcd.print(name[0]);
  lcd.print(name[1]);
  lcd.print(name[2]);
  lcd.print(">     ");
  lcd.setCursor(15, 1);
  lcd.write(byte(0));
  lcd.setCursor(15, 0);
  lcd.write(byte(1));
  lcd.setCursor(6 + letterPosition, 1);
  lcd.print(" ");
  yValue = analogRead(pinY);

  if (yValue < minThreshold && joyMovedy == false) {
    if (name[letterPosition] < ASCII_CODE_Z) {
      playSound(scroll_sound);
      name[letterPosition]++;
    }
    joyMovedy = true;
  }

  else if (yValue >= minThreshold && yValue <= maxThreshold && joyMovedy == true) {
    joyMovedy = false;
  }

  else if (yValue > maxThreshold && joyMovedy == false) {
    if (name[letterPosition] > ASCII_CODE_A) {
      playSound(scroll_sound);
      name[letterPosition]--;
    }
    joyMovedy = true;
  }

  xValue = analogRead(pinX);

  if (xValue < minThreshold && joyMovedx == false) {
    if (letterPosition > 0) {
      playSound(left_right_sound);
      letterPosition--;
    }
    joyMovedx = true;
  }

  else if (xValue >= minThreshold && xValue <= maxThreshold && joyMovedx == true) {
    joyMovedx = false;
  }

  else if (xValue > maxThreshold && joyMovedx == false) {
    if (letterPosition < 2) {
      playSound(left_right_sound);
      letterPosition++;
    }
    joyMovedx = true;
  }
}

void turnOnPixel(int posx, int posy) {
  matrix[posx][posy] = 1;
  lc.setLed(0, posx, posy, matrix[posx][posy]);
}

void turnOffPixel(int posx, int posy) {
  matrix[posx][posy] = 0;
  lc.setLed(0, posx, posy, matrix[posx][posy]);
}

int sumMatrix(byte m[8][8]) {
  int sum = 0;
  for (int i = 0; i < matrixSize; i++)
    for (int j = 0; j < matrixSize; ++j)
      sum += m[i][j];
  return sum;
}

void level12() {
  turnOnPixel(playerX % matrixSize, playerY % matrixSize);
  int auxPlayerX = playerX;
  int auxPlayerY = playerY;
  xValueGame = analogRead(pinX);
  yValueGame = analogRead(pinY);

  if (yValueGame < minThreshold && joyMoved == false && xValueGame >= minThreshold && xValueGame <= maxThreshold) {
    playerX--;
    if (playerX == invalidPosition)
      playerX = matrixSize-1;
    joyMoved = true;
  }

  else if (yValueGame >= minThreshold && yValueGame <= maxThreshold && joyMoved == true && xValueGame >= minThreshold && xValueGame <= maxThreshold) {
    joyMoved = false;
  }

  else if (yValueGame > maxThreshold && joyMoved == false && xValueGame >= minThreshold && xValueGame <= maxThreshold) {
    playerX++;
    joyMoved = true;
  }

  else if (xValueGame < minThreshold && joyMoved == false && yValueGame >= minThreshold && yValueGame <= maxThreshold) {
    playerY--;
    if (playerY == invalidPosition)
      playerY = matrixSize-1;
    joyMoved = true;
  }

  else if (xValueGame >= minThreshold && xValueGame <= maxThreshold && joyMoved == true && yValueGame >= minThreshold && yValueGame <= maxThreshold) {
    joyMoved = false;
  }

  else if (xValueGame > maxThreshold && joyMoved == false && yValueGame >= minThreshold && yValueGame <= maxThreshold) {
    playerY++;
    joyMoved = true;
  }

  if (images[randomNr][auxPlayerX % matrixSize][auxPlayerY % matrixSize] == 1 && matrix[auxPlayerX % matrixSize][auxPlayerY % matrixSize] == 1 && (auxPlayerX != playerX || auxPlayerY != playerY)) {
    scoreMatrix[auxPlayerX % matrixSize][auxPlayerY % matrixSize] = 1;
    if (score != sumMatrix(scoreMatrix) && score != sumMatrix(scoreMatrix) * 2)
      playSound(eat_sound);
    if (level == 1)
      score = sumMatrix(scoreMatrix);
    else if (level == 2)
      score = 2 * sumMatrix(scoreMatrix);
    turnOffPixel(auxPlayerX % matrixSize, auxPlayerY % matrixSize);
  } else
    turnOffPixel(auxPlayerX % matrixSize, auxPlayerY % matrixSize);
}

void level34() {

  turnOnPixel(playerX, playerY);
  int auxPlayerX = playerX;
  int auxPlayerY = playerY;
  xValueGame = analogRead(pinX);
  yValueGame = analogRead(pinY);

  if (yValueGame < minThreshold && joyMoved == false && xValueGame >= minThreshold && xValueGame <= maxThreshold) {
    if (playerX > 0)
      playerX--;
    joyMoved = true;
  }

  else if (yValueGame >= minThreshold && yValueGame <= maxThreshold && joyMoved == true && xValueGame >= minThreshold && xValueGame <= maxThreshold) {
    joyMoved = false;
  }

  else if (yValueGame > maxThreshold && joyMoved == false && xValueGame >= minThreshold && xValueGame <= maxThreshold) {
    if (playerX < 7)
      playerX++;
    joyMoved = true;
  }

  else if (xValueGame < minThreshold && joyMoved == false && yValueGame >= minThreshold && yValueGame <= maxThreshold) {
    if (playerY > 0)
      playerY--;
    joyMoved = true;
  }

  else if (xValueGame >= minThreshold && xValueGame <= maxThreshold && joyMoved == true && yValueGame >= minThreshold && yValueGame <= maxThreshold) {
    joyMoved = false;
  }

  else if (xValueGame > maxThreshold && joyMoved == false && yValueGame >= minThreshold && yValueGame <= maxThreshold) {
    if (playerY < 7)
      playerY++;
    joyMoved = true;
  }

  if (images[randomNr][auxPlayerX][auxPlayerY] == 1 && matrix[auxPlayerX][auxPlayerY] == 1 && (auxPlayerX != playerX || auxPlayerY != playerY)) {
    scoreMatrix[auxPlayerX][auxPlayerY] = 1;
    if (score != sumMatrix(scoreMatrix) * 3 && score != sumMatrix(scoreMatrix) * 4)
      playSound(eat_sound);
    if (level == 3)
      score = 3 * sumMatrix(scoreMatrix);
    else if (level == 4)
      score = 4 * sumMatrix(scoreMatrix);
    turnOffPixel(auxPlayerX, auxPlayerY);
  } else
    turnOffPixel(auxPlayerX, auxPlayerY);
}

bool levelOK = false;

void game() {
  if (levelOK == false) {
    randomNr = random(0, 4);
    for (int i = 0; i < matrixSize; i++)
      for (int j = 0; j < matrixSize; ++j) {
        if (images[randomNr][i][j] == 1)
          turnOnPixel(i, j);
        if ((i == 0 || i == 7 || j == 0 || j == 7) && (level == 3 || level == 4))
          turnOnPixel(i, j);
      }

    levelOK = true;
  }
  if (levelOK)
    if (level == 1 || level == 2)
      level12();
  if (level == 3 || level == 4)
    level34();
}

int backToMenu(bool &menu) {
  xValue = analogRead(pinX);
  if (xValue < minThreshold && joyMovedx == false) {
    playSound(left_right_sound);
    joyMovedx = true;
    backOK = true;
  } else if (xValue > minThreshold && xValue < maxThreshold && joyMovedx == true && backOK) {
    joyMovedx = false;
    lcd.clear();
    menu = true;
    backOK = false;
    return 1;
  }
  return 0;
}

int displayAbout() {
  if (!aboutOK)
    lcd.clear();
  aboutOK = true;
  lcd.setCursor(0, 0);
  lcd.print("<");
  lcd.setCursor(6, 0);
  lcd.print("About");
  if (aboutOK == true && letter <= aboutStuff.length() - lcdLength) {
    lcd.setCursor(0, 1);
    if (millis() - scrollingTime >= 500) {
      scrollingTime = millis();
      lcd.setCursor(0, 1);
      lcd.print(aboutStuff.substring(letter, lcdLength + letter));
      letter++;
    }
  }
  if (backToMenu(gameMenuSelected)) {
    aboutOK = false;
    letter = 0;
    return 0;
  }
}

int displayHowToPlay() {
  if (!howToPlayOK)
    lcd.clear();
  howToPlayOK = true;
  lcd.setCursor(0, 0);
  lcd.print("<");
  lcd.setCursor(3, 0);
  lcd.print("How to play");
  if (howToPlayOK == true && letter <= howToPlay.length() - lcdLength) {
    lcd.setCursor(0, 1);
    if (millis() - scrollingTime >= 500) {
      scrollingTime = millis();
      lcd.setCursor(0, 1);
      lcd.print(howToPlay.substring(letter, lcdLength + letter));
      letter++;
    }
  }
  if (backToMenu(gameMenuSelected)) {
    howToPlayOK = false;
    letter = 0;
    return 0;
  }
}

void menuScroller() {
  lcd.setCursor(0, 0);
  lcd.print("   Game Menu   ");
  lcd.setCursor(15, 1);
  lcd.write(byte(0));
  lcd.setCursor(15, 0);
  lcd.write(byte(1));
  lcd.setCursor(0, 1);
  lcd.print(">");

  lcd.setCursor(1, 1);
  lcd.print(menuStructure[menuPosition % numberOfMenuOptions]);

  yValue = analogRead(pinY);

  if (yValue < minThreshold && joyMovedy == false && menuPosition > 0) {
    playSound(scroll_sound);
    menuPosition--;
    lcd.clear();
    joyMovedy = true;
  }

  else if (yValue >= minThreshold && yValue <= maxThreshold && joyMovedy == true) {
    joyMovedy = false;
  }

  else if (yValue > maxThreshold && joyMovedy == false) {
    playSound(scroll_sound);
    menuPosition++;
    lcd.clear();
    joyMovedy = true;
  }

  xValue = analogRead(pinX);

  if (xValue > maxThreshold && joyMovedx == false) {
    playSound(left_right_sound);
    joyMovedx = true;
  }

  else if (xValue >= minThreshold && xValue <= maxThreshold && joyMovedx == true) {
    gameMenuSelected = false;
    joyMovedx = false;
  }
}

bool restartDecider = true;
int restartHS() {
  lcd.setCursor(1, 0);
  lcd.print("Are you sure?  ");
  lcd.setCursor(0, 1);
  lcd.print("  ");
  lcd.setCursor(3, 1);
  lcd.print("YES   ");
  lcd.setCursor(10, 1);
  lcd.print("NO    ");

  if (restartDecider) {
    lcd.setCursor(2, 1);
    lcd.print(">");
    lcd.setCursor(9, 1);
    lcd.print(" ");
  } else {
    lcd.setCursor(9, 1);
    lcd.print(">");
    lcd.setCursor(2, 1);
    lcd.print(" ");
  }

  xValue = analogRead(pinX);

  if (xValue < minThreshold && joyMovedx == false) {
    playSound(left_right_sound);
    if (!restartDecider)
      restartDecider = !restartDecider;
    joyMovedx = true;
  }

  else if (xValue >= minThreshold && xValue <= maxThreshold && joyMovedx == true) {
    joyMovedx = false;
  }

  else if (xValue > maxThreshold && joyMovedx == false) {
    playSound(left_right_sound);
    if (restartDecider)
      restartDecider = !restartDecider;
    joyMovedx = true;
  }

  if (digitalRead(pinSW) == switchOff) {
    if (millis() - lastDebounce > minReactionTime && buttonDecider != 0) {
      if (restartDecider) {
        putInitialHStoEEPROM();
      }
      lcd.clear();
      gameMenuSelected = true;
      lastDebounce = 0;
      buttonDecider = 0;
      return 0;
    }
    lastDebounce = millis();
    buttonDecider = 1;
  }
}

void settingsScroller() {
  lcd.setCursor(15, 1);
  lcd.write(byte(0));
  lcd.setCursor(15, 0);
  lcd.write(byte(1));
  lcd.setCursor(0, 1);
  lcd.print(">");
  lcd.setCursor(0, 0);
  lcd.print("<");

  lcd.setCursor(1, 0);
  lcd.print("   Settings");
  lcd.setCursor(1, 1);
  lcd.print(menuSettingsStructure[menuSettingsPosition % numberOfMenuSettingsOptions]);

  yValue = analogRead(pinY);

  if (yValue < minThreshold && joyMovedy == false && menuSettingsPosition > 0) {
    playSound(scroll_sound);
    menuSettingsPosition--;
    lcd.clear();
    joyMovedy = true;
  }

  else if (yValue >= minThreshold && yValue <= maxThreshold && joyMovedy == true) {
    joyMovedy = false;
  }

  else if (yValue > maxThreshold && joyMovedy == false) {
    playSound(scroll_sound);
    menuSettingsPosition++;
    lcd.clear();
    joyMovedy = true;
  }

  xValue = analogRead(pinX);

  if (xValue > maxThreshold && joyMovedx == false) {
    playSound(left_right_sound);
    joyMovedx = true;
  }

  else if (xValue >= minThreshold && xValue <= maxThreshold && joyMovedx == true && backOK == false) {
    settingsMenuSelected = false;
    joyMovedx = false;
  }

  backToMenu(gameMenuSelected);
}

int showHighscorePosition = 0;
int highscores[5] = { 0, 0, 0, 0, 0 };
char names[5][3] = { { 'T', 'B', 'D' }, { 'T', 'B', 'D' }, { 'T', 'B', 'D' }, { 'T', 'B', 'D' }, { 'T', 'B', 'D' } };

void showHighscore() {
  lcd.setCursor(0, 0);
  lcd.print("<");
  lcd.setCursor(1, 0);
  lcd.print("   Highscore  ");
  lcd.setCursor(15, 1);
  lcd.write(byte(0));
  lcd.setCursor(15, 0);
  lcd.write(byte(1));
  lcd.setCursor(0, 1);
  lcd.print("   ");
  if (showHighscorePosition % 5 == 0) {
    names[0][0] = EEPROM.read(FIRST_HIGHSCORE_CH1);
    names[0][1] = EEPROM.read(FIRST_HIGHSCORE_CH2);
    names[0][2] = EEPROM.read(FIRST_HIGHSCORE_CH3);

    lcd.setCursor(3, 1);
    lcd.print("1.");
    lcd.print(names[0][0]);
    lcd.setCursor(6, 1);
    lcd.print(names[0][1]);
    lcd.setCursor(7, 1);
    lcd.print(names[0][2]);
    lcd.print(": ");
    lcd.print(EEPROM.read(FIRST_HIGHSCORE) + 256 * EEPROM.read(FIRST_MULTI256));
    lcd.print(" ");
  } else if (showHighscorePosition % 5 == 1) {
    names[1][0] = EEPROM.read(SECOND_HIGHSCORE_CH1);
    names[1][1] = EEPROM.read(SECOND_HIGHSCORE_CH2);
    names[1][2] = EEPROM.read(SECOND_HIGHSCORE_CH3);

    lcd.setCursor(3, 1);
    lcd.print("2.");
    lcd.print(names[1][0]);
    lcd.setCursor(6, 1);
    lcd.print(names[1][1]);
    lcd.setCursor(7, 1);
    lcd.print(names[1][2]);
    lcd.print(": ");
    lcd.print(EEPROM.read(SECOND_HIGHSCORE) + 256 * EEPROM.read(SECOND_MULTI256));
    lcd.print("  ");
  } else if (showHighscorePosition % 5 == 2) {
    names[2][0] = EEPROM.read(THIRD_HIGHSCORE_CH1);
    names[2][1] = EEPROM.read(THIRD_HIGHSCORE_CH2);
    names[2][2] = EEPROM.read(THIRD_HIGHSCORE_CH3);

    lcd.setCursor(3, 1);
    lcd.print("3.");
    lcd.print(names[2][0]);
    lcd.setCursor(6, 1);
    lcd.print(names[2][1]);
    lcd.setCursor(7, 1);
    lcd.print(names[2][2]);
    lcd.print(": ");
    lcd.print(EEPROM.read(THIRD_HIGHSCORE) + 256 * EEPROM.read(THIRD_MULTI256));
    lcd.print("  ");
  } else if (showHighscorePosition % 5 == 3) {
    names[3][0] = EEPROM.read(FOURTH_HIGHSCORE_CH1);
    names[3][1] = EEPROM.read(FOURTH_HIGHSCORE_CH2);
    names[3][2] = EEPROM.read(FOURTH_HIGHSCORE_CH3);

    lcd.setCursor(3, 1);
    lcd.print("4.");
    lcd.print(names[3][0]);
    lcd.setCursor(6, 1);
    lcd.print(names[3][1]);
    lcd.setCursor(7, 1);
    lcd.print(names[3][2]);
    lcd.print(": ");
    lcd.print(EEPROM.read(FOURTH_HIGHSCORE) + 255 * EEPROM.read(FOURTH_MULTI256));
    lcd.print("  ");
  } else if (showHighscorePosition % 5 == 4) {
    names[4][0] = EEPROM.read(FIFTH_HIGHSCORE_CH1);
    names[4][1] = EEPROM.read(FIFTH_HIGHSCORE_CH2);
    names[4][2] = EEPROM.read(FIFTH_HIGHSCORE_CH3);

    lcd.setCursor(3, 1);
    lcd.print("5.");
    lcd.print(names[4][0]);
    lcd.setCursor(6, 1);
    lcd.print(names[4][1]);
    lcd.setCursor(7, 1);
    lcd.print(names[4][2]);
    lcd.print(": ");
    lcd.print(EEPROM.read(FIFTH_HIGHSCORE) + 256 * EEPROM.read(FIFTH_MULTI256));
    lcd.print("  ");
  }

  yValue = analogRead(pinY);

  if (yValue < minThreshold && joyMovedy == false) {
    playSound(scroll_sound);
    if (showHighscorePosition > 0)
      showHighscorePosition--;
    joyMovedy = true;
  }

  else if (yValue >= minThreshold && yValue <= maxThreshold && joyMovedy == true) {
    joyMovedy = false;
  }

  else if (yValue > maxThreshold && joyMovedy == false) {
    playSound(scroll_sound);
    showHighscorePosition++;
    joyMovedy = true;
  }

  backToMenu(gameMenuSelected);
}

void setLevel() {
  lcd.setCursor(0, 0);
  lcd.print("<");
  lcd.setCursor(1, 0);
  lcd.print("     Level    ");
  lcd.setCursor(15, 1);
  lcd.write(byte(0));
  lcd.setCursor(15, 0);
  lcd.write(byte(1));
  lcd.setCursor(0, 1);
  lcd.print("        ");
  lcd.print(level);
  lcd.print(" ");

  yValue = analogRead(pinY);

  if (yValue < minThreshold && joyMovedy == false) {
    if (level < 4) {
      playSound(scroll_sound);
      level++;
    }
    joyMovedy = true;
  }

  else if (yValue >= minThreshold && yValue <= maxThreshold && joyMovedy == true) {
    joyMovedy = false;
  }

  else if (yValue > maxThreshold && joyMovedy == false) {
    if (level > 1) {
      playSound(scroll_sound);
      level--;
    }
    joyMovedy = true;
  }
  backToMenu(settingsMenuSelected);
}

void setContrast() {
  lcd.setCursor(0, 0);
  lcd.print("<");
  lcd.setCursor(1, 0);
  lcd.print("    Contrast  ");
  lcd.setCursor(15, 0);
  lcd.write(byte(5));
  lcd.setCursor(0, 1);
  lcdContrast = readStringFromEEPROM(LCD_CONTRAST_ADDRESS).toInt();
  for (int i = 0; i < lcdContrast; ++i)
    lcd.write(byte(2));

  for (int i = lcdContrast; i < lcdLength; ++i)
    lcd.print(" ");

  yValue = analogRead(pinY);

  if (yValue < minThreshold && joyMovedy == false) {
    if (lcdContrast < lcdLength) {
      playSound(scroll_sound);
      lcdContrast++;
    }
    joyMovedy = true;
  }

  else if (yValue >= minThreshold && yValue <= maxThreshold && joyMovedy == true) {
    joyMovedy = false;
  }

  else if (yValue > maxThreshold && joyMovedy == false) {
    if (lcdContrast > 0) {
      playSound(scroll_sound);
      lcdContrast--;
    }
    joyMovedy = true;
  }
  updateStringToEEPROM(LCD_CONTRAST_ADDRESS, String(lcdContrast));
  analogWrite(pinV0, (maxContrastBrightness / lcdLength) * lcdContrast);
  backToMenu(settingsMenuSelected);
}

void setLcdBrightness() {
  lcd.setCursor(0, 0);
  lcd.print("<");
  lcd.setCursor(1, 0);
  lcd.print("Lcd Brightness ");
  lcd.setCursor(15, 0);
  lcd.write(byte(5));
  lcd.setCursor(0, 1);
  lcdBrightness = readStringFromEEPROM(LCD_BRIGHTNESS_ADDRESS).toInt();
  for (int i = 0; i < lcdBrightness; ++i)
    lcd.write(byte(2));

  for (int i = lcdBrightness; i < lcdLength; ++i)
    lcd.print(" ");

  yValue = analogRead(pinY);

  if (yValue < minThreshold && joyMovedy == false) {
    if (lcdBrightness < lcdLength) {
      playSound(scroll_sound);
      lcdBrightness++;
    }
    joyMovedy = true;
  }

  else if (yValue >= minThreshold && yValue <= maxThreshold && joyMovedy == true) {
    joyMovedy = false;
  }

  else if (yValue > maxThreshold && joyMovedy == false) {
    if (lcdBrightness > 0) {
      playSound(scroll_sound);
      lcdBrightness--;
    }
    joyMovedy = true;
  }
  updateStringToEEPROM(LCD_BRIGHTNESS_ADDRESS, String(lcdBrightness));
  analogWrite(pinA, (maxContrastBrightness / lcdLength) * lcdBrightness);
  backToMenu(settingsMenuSelected);
}

void setMatrixBrightness() {
  lcd.setCursor(0, 0);
  lcd.print("<");
  lcd.setCursor(1, 0);
  lcd.print(" MtxBrightness");
  lcd.setCursor(15, 0);
  lcd.write(byte(5));
  lcd.setCursor(0, 1);
  matrixBrightness = readStringFromEEPROM(MTX_BRIGHTNESS_ADDRESS).toInt();
  for (int i = 0; i < matrixBrightness; ++i)
    lcd.write(byte(2));

  for (int i = matrixBrightness; i < lcdLength; ++i)
    lcd.print(" ");

  yValue = analogRead(pinY);

  if (yValue < minThreshold && joyMovedy == false) {
    if (matrixBrightness < lcdLength) {
      playSound(scroll_sound);
      matrixBrightness++;
    }
    joyMovedy = true;
  }

  else if (yValue >= minThreshold && yValue <= maxThreshold && joyMovedy == true) {
    joyMovedy = false;
  }

  else if (yValue > maxThreshold && joyMovedy == false) {
    if (matrixBrightness > 0) {
      playSound(scroll_sound);
      matrixBrightness--;
    }
    joyMovedy = true;
  }
  updateStringToEEPROM(MTX_BRIGHTNESS_ADDRESS, String(matrixBrightness));
  lc.shutdown(0, false);
  lc.setIntensity(0, matrixBrightness);
  lc.clearDisplay(0);
  for (int i = 0; i < matrixSize; ++i)
    lc.setRow(0, i, matrixLedsOn);
  if (backToMenu(settingsMenuSelected)) {
    for (int row = 0; row < matrixSize; ++row)
      for (int col = 0; col < matrixSize; ++col)
        turnOffPixel(row, col);
  }
}

void setSound() {
  lcd.setCursor(0, 0);
  lcd.print("<");
  lcd.setCursor(1, 0);
  lcd.print("    Sound    ");
  lcd.setCursor(15, 1);
  lcd.write(byte(0));
  lcd.setCursor(15, 0);
  lcd.write(byte(1));
  lcd.setCursor(0, 1);
  lcd.print("      ");
  sound = EEPROM.read(BUZZER_SOUND_ADDRESS);
  if (sound) {
    lcd.print("ON");
    lcd.print("       ");
  } else {
    lcd.print("OFF");
    lcd.print("      ");
  }

  yValue = analogRead(pinY);

  if (yValue < minThreshold && joyMovedy == false) {
    sound = !sound;
    joyMovedy = true;
  }

  else if (yValue >= minThreshold && yValue <= maxThreshold && joyMovedy == true) {
    joyMovedy = false;
  }

  else if (yValue > maxThreshold && joyMovedy == false) {
    sound = !sound;
    joyMovedy = true;
  }
  EEPROM.update(BUZZER_SOUND_ADDRESS, sound);
  backToMenu(settingsMenuSelected);
}

bool enterNameOK = false;
int hsChanged = 0;

void startGame() {
  game();
  if (messageOK) {
    messageOK = false;
    if (level == 1 || level == 2) {
      lcd.setCursor(0, 0);
      lcd.print("Hello! ^.^   ");
      lcd.setCursor(0, 1);
      lcd.print("be ready     ");
    } else {
      lcd.setCursor(0, 0);
      lcd.print("  Do not touch  ");
      lcd.setCursor(0, 1);
      lcd.print("   the border!  ");
    }
    delay(3000);
  }
  lcd.setCursor(0, 0);
  for (int i = 0; i < lives; ++i)
    lcd.write(byte(3));
  for (int i = lives; i < 3; ++i)
    lcd.print(" ");
  lcd.setCursor(3, 0);
  lcd.print("  Score:");
  lcd.print(scoreTotal + score);
  lcd.print("  ");
  lcd.setCursor(15, 0);
  lcd.print(" ");

  lcd.setCursor(0, 1);
  lcd.print("Time:");

  if (timeOK) {
    sec = millis();
    timeOK = false;
  }

  if (millis() - sec >= 1000) {
    timer++;
    timeOK = true;
  }

  if (level == 1)
    lcd.print(timerLevel1 - timer);
  else if (level == 2)
    lcd.print(timerLevel2 - timer);
  else if (level == 3)
    lcd.print(timerLevel3 - timer);
  else if (level == 4)
    lcd.print(timerLevel4 - timer);

  lcd.setCursor(7, 1);
  lcd.print("         ");
  if ((timerLevel1 - timer <= 9 && level == 1) || (timerLevel2 - timer <= 9 && level == 2) || (timerLevel3 - timer <= 9 && level == 3) || (timerLevel4 - timer <= 9 && level == 4)) {
    lcd.setCursor(6, 1);
    lcd.print(" ");
  }
  if ((((timerLevel1 - timer <= 0 && level == 1) || (timerLevel2 - timer <= 0 && level == 2) || (timerLevel3 - timer <= 0 && level == 3) || (timerLevel4 - timer <= 0 && level == 4)) && lives == 1) || lives == 0) {
    lcd.clear();
    gameMenuSelected = true;
    score = score + scoreTotal;
    if (score % 100 == 0)
      score = score * 2;  //Easter egg
    if (score > EEPROM.read(FIRST_HIGHSCORE) + EEPROM.read(FIRST_MULTI256) * 256) {
      enterNameOK = true;
      hsChanged = 1;

      EEPROM.update(FIFTH_HIGHSCORE, EEPROM.read(FOURTH_HIGHSCORE));
      EEPROM.update(FIFTH_HIGHSCORE_CH1, EEPROM.read(FOURTH_HIGHSCORE_CH1));
      EEPROM.update(FIFTH_HIGHSCORE_CH2, EEPROM.read(FOURTH_HIGHSCORE_CH2));
      EEPROM.update(FIFTH_HIGHSCORE_CH3, EEPROM.read(FOURTH_HIGHSCORE_CH3));
      EEPROM.update(FIFTH_MULTI256, EEPROM.read(FOURTH_MULTI256));

      EEPROM.update(FOURTH_HIGHSCORE, EEPROM.read(THIRD_HIGHSCORE));
      EEPROM.update(FOURTH_HIGHSCORE_CH1, EEPROM.read(THIRD_HIGHSCORE_CH1));
      EEPROM.update(FOURTH_HIGHSCORE_CH2, EEPROM.read(THIRD_HIGHSCORE_CH2));
      EEPROM.update(FOURTH_HIGHSCORE_CH3, EEPROM.read(THIRD_HIGHSCORE_CH3));
      EEPROM.update(FOURTH_MULTI256, EEPROM.read(THIRD_MULTI256));

      EEPROM.update(THIRD_HIGHSCORE, EEPROM.read(SECOND_HIGHSCORE));
      EEPROM.update(THIRD_HIGHSCORE_CH1, EEPROM.read(SECOND_HIGHSCORE_CH1));
      EEPROM.update(THIRD_HIGHSCORE_CH2, EEPROM.read(SECOND_HIGHSCORE_CH2));
      EEPROM.update(THIRD_HIGHSCORE_CH3, EEPROM.read(SECOND_HIGHSCORE_CH3));
      EEPROM.update(THIRD_MULTI256, EEPROM.read(SECOND_MULTI256));

      EEPROM.update(SECOND_HIGHSCORE, EEPROM.read(FIRST_HIGHSCORE));
      EEPROM.update(SECOND_HIGHSCORE_CH1, EEPROM.read(FIRST_HIGHSCORE_CH1));
      EEPROM.update(SECOND_HIGHSCORE_CH2, EEPROM.read(FIRST_HIGHSCORE_CH2));
      EEPROM.update(SECOND_HIGHSCORE_CH3, EEPROM.read(FIRST_HIGHSCORE_CH3));
      EEPROM.update(SECOND_MULTI256, EEPROM.read(FIRST_MULTI256));

      EEPROM.update(FIRST_HIGHSCORE, score);
      EEPROM.update(FIRST_MULTI256, score / 256);
    } else if (score > EEPROM.read(SECOND_HIGHSCORE) + EEPROM.read(SECOND_MULTI256) * 256) {
      enterNameOK = true;
      hsChanged = 2;

      EEPROM.update(FIFTH_HIGHSCORE, EEPROM.read(FOURTH_HIGHSCORE));
      EEPROM.update(FIFTH_HIGHSCORE_CH1, EEPROM.read(FOURTH_HIGHSCORE_CH1));
      EEPROM.update(FIFTH_HIGHSCORE_CH2, EEPROM.read(FOURTH_HIGHSCORE_CH2));
      EEPROM.update(FIFTH_HIGHSCORE_CH3, EEPROM.read(FOURTH_HIGHSCORE_CH3));
      EEPROM.update(FIFTH_MULTI256, EEPROM.read(FOURTH_MULTI256));

      EEPROM.update(FOURTH_HIGHSCORE, EEPROM.read(THIRD_HIGHSCORE));
      EEPROM.update(FOURTH_HIGHSCORE_CH1, EEPROM.read(THIRD_HIGHSCORE_CH1));
      EEPROM.update(FOURTH_HIGHSCORE_CH2, EEPROM.read(THIRD_HIGHSCORE_CH2));
      EEPROM.update(FOURTH_HIGHSCORE_CH3, EEPROM.read(THIRD_HIGHSCORE_CH3));
      EEPROM.update(FOURTH_MULTI256, EEPROM.read(THIRD_MULTI256));

      EEPROM.update(THIRD_HIGHSCORE, EEPROM.read(SECOND_HIGHSCORE));
      EEPROM.update(THIRD_HIGHSCORE_CH1, EEPROM.read(SECOND_HIGHSCORE_CH1));
      EEPROM.update(THIRD_HIGHSCORE_CH2, EEPROM.read(SECOND_HIGHSCORE_CH2));
      EEPROM.update(THIRD_HIGHSCORE_CH3, EEPROM.read(SECOND_HIGHSCORE_CH3));
      EEPROM.update(THIRD_MULTI256, EEPROM.read(SECOND_MULTI256));

      EEPROM.update(SECOND_HIGHSCORE, score);
      EEPROM.update(SECOND_MULTI256, score / 256);

    } else if (score > EEPROM.read(THIRD_HIGHSCORE) + EEPROM.read(THIRD_MULTI256) * 256) {
      enterNameOK = true;
      hsChanged = 3;

      EEPROM.update(FIFTH_HIGHSCORE, EEPROM.read(FOURTH_HIGHSCORE));
      EEPROM.update(FIFTH_HIGHSCORE_CH1, EEPROM.read(FOURTH_HIGHSCORE_CH1));
      EEPROM.update(FIFTH_HIGHSCORE_CH2, EEPROM.read(FOURTH_HIGHSCORE_CH2));
      EEPROM.update(FIFTH_HIGHSCORE_CH3, EEPROM.read(FOURTH_HIGHSCORE_CH3));
      EEPROM.update(FIFTH_MULTI256, EEPROM.read(FOURTH_MULTI256));

      EEPROM.update(FOURTH_HIGHSCORE, EEPROM.read(THIRD_HIGHSCORE));
      EEPROM.update(FOURTH_HIGHSCORE_CH1, EEPROM.read(THIRD_HIGHSCORE_CH1));
      EEPROM.update(FOURTH_HIGHSCORE_CH2, EEPROM.read(THIRD_HIGHSCORE_CH2));
      EEPROM.update(FOURTH_HIGHSCORE_CH3, EEPROM.read(THIRD_HIGHSCORE_CH3));
      EEPROM.update(FOURTH_MULTI256, EEPROM.read(THIRD_MULTI256));

      EEPROM.update(THIRD_HIGHSCORE, score);
      EEPROM.update(THIRD_MULTI256, score / 256);
    } else if (score > EEPROM.read(FOURTH_HIGHSCORE) + EEPROM.read(FOURTH_MULTI256) * 256) {
      enterNameOK = true;
      hsChanged = 4;

      EEPROM.update(FIFTH_HIGHSCORE, EEPROM.read(FOURTH_HIGHSCORE));
      EEPROM.update(FIFTH_HIGHSCORE_CH1, EEPROM.read(FOURTH_HIGHSCORE_CH1));
      EEPROM.update(FIFTH_HIGHSCORE_CH2, EEPROM.read(FOURTH_HIGHSCORE_CH2));
      EEPROM.update(FIFTH_HIGHSCORE_CH3, EEPROM.read(FOURTH_HIGHSCORE_CH3));
      EEPROM.update(FIFTH_MULTI256, EEPROM.read(FOURTH_MULTI256));

      EEPROM.update(FOURTH_HIGHSCORE, score);
      EEPROM.update(FOURTH_MULTI256, score / 256);
    } else if (score > EEPROM.read(FIFTH_HIGHSCORE) + EEPROM.read(FIFTH_MULTI256) * 256) {
      enterNameOK = true;
      hsChanged = 5;
      EEPROM.update(FIFTH_HIGHSCORE, score);
      EEPROM.update(FIFTH_MULTI256, score / 256);
    }

    timer = 0;
    score = 0;
    lives = 3;
    timerLevel1 = 60;
    timerLevel2 = 30;
    timerLevel3 = 20;
    timerLevel4 = 15;
    scoreTotal = 0;
    playerX = 1;
    playerY = 1;
    for (int i = 0; i < matrixSize; i++)
      for (int j = 0; j < matrixSize; ++j) {
        scoreMatrix[i][j] = 0;
        turnOffPixel(i, j);
      }
    levelOK = false;
    messageOK = true;
    if (enterNameOK) {
      lcd.setCursor(4, 0);
      lcd.print("Congrats!");
      lcd.setCursor(0, 1);
      lcd.print("You are in top 5");
      delay(3000);
      while (enterNameOK) {
        enterName();
        if (digitalRead(pinSW) == switchOff) {
          if (millis() - lastDebounce > minReactionTime && buttonDecider != 0) {
            enterNameOK = false;
            buttonDecider = 0;
            lastDebounce = 0;
          }
          if (enterNameOK) {
            lastDebounce = millis();
            buttonDecider = 1;
          }
        }
      }
      if (hsChanged == 1) {
        EEPROM.update(FIRST_HIGHSCORE_CH1, name[0]);
        EEPROM.update(FIRST_HIGHSCORE_CH2, name[1]);
        EEPROM.update(FIRST_HIGHSCORE_CH3, name[2]);
      } else if (hsChanged == 2) {
        EEPROM.update(SECOND_HIGHSCORE_CH1, name[0]);
        EEPROM.update(SECOND_HIGHSCORE_CH2, name[1]);
        EEPROM.update(SECOND_HIGHSCORE_CH3, name[2]);
      } else if (hsChanged == 3) {
        EEPROM.update(THIRD_HIGHSCORE_CH1, name[0]);
        EEPROM.update(THIRD_HIGHSCORE_CH2, name[1]);
        EEPROM.update(THIRD_HIGHSCORE_CH3, name[2]);
      } else if (hsChanged == 4) {
        EEPROM.update(FOURTH_HIGHSCORE_CH1, name[0]);
        EEPROM.update(FOURTH_HIGHSCORE_CH2, name[1]);
        EEPROM.update(FOURTH_HIGHSCORE_CH3, name[2]);
      } else if (hsChanged == 5) {
        EEPROM.update(FIFTH_HIGHSCORE_CH1, name[0]);
        EEPROM.update(FIFTH_HIGHSCORE_CH2, name[1]);
        EEPROM.update(FIFTH_HIGHSCORE_CH3, name[2]);
      }
    }
  } else if ((timerLevel1 - timer <= 0 && level == 1) || (timerLevel2 - timer <= 0 && level == 2) || (timerLevel3 - timer <= 0 && level == 3) || (timerLevel4 - timer <= 0 && level == 4) || ((playerX == 0 || playerX == 7 || playerY == 0 || playerY == 7) && (level == 3 || level == 4))) {
    scoreTotal += score;
    score = 0;
    for (int i = 0; i < matrixSize; i++)
      for (int j = 0; j < matrixSize; ++j) {
        scoreMatrix[i][j] = 0;
        turnOffPixel(i, j);
      }
    lives--;
    timer = 0;
    levelOK = false;
    playerX = 1;
    playerY = 1;
  } else {
    int nr_on = 0;
    for (int i = 1; i < matrixSize - 1; ++i)
      for (int j = 1; j < matrixSize - 1; ++j)
        if (matrix[i][j] == 1)
          nr_on++;

    if (nr_on == 0) {
      timer = 0;
      if (level == 1) {
        if (timerLevel1 > timerLevel4)
          timerLevel1 -= 5;
        else
          timerLevel1--;
      } else if (level == 2) {
        if (timerLevel2 > timerLevel4)
          timerLevel2 -= 5;
        else
          timerLevel2--;
      } else if (level == 3) {
        if (timerLevel3 > timerLevel4)
          timerLevel3 -= 5;
        else
          timerLevel3--;
      } else if (level == 4) {
        timerLevel4--;
      }
      scoreTotal += score;
      score = 0;
      for (int i = 0; i < matrixSize; i++)
        for (int j = 0; j < matrixSize; ++j) {
          scoreMatrix[i][j] = 0;
          turnOffPixel(i, j);
        }
      levelOK = false;
      playerX = 1;
      playerY = 1;
    }
  }
}

void putInitialHStoEEPROM() {

  EEPROM.update(FIRST_HIGHSCORE, 0);
  EEPROM.update(FIRST_HIGHSCORE_CH1, 'T');
  EEPROM.update(FIRST_HIGHSCORE_CH2, 'B');
  EEPROM.update(FIRST_HIGHSCORE_CH3, 'D');
  EEPROM.update(FIRST_MULTI256, 0);

  EEPROM.update(SECOND_HIGHSCORE, 0);
  EEPROM.update(SECOND_HIGHSCORE_CH1, 'T');
  EEPROM.update(SECOND_HIGHSCORE_CH2, 'B');
  EEPROM.update(SECOND_HIGHSCORE_CH3, 'D');
  EEPROM.update(SECOND_MULTI256, 0);

  EEPROM.update(THIRD_HIGHSCORE, 0);
  EEPROM.update(THIRD_HIGHSCORE_CH1, 'T');
  EEPROM.update(THIRD_HIGHSCORE_CH2, 'B');
  EEPROM.update(THIRD_HIGHSCORE_CH3, 'D');
  EEPROM.update(THIRD_MULTI256, 0);

  EEPROM.update(FOURTH_HIGHSCORE, 0);
  EEPROM.update(FOURTH_HIGHSCORE_CH1, 'T');
  EEPROM.update(FOURTH_HIGHSCORE_CH2, 'B');
  EEPROM.update(FOURTH_HIGHSCORE_CH3, 'D');
  EEPROM.update(FOURTH_MULTI256, 0);

  EEPROM.update(FIFTH_HIGHSCORE, 0);
  EEPROM.update(FIFTH_HIGHSCORE_CH1, 'T');
  EEPROM.update(FIFTH_HIGHSCORE_CH2, 'B');
  EEPROM.update(FIFTH_HIGHSCORE_CH3, 'D');
  EEPROM.update(FIFTH_MULTI256, 0);
}

void updateStringToEEPROM(int address, const String &strToWrite) {
  int len = strToWrite.length();
  //erasing the value stored at the address before
  for (int i = address; i < address + len; ++i) {
    EEPROM.update(i, 0);
  }
  //updating in EEPROM
  EEPROM.update(address, len);
  for (int i = 0; i < len; i++) {
    EEPROM.update(address + 1 + i, strToWrite[i]);
  }
}

String readStringFromEEPROM(int address) {
  //reading the length of the message from EEPROM
  int newStrLen = EEPROM.read(address);
  char data[newStrLen];
  //reading the string from EEPROM
  for (int i = 0; i < newStrLen; i++) {
    data[i] = EEPROM.read(address + 1 + i);
  }
  return String(data);
}

void setup() {
  lc.shutdown(0, false);
  lcd.begin(16, 2);
  lcd.createChar(0, down);
  lcd.createChar(1, up);
  lcd.createChar(2, full);
  lcd.createChar(3, heart);
  lcd.createChar(4, hungryFace);
  lcd.createChar(5, upDown);
  analogWrite(3, 40);
  pinMode(pinSW, INPUT_PULLUP);
  pinMode(pinX, INPUT);
  pinMode(pinY, INPUT);
  pinMode(pinA, OUTPUT);
  pinMode(pinV0, OUTPUT);
  lcdBrightness = readStringFromEEPROM(LCD_BRIGHTNESS_ADDRESS).toInt();
  lcdContrast = readStringFromEEPROM(LCD_CONTRAST_ADDRESS).toInt();
  matrixBrightness = readStringFromEEPROM(MTX_BRIGHTNESS_ADDRESS).toInt();
  sound = EEPROM.read(BUZZER_SOUND_ADDRESS);
  analogWrite(pinA, (maxContrastBrightness / lcdLength) * lcdBrightness);
  analogWrite(pinV0, (maxContrastBrightness / lcdLength) * lcdContrast);
  lc.setIntensity(0, matrixBrightness);
  //setSound();
  //putInitialHStoEEPROM();
  Serial.begin(9600);
}

void loop() {

  if (introOK) {
    lcd.setCursor(0, 0);
    lcd.print("   Welcome to   ");
    lcd.setCursor(0, 1);
    lcd.print(" Pixel Eater ");
    lcd.write(byte(4));
    lcd.print("* ");
    delay(3000);
    lcd.clear();
    introOK = false;
  }
  if (gameMenuSelected == true)
    menuScroller();

  if (gameMenuSelected == false) {

    if (menuPosition % numberOfMenuOptions == 0)
      restartHS();
    else if (menuPosition % numberOfMenuOptions == 1) {
      startGame();
    } else if (menuPosition % numberOfMenuOptions == 2)
      showHighscore();

    else if (menuPosition % numberOfMenuOptions == 3) {
      if (settingsMenuSelected == true)
        settingsScroller();
      else {
        if (menuSettingsPosition % numberOfMenuSettingsOptions == 0)
          setLevel();
        else if (menuSettingsPosition % numberOfMenuSettingsOptions == 1)
          setContrast();
        else if (menuSettingsPosition % numberOfMenuSettingsOptions == 2)
          setLcdBrightness();
        else if (menuSettingsPosition % numberOfMenuSettingsOptions == 3)
          setMatrixBrightness();
        else if (menuSettingsPosition % numberOfMenuSettingsOptions == 4)
          setSound();
      }
    } else if (menuPosition % numberOfMenuOptions == 4)
      displayAbout();
    else if (menuPosition % numberOfMenuOptions == 5)
      displayHowToPlay();
  }
}
