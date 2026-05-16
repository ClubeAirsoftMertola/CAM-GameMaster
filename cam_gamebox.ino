#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h>


/*
  PIN         CONNECT TO                 PIN           CONNECTED TO
-----------------------------------|--------------------------------
  EN                               |     D23             RFID_MOSI
  VP                               |     D22             OLED_SCL
  VIN ---------------------------  |     TX0
  D34                              |     RX0
  D35                              |     D21             OLED_SDA
  D32    RFID_MISO                 |     D19
  D33    RFID_RESET                |     D18             BTN_BLUE
  D25    LED_R                     |     D5              BTN_RED
  D26    LED_G                     |     TX2
  D27    LED_B                     |     RX2
  D14    RFID_SCK                  |     D4              RFID_SDA
  D12                              |     D2
  D13    BUZZER                    |     D15
  
  VIN    LiPO Battery Power Reducer
  3.3V   OLED VDD, RFID
*/



// =========================
// BUTTONS
// =========================
#define BTN_RED 5
#define BTN_BLUE 18


// =========================
// RGB LED    (+ Ω220 resistor)
// =========================
#define LED_R 25
#define LED_G 26
#define LED_B 27

// =========================
// OLED SH110X
// =========================
#define OLED_SCL 22
#define OLED_SDA 21
#define OLED_SCREEN_WIDTH 128
#define OLED_SCREEN_HEIGHT 64

Adafruit_SH1106G display = Adafruit_SH1106G(OLED_SCREEN_WIDTH, OLED_SCREEN_HEIGHT, &Wire, -1);




void setup() {

  Serial.begin(115200);

  setupLed();
  setupButtons();
  setupOLEDScreen();
  
}

void loop() {
  //
}





/*
###################################
##        SETUP HARDWARE         ##
###################################
*/

void setupOLEDScreen(){
  Wire.begin(OLED_SDA, OLED_SCL);

  if(!display.begin(0x3C, true)) {

    Serial.println("OLED FAIL");
    setColor(true, false, false);

    while(1);
  }

  

  // START SCREEN
  /*display.clearDisplay();

  display.setTextSize(1);
  display.setTextColor(SH110X_WHITE);
  display.setCursor(20, 12);
  display.println("CLUBE  AIRSOFT");

  display.setTextSize(2);
  display.setCursor(20, 28);
  display.setTextColor(SH110X_WHITE);
  display.println("MERTOLA");
  
  display.display();*/
  display.clearDisplay();
  showMessage(1, 20, 12, "CLUBE  AIRSOFT", true);
  showMessage(2, 20, 28, "MERTOLA", true);

}

void setupLed(){
  pinMode(LED_R, OUTPUT);
  pinMode(LED_G, OUTPUT);
  pinMode(LED_B, OUTPUT); 

  setColor(true, false, false);  // red
  delay(500);
  setColor(false, true, false);  // green
  delay(500);
  setColor(false, false, true);  // blue
  delay(500);

  digitalWrite(LED_R, LOW);
  digitalWrite(LED_G, LOW);
  digitalWrite(LED_B, LOW);
}

void setupButtons(){
  pinMode(BTN_BLUE, INPUT_PULLUP);
  pinMode(BTN_RED, INPUT_PULLUP);
}




/*
###################################
##            UTILS              ##
###################################
*/

void showMessage(int textSize, int line, int col, String text, bool setTextColor){

  display.setTextSize(textSize);
  display.setCursor(line, col);
  if (setTextColor == true) display.setTextColor(SH110X_WHITE);
  display.println(text);

  display.display();
}

void setColor(bool red, bool green, bool blue){
  if(red == true) { digitalWrite(LED_R, HIGH); }else{ digitalWrite(LED_R, LOW); }
  if(green == true) { digitalWrite(LED_G, HIGH); }else{ digitalWrite(LED_G, LOW); }
  if(blue == true) { digitalWrite(LED_B, HIGH); }else{ digitalWrite(LED_B, LOW); }
}





