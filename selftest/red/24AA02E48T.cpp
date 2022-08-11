#include "redShield.h"
#include "24AA02E48T.h"
#include <Wire.h>

uint64_t eepromGetEUI48(void){
  uint64_t EUI48 = 0;
  uint8_t i = 0;
  Wire.beginTransmission(I2C_ADDR_EEPROM);
  Wire.write(EEPROM_24AA02E48T_EUI48_ADDRESS);
  Wire.endTransmission();

  Wire.requestFrom(I2C_ADDR_EEPROM, 6);
  while(Wire.available() && i < 6){
    EUI48 <<= 1;
    EUI48 |= Wire.read();
    i++;
  }
  return EUI48;
}
bool eepromWriteByte(uint8_t address, uint8_t data){
  Wire.beginTransmission(I2C_ADDR_EEPROM);
  Wire.write(address);
  Wire.write(data);
  Wire.endTransmission();
}
bool eepromWritePage(uint8_t address, uint8_t* data, uint8_t count){
  uint8_t i = 0;
  if(count <= 8){
    Wire.beginTransmission(I2C_ADDR_EEPROM);
    Wire.write(address);
    for(i=0; i < count; i++){
      Wire.write(data[i]);
    }
    Wire.endTransmission();
  }
}

uint8_t eepromReadByte(void){
  uint8_t data;
  
  Wire.requestFrom(I2C_ADDR_EEPROM, 1);
  if(Wire.available()){
    data = Wire.read();
  }
  return data;
}
uint8_t eepromReadByte(uint8_t address){
  uint8_t data;
  
  Wire.beginTransmission(I2C_ADDR_EEPROM);
  Wire.write(address);
  Wire.endTransmission();
  
  Wire.requestFrom(I2C_ADDR_EEPROM, 1);
  if(Wire.available()){
    data = Wire.read();
  }
  return data;
}
uint8_t eepromRead(uint8_t* data, uint8_t count){
  uint8_t i = 0;
  
  Wire.requestFrom(I2C_ADDR_EEPROM, count);
  while(Wire.available() && i < count){
    data[i] = Wire.read();
    i++;
  }
  return i;
}
uint8_t eepromRead(uint8_t address, uint8_t* data, uint8_t count){
  uint8_t i = 0;
  Wire.beginTransmission(I2C_ADDR_EEPROM);
  Wire.write(address);
  Wire.endTransmission();

  Wire.requestFrom(I2C_ADDR_EEPROM, count);
  while(Wire.available() && i < count){
    data[i] = Wire.read();
    i++;
  }
  return i;
}
