#include <Wire.h>
#include <LiquidCrystal_I2C.h>



LiquidCrystal_I2C lcd(0x3F, 16, 2); // endereço I2C, colunas, linhas


const int buzzerPin = 15;
const int buttonPin = 14;  // botão no GPIO14




void setup() {
  setupBuzzer();
  setupButtons();
  setupLCD();

  Serial.begin(115200);
  Serial.println("Início do loop...");

  screenFinishBoot();
}

void loop() {
  Serial.println("Beep!");

  digitalWrite(buzzerPin, HIGH);
  delay(1000);

  digitalWrite(buzzerPin, LOW);

  if (digitalRead(buttonPin) == HIGH) {
    Serial.println("Botão pressionado!");
  } else {
    Serial.println("Botão solto");
  }
  delay(500);
}




/**
##################################################
##                   SETUP                      ##
##################################################
*/

void setupBuzzer(){
  pinMode(buzzerPin, OUTPUT);
}


void setupLCD(){
  lcd.init();          // Inicializa o LCD
  lcd.backlight();     // Liga a luz de fundo
}


void setupButtons(){
  pinMode(buttonPin, INPUT);
}







/**
##################################################
##                   UTILS                      ##
##################################################
*/

void showLCDMessage(String message, byte posX, byte posY){
  lcd.setCursor(posX, posY); // coluna 0, linha 0
  lcd.print("                ");  // limpa linha (16 espaços)
  lcd.setCursor(posX, posY); // coluna 0, linha 0
  lcd.print(message);
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

