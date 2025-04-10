#include <Wire.h>
#include <LiquidCrystal_I2C.h>



LiquidCrystal_I2C lcd(0x3F, 16, 2); // endereço I2C, colunas, linhas


const int buzzerPin = 15;
const int buttonPin = 14;  // botão no GPIO14


unsigned long countdownStart = 0;
bool countdownDone = false;


void setup() {
  Serial.begin(115200);

  setupBuzzer();
  setupButtons();
  setupLCD();

  //Serial.println("Início do loop...");
  screenFinishBoot();

  delay(1500); //pause
  showLCDMessage("Arming in:", 0, 0);
  showLCDMessage("30 seconcs", 0, 1);

  countdownStart = millis(); // começa a contagem
}

void loop() {
  if (!countdownDone) {
    unsigned long elapsed = (millis() - countdownStart) / 1000;
    int remaining = 30 - elapsed;

    if (remaining >= 0) {
      // Mostrar apenas o número no segundo linha
      showLCDMessage("Arming in:", 4, 0);
      showLCDMessage(String(remaining) + " seconds", 3, 1);
    }

    if (remaining <= 0) {
      countdownDone = true;
      lcd.clear();
      showLCDMessage("BOMBA ATIVA!", 0, 0);
      showLCDMessage("30 minutos", 0, 1);

      // Beep de 3 segundos
      digitalWrite(buzzerPin, HIGH);
      delay(3000);
      digitalWrite(buzzerPin, LOW);

      // Proxima fase virá aqui
    }
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

void showLCDMessage(String message, byte posX, byte posY) {
  lcd.setCursor(posX, posY);
  lcd.print("                "); // limpa linha (16 espaços)

  if (message.length() > 0) {
    lcd.setCursor(posX, posY);
    lcd.print(message);
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

