#include <Wire.h>
#include <Servo.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "DFRobot_VL53L0X.h"
#include "redShield.h"
#include "24AA02E48T.h"


Servo s;
Adafruit_SSD1306 display(128, 32, &Wire, -1);
DFRobot_VL53L0X sensor;
volatile int pulseCount = 0;
int textY = 16;

void setup() {
  // put your setup code here, to run once:
  pinMode(PIN_ENCODER_A, INPUT);
  pinMode(PIN_ENCODER_B, INPUT);
  pinMode(PIN_SERVO, OUTPUT);
  pinMode(11, OUTPUT);
  pinMode(12, INPUT);
  pinMode(13, OUTPUT);
  pinMode(PIN_SD_WP, INPUT_PULLUP);
  pinMode(PIN_SD_CD, INPUT_PULLUP);  
  pinMode(PIN_SERVO, OUTPUT);
  s.attach(PIN_SERVO);
  attachInterrupt(digitalPinToInterrupt(PIN_ENCODER_A), isr_encoder, CHANGE);
  Wire.begin();
  Serial.begin(115200);
  
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.display();
  
  uint64_t eui48 = eepromGetEUI48();
  Serial.print((uint32_t)(eui48 >> 32), HEX);
  Serial.println((uint32_t)(eui48), HEX);

  for(int i = 0; i < 0x80; i++){
    eepromWriteByte(i, i);
    delay(EEPROM_24AA02E48T_WRITE_TIME_MS);
  }
  Serial.println(eepromReadByte(0));
  for(int i = i; i < 0x80; i++){
    Serial.println(eepromReadByte());
    delay(1);
  }
  
  sensor.begin(I2C_ADDR_VL53L0X);
  sensor.setMode(sensor.eContinuous,sensor.eHigh);
  sensor.start();
  
  
}

void loop() {
  // put your main code here, to run repeatedly:
  int adc = analogRead(PIN_ADC_POT);
  textY += pulseCount;
  pulseCount = 0;
  s.write(adc >> 2);

  if(textY > 25){
    textY = 25;
  }
  else if(textY < 0){
    textY = 0;
  }
    
  display.clearDisplay();
  display.setCursor(adc >> 4,textY);
  display.println(F("OLED teszt"));
  display.display();

  bool cardPresent = digitalRead(PIN_SD_CD) == LOW;
  bool cardWriteProtect = digitalRead(PIN_SD_WP);
  int temperature = analogRead(PIN_ADC_NTC);
  int light = analogRead(PIN_ADC_LIGHT);

  Serial.print("Light: ");
  Serial.print(light);
  Serial.print("\ttemperature: ");
  Serial.print(temperature);
  Serial.print("\tLaser: ");
  Serial.print(sensor.getDistance());
  Serial.print("mm");
  if(cardPresent)
  {
    Serial.print("\tSD card found, write protect:");
    if(cardWriteProtect)
    {
      Serial.println("on");
    }
    else
    {
      Serial.println("off");
    }
  }
  else
  {
    Serial.println("\tNo SD card");
  }
  

  delay(200);
}

void isr_encoder(void){
  if(digitalRead(PIN_ENCODER_A) == HIGH){
    if(digitalRead(PIN_ENCODER_B) == LOW){
      pulseCount++;
    }
    else{
      pulseCount--;
    }
  }
  else{
    if(digitalRead(PIN_ENCODER_B) == LOW){
      pulseCount--;
    }
    else{
      pulseCount++;
    }
  }
}
