#include <Arduino.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <DallasTemperature.h>


const int oneWireBus = 12;     

OneWire oneWire(oneWireBus);

Adafruit_SSD1306 display(-1);
DallasTemperature sensors(&oneWire);

void displayTemp(float temp);
void getTemp();


void setup()   
{         
  Serial.begin(112500);       
  // initialize with the I2C addr 0x3C
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  

  // temp begin
  sensors.begin();

  // Clear the buffer.
  display.clearDisplay();


}

void loop() {

  getTemp();
  delay(1000);


}

void getTemp(){
  sensors.requestTemperatures(); 
  float temp = sensors.getTempCByIndex(0);
  Serial.println(temp);
  displayTemp(temp);
}
void displayTemp(float temp){
   	
  // Display Text
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0,15);
  display.print("Temperatur: ");
  display.print(temp, 2);
  display.write(248);
  display.println("C"); 
  display.display();
  delay(1000);

}


