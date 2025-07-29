// POKEMON NAME + TYPE Robust Sketch (with API)
// Engineer: Andrew Nguyen

#include <LiquidCrystal.h>

// GLOBAL DO NOT CHANGE THESE PINS UNLESS CHANGED THE PHYSICAL ONES!
// initialize the library with the numbers of the interface pins I have set up on physical breadboard
LiquidCrystal lcd(4, 6, 10, 11, 12, 13);

String incoming = "";
bool ready = false;

void setup() {
  /*
  Serial.begin(9600);

  lcd.setCursor(0, 0);
  lcd.print("Ready for");
  lcd.setCursor(0, 1);
  lcd.print("Pokemon Data!");

  delay(5000); // delay 5 seconds before clearing
  lcd.clear();
  lcd.print("Waiting...");
  */

  lcd.begin(16, 2);
  Serial.begin(9600);
  lcd.print("Waiting for");
  lcd.setCursor(0,1);
  lcd.print("Pokémon...");
}

void loop() {
  // Read serial data
  while (Serial.available() > 0) {
    char c = Serial.read();

    if (c == '\n') {
      ready = true;
      break;
    } else {
      incoming += c;
    }
  }

if (ready) {
  incoming.trim();  // remove leading/trailing \r, \n, spaces
  Serial.print("Received: '"); Serial.print(incoming); Serial.println("'");

  // 1) Show raw message for 3 s (sanity check section, comment out when not in debug mode)
  // lcd.clear();
  // lcd.setCursor(0,0);
  // lcd.print(incoming.substring(0, 16));        // first 16 chars
  // lcd.setCursor(0,1);
  // lcd.print(incoming.substring(16, 32));       // next 16 chars
  // delay(5000); // show for 5 seconds on LCD

  // 2) Now try to parse
  int sep = incoming.indexOf('|');
  lcd.clear(); // clear screen completely
  if (sep != -1) {
    String name = incoming.substring(0, sep);
    String type = incoming.substring(sep + 1);

          // Line 0: "Name: "+name, padded to 16 chars
      lcd.setCursor(0, 0);
      String line0 = "Name: " + name;
      while (line0.length() < 16) line0 += ' ';
      lcd.print(line0);

      // Line 1: "Type: "+type, padded to 16 chars
      lcd.setCursor(0, 1);
      String line1 = "Type: " + type;
      while (line1.length() < 16) line1 += ' ';
      lcd.print(line1);
    } else {
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Invalid data");
    }
    incoming = "";
    ready = false;
  }

// OLD LOGIC
  // if (ready) {
  //   Serial.print("Received: ");
  //   Serial.println(incoming);

  //   int sep = incoming.indexOf('|');

  //   if (sep != -1) {
  //     String name = incoming.substring(0, sep);
  //     String type = incoming.substring(sep + 1);

  //     lcd.clear();
  //     lcd.setCursor(0, 0);
  //     lcd.print("Name: " + name);
  //     lcd.setCursor(0, 1);
  //     lcd.print("Type: " + type);
  //   } else {
  //     lcd.clear();
  //     lcd.setCursor(0, 0);
  //     lcd.print("Invalid data =(");
  //   }

  //   incoming = "";
  //   ready = false;
  // }
}