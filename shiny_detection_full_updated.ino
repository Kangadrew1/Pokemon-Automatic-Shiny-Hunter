
#include <EEPROM.h>
#include <LiquidCrystal.h>
#include <ESP32Servo.h>

// Servos for pressing buttons
Servo servo1; // A button - GPIO 2
Servo servo2; // Start button - GPIO 4
Servo servo3; // Up button - GPIO 3

// LCD setup (if desired, still included)
LiquidCrystal lcd(7, 8, 9, 10, 11, 12);

// Pin constants
const int LIGHT_SENSOR_PIN = 34;     // Analog pin for LDR
const int BUTTON_A_PIN = 6;          // Start automation
const int BUTTON_B_PIN = 5;          // Save counter, enter menu

// Detection thresholds
const int STABLE_LOWER = 1000;
const int STABLE_UPPER = 1040;
const int STABLE_COUNT_REQUIRED = 60;  // 3 seconds @ 50ms each

// Counters and state
int totalCounter = 0;
int dailyCounter = 0;
int failCounter = 0;
int failPercentage = 0;
int lightValue = 0;
bool isRunning = false;
bool isShiny = false;

// Function declarations
void pressButtonA(int angle);
void pressButtonSTART(int angle);
void pressButtonUP(int angle);
void updateLCD();
void softReset();
void adjustThreshold();

void setup() {
  Serial.begin(115200);
  lcd.begin(16, 2);

  pinMode(BUTTON_A_PIN, INPUT_PULLUP);
  pinMode(BUTTON_B_PIN, INPUT_PULLUP);

  servo1.attach(2);
  servo2.attach(4);
  servo3.attach(3);

  EEPROM.begin(512);
  EEPROM.get(0, totalCounter);

  lcd.setCursor(0, 0);
  lcd.print("Ready to start");
}

void loop() {
  servo1.write(0);
  servo2.write(0);
  servo3.write(35);

  int stableCounter = 0;

  if (digitalRead(BUTTON_A_PIN) == LOW || isRunning) {
    isRunning = true;
    updateLCD();

    while (!isShiny) {
      // Press encounter buttons
      pressButtonA(23); delay(1300);
      pressButtonA(23); delay(2900);
      pressButtonA(23); delay(2800);
      pressButtonUP(35); delay(2500);
      pressButtonA(23); delay(4800);

      // Light detection during sparkle animation
      for (int i = 0; i < 100; i++) {
        lightValue = analogRead(LIGHT_SENSOR_PIN);
        Serial.print("LDR="); Serial.print(lightValue);
        Serial.print("  Count="); Serial.println(stableCounter);

        if (lightValue >= STABLE_LOWER && lightValue <= STABLE_UPPER) {
          stableCounter++;
        } else {
          stableCounter = 0;
        }

        if (stableCounter >= STABLE_COUNT_REQUIRED) {
          isShiny = true;
          break;
        }

        delay(50);
      }

      if (isShiny) {
        servo1.write(0);
        servo2.write(0);
        servo3.write(0);
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("*SHINY FOUND*");
        lcd.setCursor(0, 1);
        lcd.print("Pause to catch!");
        while (true);
      }

      // Not shiny — reset and count
      totalCounter++;
      dailyCounter++;
      failPercentage = (failCounter * 100) / dailyCounter;
      updateLCD();
      softReset();
    }

    // Save counter if button B is pressed
    if (digitalRead(BUTTON_B_PIN) == LOW) {
      EEPROM.put(0, totalCounter);
      EEPROM.commit();
      isRunning = false;
      adjustThreshold();
    }
  }
}

void pressButtonA(int angle) {
  for (int pos = 0; pos <= angle; pos++) {
    servo1.write(pos);
    delay(15);
  }
  for (int pos = angle; pos >= 0; pos--) {
    servo1.write(pos);
    delay(15);
  }
}

void pressButtonSTART(int angle) {
  for (int pos = 0; pos <= angle; pos++) {
    servo2.write(pos);
    delay(15);
  }
  for (int pos = angle; pos >= 0; pos--) {
    servo2.write(pos);
    delay(15);
  }
}

void pressButtonUP(int angle) {
  for (int pos = angle; pos >= 0; pos--) {
    servo3.write(pos);
    delay(15);
  }
  for (int pos = 0; pos <= angle; pos++) {
    servo3.write(pos);
    delay(15);
  }
}

void softReset() {
  pressButtonSTART(33);
  delay(8700);
}

void updateLCD() {
  lcd.setCursor(0, 0);
  lcd.print("SRs=");
  lcd.setCursor(4, 0);
  lcd.print(totalCounter);
  lcd.setCursor(10, 1);
  lcd.print("F=");
  lcd.setCursor(12, 1);
  lcd.print(failPercentage);
  lcd.print("%");
}

void adjustThreshold() {
  lcd.clear();
  lcd.setCursor(0, 1);
  lcd.print("Today's SRs: ");
  lcd.setCursor(12, 1);
  lcd.print(dailyCounter);

  for (int s = 0; s < 100000; s++) {
    if (digitalRead(BUTTON_A_PIN) == LOW) {
      // Threshold adjustment logic placeholder
    }
    if (digitalRead(BUTTON_B_PIN) == LOW && digitalRead(BUTTON_A_PIN) == LOW) {
      break;
    }
    delay(500);
  }
}
