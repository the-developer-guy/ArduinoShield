#include <SPI.h>
#include "music.h"
#include "max7219.h"
#include "blueShield.h"

volatile bool measurementReady = false;
volatile unsigned long startTime;
volatile unsigned long endTime;
int motorDir = 1;

void setup() {
  // put your setup code here, to run once:
  digitalWrite(PIN_CS_SHIFT, HIGH);
  digitalWrite(PIN_CS_MATRIX, HIGH);
  pinMode(PIN_CS_SHIFT, OUTPUT);
  pinMode(PIN_CS_MATRIX, OUTPUT);
  pinMode(PIN_SHIFT_ENABLE, OUTPUT);
  digitalWrite(PIN_SHIFT_ENABLE, HIGH);
  
  digitalWrite(PIN_LAMP_RED, HIGH);
  digitalWrite(PIN_LAMP_YELLOW, HIGH);
  digitalWrite(PIN_LAMP_GREEN, HIGH);
  pinMode(PIN_LAMP_RED, OUTPUT);
  pinMode(PIN_LAMP_YELLOW, OUTPUT);
  pinMode(PIN_LAMP_GREEN, OUTPUT);
  
  pinMode(PIN_BUZZER, OUTPUT);

  pinMode(PIN_TAMPER, INPUT_PULLUP);

  pinMode(PIN_ULTRASONIC_ECHO, INPUT);
  pinMode(PIN_ULTRASONIC_TRIGGER, OUTPUT);

  pinMode(PIN_MOTOR_DIR, OUTPUT);
  pinMode(PIN_MOTOR_PWM, OUTPUT);
  
  SPI.begin();
  Serial.begin(115200);
  attachInterrupt(digitalPinToInterrupt(PIN_ULTRASONIC_ECHO),
                  isr_us, CHANGE);
  rgb_off();
  maxTest(); 
  shiftTest();
  buzzerTest();
}

void loop() {
  // put your main code here, to run repeatedly:
  motor(motorDir);
  motorDir *= -1;
  triggerUltrasonicMeasurement();
  lampTest();
  tamperTest();
  ultrasonicTest();
}

void rgb_off(void) {
  digitalWrite(PIN_RGB_BLUE, HIGH);
  digitalWrite(PIN_RGB_RED, HIGH);
  digitalWrite(PIN_RGB_GREEN, HIGH);
}

void shiff(byte data) {
  digitalWrite(PIN_CS_SHIFT, HIGH);
  SPI.beginTransaction(SPISettings(14000000, MSBFIRST, SPI_MODE0));
  SPI.transfer(data);
  SPI.endTransaction();
  digitalWrite(PIN_CS_SHIFT, LOW);
  digitalWrite(PIN_CS_SHIFT, HIGH);
}

void triggerUltrasonicMeasurement(void) {
  digitalWrite(PIN_ULTRASONIC_TRIGGER, HIGH);
  delayMicroseconds(20);
  digitalWrite(PIN_ULTRASONIC_TRIGGER, LOW);
  measurementReady = false;
}

void isr_us(void) {
  if (digitalRead(PIN_ULTRASONIC_ECHO) == HIGH) {
    startTime = micros();
  }
  else {
    endTime = micros();
    measurementReady = true;
  }
}

void motor(int dir){
  if(dir > 0){
    analogWrite(PIN_MOTOR_PWM, 128);
    digitalWrite(PIN_MOTOR_DIR, LOW);
  }
  else if(dir < 0){
    analogWrite(PIN_MOTOR_PWM, 128);
    digitalWrite(PIN_MOTOR_DIR, HIGH);
  }
  else{
    analogWrite(PIN_MOTOR_PWM, 0);
    digitalWrite(PIN_MOTOR_DIR, LOW);
  }
}

void maxTest(){
  maxInit(PIN_CS_MATRIX);
  for(int i = 1; i <= 8; i++){
    maxTransfer(i, (1<<(i-1)));
    delay(100);
  }
  for(int i = 0; i < 8; i++){
    maxTransfer(8-i, (1<<i));
    delay(100);
  }
}

void shiftTest(void){
  uint8_t x = 0;
  digitalWrite(PIN_SHIFT_ENABLE, LOW);
  for (int i = 0; i <= 8; i++) {
    x <<= 1;
    x++;
    shiff(x);
    delay(100);
  }
}
void buzzerTest(void){
  for (int i = 0; i < 100; i++) {
    tone(PIN_BUZZER, i * 100, 10);
    delay(20);
  }
}

void lampTest(void){
  rgb_off();
  digitalWrite(PIN_LAMP_RED, LOW);
  delay(200);
  rgb_off();
  digitalWrite(PIN_LAMP_YELLOW, LOW);
  delay(200);
  rgb_off();
  digitalWrite(PIN_LAMP_GREEN, LOW);
  delay(200);
  rgb_off();
  for (int i = 0; i < 255; i++) {
    analogWrite(PIN_RGB_RED, i);
    analogWrite(PIN_RGB_GREEN, i);
    analogWrite(PIN_RGB_BLUE, i);
    delay(10);
  }
}

void tamperTest(void){
  
  if(digitalRead(PIN_TAMPER) == LOW){
    Serial.println("Tamper OK");
  }
  else{
    Serial.println("Tamper broken!");
  }
}

void ultrasonicTest(void){
  if (measurementReady == true) {
    measurementReady = false;
    unsigned long diff = endTime - startTime;
    unsigned long distance = (diff * 21) >> 7;
    Serial.println(distance);
  }
}
