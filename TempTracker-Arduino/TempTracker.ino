#include <LiquidCrystal_I2C.h>
#include <math.h>
#include <SPI.h>
#include <SD.h>

const int B = 4275;               // B value of the thermistor
const int R0 = 100000;            // R0 = 100k
const int pinTempSensor = A0;     // Grove - Temperature Sensor connect to A0

File TempData;
const int SD_CS = 10;
const long SLEEP_TIME = 5000;  // write to card every 5 seconds
long multiplier = 0;

LiquidCrystal_I2C lcd(0x3F, 20, 4);

void setup() {

  lcd.begin();
  lcd.setCursor(2,0);
  lcd.print("## Temp Tracker ##");

  if (SD.begin(SD_CS)) {
    lcd.setCursor(2,2);
    lcd.print("SD card init");
  }
}

void loop() {

  int a = analogRead(pinTempSensor);
  float R = 1023.0/a-1.0;
  R = R0*R;
  // convert to temperature (F) via temp component datasheet
  float temperature = 1.0/(log(R/R0)/B+1/298.15)-273.15; 
  temperature = (temperature * 1.8) + 32;
  lcd.setCursor(2,1);
  lcd.print(temperature);
  if (millis() > long(multiplier * SLEEP_TIME)){
    writeToSD(temperature);
    lcd.setCursor(8,1);
    lcd.print(multiplier);
    multiplier++;
  }
}

void writeToSD(float temperature){
    TempData = SD.open("temps.csv", FILE_WRITE);
    if (TempData) {
      lcd.setCursor(2,2);
      lcd.print("file WRITE success");
      TempData.print(millis());
      TempData.print(",");
      TempData.println(temperature);
      TempData.close();
    }
    else{
      lcd.setCursor(2,3);
      lcd.print("file WRITE fail    ");
    }
}


