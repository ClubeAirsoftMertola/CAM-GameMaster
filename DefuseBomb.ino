#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x3F, 16, 2); // Endereço I2C do LCD

const int buzzerPin = 15;
const int buttonPin = 14;

unsigned long countdownStart = 0;
bool countdownDone = false;
bool gameStarted = false;
unsigned long gameStartMillis = 0;

// Botão
unsigned long buttonPressedMillis = 0;
bool buttonHeld = false;
bool bombDefused = false;

// Evitar flicker
String lastTimeDisplayed = "";
int lastRemainingArming = -1;

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

    if (remaining != lastRemainingArming && remaining >= 0) {
      lastRemainingArming = remaining;
      showLCDMessage("Arming in:", 0, 0);
      showLCDMessage(String(remaining) + " seconds", 0, 1);
    }

    if (remaining <= 0) {
      countdownDone = true;
      lcd.clear();
      showLCDMessage("BOMBA ATIVA!", 0, 0);

      // Beep de 3 segundos
      digitalWrite(buzzerPin, HIGH);
      delay(3000);
      digitalWrite(buzzerPin, LOW);

      gameStartMillis = millis();
      gameStarted = true;

      lcd.setCursor(0, 0);
      lcd.print("Tempo restante:");
    }
  }

  if (gameStarted && !bombDefused) {
    updateGameCountdown();
    checkDisarmButton();
  }
}

/**
##################################################
##                   SETUP                      ##
##################################################
*/

void setupBuzzer(){
  digitalWrite(buzzerPin, LOW);
  pinMode(buzzerPin, OUTPUT);
}

void setupLCD(){
  lcd.init();
  lcd.backlight();
}

void setupButtons(){
  pinMode(buttonPin, INPUT); // botão vai a GND -> valor LOW
}


/**
##################################################
##                   UTILS                      ##
##################################################
*/

void showLCDMessage(String message, byte posX, byte posY) {
  lcd.setCursor(posX, posY);
  lcd.print("                "); // limpa linha
  if (message.length() > 0) {
    lcd.setCursor(posX, posY);
    lcd.print(message);
  }
}

void showLCDTime(String timeStr) {
  if (timeStr != lastTimeDisplayed) {
    lcd.setCursor(4, 1);
    lcd.print("        "); // limpa o tempo
    lcd.setCursor(4, 1);
    lcd.print(timeStr);
    lastTimeDisplayed = timeStr;
  }
}

void beepBuzzer(int duration) {
  digitalWrite(buzzerPin, HIGH);
  delay(duration);
  digitalWrite(buzzerPin, LOW);
}

void blinkLCD() {
  static bool state = false;
  state = !state;
  if (state) lcd.noBacklight();
  else lcd.backlight();
}

void updateGameCountdown() {
  unsigned long elapsed = (millis() - gameStartMillis) / 1000;
  int remaining = 20 * 60 - elapsed; // 20 minutos

  if (remaining >= 0) {
    int minutes = remaining / 60;
    int seconds = remaining % 60;

    char timeBuffer[9];
    sprintf(timeBuffer, "%02d:%02d", minutes, seconds);
    showLCDTime(String(timeBuffer));
  } else {
    gameStarted = false;
    lcd.clear();
    showLCDMessage("BOMB EXPLODED", 1, 0);
    digitalWrite(buzzerPin, HIGH); // Beep contínuo

    while (true) {
      blinkLCD();
      delay(300);
    }
  }
}

void checkDisarmButton() {
  int buttonState = digitalRead(buttonPin);

  if (buttonState == HIGH) {
    if (!buttonHeld) {
      buttonPressedMillis = millis();
      buttonHeld = true;
    } else {
      unsigned long heldTime = millis() - buttonPressedMillis;

      // Barra de progresso
      int totalBlocks = 16;
      int filledBlocks = map(heldTime, 0, 3000, 0, totalBlocks);

      lcd.setCursor(0, 1);
      for (int i = 0; i < totalBlocks; i++) {
        if (i < filledBlocks) lcd.write(255);
        else lcd.print(" ");
      }

      if (heldTime >= 3000) {
        bombDefused = true;
        gameStarted = false;
        lcd.clear();
        showLCDMessage("BOMBA DESARMADA", 0, 0);
        showLCDMessage("CT WIN", 5, 1);
        beepBuzzer(500);
        delay(500);
        beepBuzzer(500);
      }
    }
  } else {
    if (buttonHeld) {
      lcd.setCursor(0, 1);
      lcd.print("                ");
    }
    buttonHeld = false;
  }
}


/**
##################################################
##               MENSAGENS                      ##
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
