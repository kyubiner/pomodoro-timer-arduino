#include <LiquidCrystal_I2C.h>
#include <SoftwareSerial.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);

unsigned long lastUpdate = 0;
int workTime = 25 * 60;
int breakTime = 5 * 60;
int timer = workTime;
bool isRunning = false;
bool isWork = true;

const int ledRed = 7;
const int ledYellow = 6;
const int ledGreen = 5;
const int buzzer = 9;
const int buttonStart = 4;
const int buttonStop = 3;
int i = 0;

void setup() {
  pinMode(ledRed, OUTPUT);
  pinMode(ledYellow, OUTPUT);
  pinMode(ledGreen, OUTPUT);
  pinMode(buzzer, OUTPUT);
  pinMode(buttonStart, INPUT);
  pinMode(buttonStop, INPUT);
  lcd.init();
  lcd.backlight();
  lcd.clear();
  digitalWrite(ledRed, LOW);
  digitalWrite(ledYellow, LOW);
  digitalWrite(ledGreen, LOW);
  lcd.setCursor(1, 0);
  lcd.print("Pomodoro Timer");
  lcd.setCursor(1, 1);
  lcd.print("Press to Start");
}

void loop() {
  if(digitalRead(buttonStart) == HIGH && !isRunning) {
    lcd.clear();
    isRunning = true;
    lastUpdate = millis();
    timer = (isWork ? workTime : breakTime);
    digitalWrite((isWork ? ledGreen : ledRed), HIGH);
    digitalWrite(ledYellow, LOW);
  }

  if(digitalRead(buttonStop) == HIGH) {
    digitalWrite(ledGreen, LOW);
    digitalWrite(ledRed, LOW);
    lcd.clear();
    lcd.setCursor(5, 0);
    lcd.print("RESET...");
    digitalWrite(ledYellow, HIGH);
    delay(1000);
    isRunning = false;
    digitalWrite(ledYellow, LOW);
    lcd.clear();
    lcd.setCursor(1, 0);
    lcd.print("Pomodoro Timer");
    lcd.setCursor(1, 1);
    lcd.print("Press to Start");
  }

  if(isRunning) {
    unsigned long currentMillis = millis();
    if(currentMillis - lastUpdate >= 1000) {
      lastUpdate = currentMillis;
      timer--;

      int minutes = timer / 60;
      int seconds = timer % 60;

      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print((isWork ? "> Study Time" : "> Break Time"));
      lcd.setCursor(0, 1);
      lcd.print("Timer : ");
      lcd.print(minutes);
      lcd.print(":");
      if(seconds < 10) lcd.print("0");
      lcd.print(seconds);

      if(timer <= 0) {
        digitalWrite(ledGreen, LOW);
        digitalWrite(ledRed, LOW);
        digitalWrite(buzzer, HIGH);
        digitalWrite(ledYellow, HIGH);
        isRunning = false;
        delay(1000);
        isWork = !isWork;
        digitalWrite(buzzer, LOW);
        lcd.clear();
        while(i <= 10) {
          lcd.setCursor(0, 0);
          lcd.print("press to lanjut");
          lcd.setCursor(0, 1);
          lcd.print("> continue.");
          delay(500);
          lcd.print(".");
          delay(500);
          lcd.print(".");
          delay(500);
          lcd.clear();
          i++;
        }
        lcd.setCursor(0, 0);
        lcd.print("press to lanjut");
        lcd.setCursor(0, 1);
        lcd.print("> continue...");
      }
    }
  }
}
