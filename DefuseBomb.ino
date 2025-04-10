#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x3F, 16, 2); // endereço I2C, colunas, linhas

const int buzzerPin = 15;
const int buttonPin = 14;

unsigned long countdownStart = 0;
bool countdownDone = false;
bool gameStarted = false;
unsigned long gameStartMillis = 0;
unsigned long lastBeepMillis = 0;

void setup() {
  Serial.begin(115200);

  setupBuzzer();
  setupButtons();
  setupLCD();

  screenFinishBoot();

  delay(1500);
  showLCDMessage("Arming in:", 0, 0);
  showLCDMessage("5 seconds", 0, 1);

  countdownStart = millis();
}

void loop() {
  if (!countdownDone) {
    unsigned long elapsed = (millis() - countdownStart) / 1000;
    int remaining = 5 - elapsed;

    if (remaining >= 0) {
      showLCDMessage("Arming in:", 4, 0);
      showLCDMessage(String(remaining) + " seconds", 3, 1);
    }

    if (remaining <= 0) {
      countdownDone = true;
      lcd.clear();
      showLCDMessage("BOMBA ATIVA!", 0, 0);

      // Beep 3s
      digitalWrite(buzzerPin, HIGH);
      delay(3000);
      digitalWrite(buzzerPin, LOW);

      gameStartMillis = millis();
      gameStarted = true;
    }
  }

  if (gameStarted) {
    updateGameCountdown();
  }

  delay(200);
}


/**
##################################################
##                   SETUP                      ##
##################################################
*/

void setupBuzzer(){
  pinMode(buzzerPin, OUTPUT);
  pinMode(buttonPin, INPUT);
}

void setupLCD(){
  lcd.init();
  lcd.backlight();
}

void setupButtons(){
  pinMode(buttonPin, INPUT);
}


/**
##################################################
##                   UTILS                      ##
##################################################
*/

void showLCDMessage(String message, byte posX, byte posY) {
  lcd.setCursor(posX, posY);
  lcd.print("                ");
  if (message.length() > 0) {
    lcd.setCursor(posX, posY);
    lcd.print(message);
  }
}

void beepBuzzer(int duration) {
  digitalWrite(buzzerPin, HIGH);
  delay(duration);
  digitalWrite(buzzerPin, LOW);
}

void updateGameCountdown() {
  unsigned long elapsed = (millis() - gameStartMillis) / 1000;
  int remaining = 30 * 60 - elapsed;

  if (remaining >= 0) {
    int minutes = remaining / 60;
    int seconds = remaining % 60;

    char timeBuffer[9];
    sprintf(timeBuffer, "%02d:%02d", minutes, seconds);
    showLCDMessage("Tempo restante:", 0, 0);
    showLCDMessage(String(timeBuffer), 4, 1);

    unsigned long now = millis();

    // Controlar beeps progressivos
    int interval = 60000; // 1 min default
    if (remaining <= 60 && remaining > 30) interval = 15000;
    else if (remaining <= 30 && remaining > 5) interval = 5000;
    else if (remaining <= 5) interval = 1000;

    if (now - lastBeepMillis >= interval) {
      lastBeepMillis = now;
      beepBuzzer(2000);
    }
  } else {
    gameStarted = false;
    lcd.clear();
    showLCDMessage("BOMB EXPLODED", 1, 0);
    digitalWrite(buzzerPin, HIGH); // Beep contínuo
  }
}


/**
##################################################
##               MENSSAGES                      ##
##################################################
*/

void screenFinishBoot() {
  lcd.clear();
  lcd.noBacklight();
  delay(300);
  lcd.backlight();

  showLCDMessage("CLUBE AIRSOFT", 1, 0);
  delay(500);
  showLCDMessage("MERTOLA", 4, 1);
  delay(3000);
}
