// arduino cloud
#include "thingProperties.h"
// Motor pin
#define PIN_IN1  5
#define PIN_IN2 4
#define PIN_ENA  19
//DHT SENSOR
#include "DHT.h"
#define DHT11Pin 15
#define DHTType DHT11
//OLED
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
//DHT11 setup
DHT HT(DHT11Pin,DHTType);
float humi;
float tempC;
float tempF;

//OLED define
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);
void setup() {
  // put your setup code here, to run once:
    Serial.begin(115200);
  // pin setup
  pinMode(PIN_IN1, OUTPUT);
  pinMode(PIN_IN2, OUTPUT);
  pinMode(PIN_ENA, OUTPUT);
  // pin setup
  HT.begin();
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) // Address 0x3C for 128x64
  { 
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  }
  delay(1000); 
  display.clearDisplay();
  // Defined in thingProperties.h
  initProperties();
  // Connect to Arduino IoT Cloud
  ArduinoCloud.begin(ArduinoIoTPreferredConnection);
  /*
     The following function allows you to obtain more information
     related to the state of network and IoT Cloud connection and errors
     the higher number the more granular information you’ll get.
     The default is 0 (only errors).
     Maximum is 4
  */
  setDebugMessageLevel(2);
  ArduinoCloud.printDebugInfo();
}

// void loop start

void loop() {
  // put your main code here, to run repeatedly:
    ArduinoCloud.update();
  // Your code here
  onHumidityChange();
  delay(1000);

  delay(1000);
 humi  = HT.readHumidity();
 tempC = HT.readTemperature();
 tempF = HT.readTemperature(true);

 Serial.print("Humidity:");
 Serial.print(humi,0);
 Serial.print("%");
 Serial.print(" Temperature:");
 Serial.print(tempC,1);
 Serial.print("C ~ ");
 Serial.print(tempF,1);
 Serial.println("F");

 display.clearDisplay();
 oledDisplayHeader();
 

 oledDisplay(3,5,28,humi,"%");
 oledDisplay(2,70,16,tempC,"C");
 oledDisplay(2,70,44,tempF,"F");
 
 display.display(); 

 // display in void loop part ends

 // motor starts
 if (tempC > 28 && tempC < 32) {
  digitalWrite(PIN_IN1, HIGH); // control the motor's direction in clockwise
  digitalWrite(PIN_IN2, LOW);  // control the motor's direction in clockwise

    analogWrite(PIN_ENA, 85); // speed up
    delay(10);
 }
 else if (tempC > 32 && tempC < 36) {
  digitalWrite(PIN_IN1, HIGH); // control the motor's direction in clockwise
  digitalWrite(PIN_IN2, LOW);  // control the motor's direction in clockwise

    analogWrite(PIN_ENA, 170); // speed up
    delay(10);
}
   else if (tempC > 36 && tempC < 40) {
  digitalWrite(PIN_IN1, HIGH); // control the motor's direction in clockwise
  digitalWrite(PIN_IN2, LOW);  // control the motor's direction in clockwise

    analogWrite(PIN_ENA, 255); // speed up
    delay(10);
}
 else if (tempC > 40 && tempC < 45) {
  digitalWrite(PIN_IN1, LOW); // control the motor's direction in clockwise
  digitalWrite(PIN_IN2, HIGH);  // control the motor's direction in clockwise

    analogWrite(PIN_ENA, 192); // speed up
    delay(10);
}
 else if (tempC > 45) {
  digitalWrite(PIN_IN1, LOW); // control the motor's direction in clockwise
  digitalWrite(PIN_IN2, HIGH);  // control the motor's direction in clockwise

    analogWrite(PIN_ENA, 255); // speed up
    delay(10);
}
 else {
  digitalWrite(PIN_IN1, LOW); // control the motor's direction in clockwise
  digitalWrite(PIN_IN2, LOW);  // control the motor's direction in clockwise
 }

}
// void loop end
// void oled display header start

void oledDisplayHeader() {

 display.setTextSize(1);
 display.setTextColor(WHITE);  //WHITE
 display.setCursor(0, 0);
 display.print("Humidity");
 display.setCursor(60, 0);
 display.print("Temperature");
}


// void oled display header end
void onHumidityChange()  {
  humi = dht.readHumidity();
  // Read temperature as Celsius (the default)
  tempC = dht.readTemperature();
  // Check if any reads failed and exit early (to try again).

  }
// void oled display start

void oledDisplay(int size, int x,int y, float value, String unit)
{
 int charLen=12;
 int xo=x+charLen*3.2;
 int xunit=x+charLen*3.6;
 int xval = x; 
 display.setTextSize(size);
 display.setTextColor(WHITE);
 
 if (unit=="%")
 {
   display.setCursor(x, y);
   display.print(value,0);
   display.print(unit);
 } else 
 {
   if (value>99)
   {
    xval=x;
   } else 
   {
    xval=x+charLen;
   }
   display.setCursor(xval, y);
   display.print(value,0);
   display.drawCircle(xo, y+2, 2, WHITE);  // print degree symbols (  )
   display.setCursor(xunit, y);
   display.print(unit);
 }

}
// void oled display end
void onTempCChange()  {
  // Add your code here to act upon TempC change
}
/*
  Since Humi is READ_WRITE variable, onHumiChange() is
  executed every time a new value is received from IoT Cloud.
*/
void onHumiChange()  {
  // Add your code here to act upon Humi change
}
