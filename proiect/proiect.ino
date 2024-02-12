#include <dht11.h>
#include <LiquidCrystal_I2C.h> 

//initialize the two objects: the sensor and the lcd display
#define DHT11PIN 4
dht11 DHT11;
LiquidCrystal_I2C lcd(0x27, 16, 2); // I2C address 0x27, 16 column and 2 row

String message = "";
String esp_temp = "23";
String esp_city = "1";
String esp_cond = "Clouds";

int toggle;
int start;
int current;

void setup()
{
  //initialize Serial (will receive messages from the ESP through it)
  Serial.begin(115200);
  start = millis();
  current = start;

  //initialize the lcd
  lcd.init();
  lcd.backlight();
  lcd.clear();  

  //led indicating high humidity
  pinMode(13, OUTPUT);
}

void serialEvent() {
  //citire formatata
  message = "";
  esp_temp = "";
  esp_city = "";
  esp_cond = "";
  int i = 0;

  while(Serial.available()) {
    char c = (char)Serial.read();

    if (c == '\n' || c == '\r' || c == '\"') {
      continue;
    }
    else {
      if (c == '-') {
        i--;
      }
      if (c == '.') {
        i = 0;
      }
      if (i < 3) {
        esp_temp += c;
      }
      else if (i < 4) {
        esp_city += c;
      }
      else {
        esp_cond += c;
      }
      i++;
    }
  }

  Serial.println(esp_temp);
  Serial.println(esp_city);
  Serial.println(esp_cond);
}

void loop()
{           
  //read and display on the lcd the sensor data for 5 seconds
  int chk = DHT11.read(DHT11PIN);
  current = millis() - start;

  if (toggle == 0) {
    lcd.setCursor(0, 0);
    lcd.print("H (%, in): ");
    lcd.print((float)DHT11.humidity, 2);
  
    lcd.setCursor(0, 1);
    lcd.print("T (C, in): ");
    lcd.print((float)DHT11.temperature, 2);
  }
  else {
    //display on the lcd the ESP data for 5 seconds
    lcd.setCursor(0, 0);
    //hardcoded city names
    if (esp_city == "1") {
      lcd.print("Cluj-Napoca: ");
    }
    else if (esp_city == "2") {
      lcd.print("Bucuresti:");
    }
    else {
      lcd.print("Iasi:");
    }
  
    lcd.setCursor(0, 1);
    lcd.print(esp_temp);
    lcd.print((char)223);
    lcd.print("C, ");
    lcd.print(esp_cond);
  }

  //5 seconds passed
  if (current > 5000) {
    start = millis();
    toggle = 1 - toggle;
    lcd.clear();
  }

  if (esp_cond.endsWith("Rain") || esp_cond.endsWith("Snow") || (float)DHT11.humidity > 75.00) {
    digitalWrite(13, HIGH);
  }
  else {
    digitalWrite(13, LOW);
  }

}
