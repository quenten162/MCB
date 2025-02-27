#include <Arduino.h>
#include <BH1750.h>
#include <Wire.h>


#define SDA_PIN 21
#define SCL_PIN 22

BH1750 lightMeter(0x23); 
void setup() {
  Serial.begin(115200);
  Wire.begin(SDA_PIN, SCL_PIN); 
  
  if (lightMeter.begin(BH1750::CONTINUOUS_HIGH_RES_MODE)) {
    Serial.println("BH1750 conection");
  } else {
    Serial.println("BH1750 not connection");
  }
}

void loop() {
  float lux = lightMeter.readLightLevel();
  
  if (lux < 0) {
    Serial.println("[Error] cannot read");
  } else {
    Serial.print("lux sensor: ");
    Serial.print(lux);
    Serial.println(" lx");
  }
  
  delay(1000);
}
