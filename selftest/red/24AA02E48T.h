#ifndef TDG_24AA02E48T_H
#define TDG_24AA02E48T_H

#include <Arduino.h>

#define EEPROM_24AA02E48T_EUI48_ADDRESS (0xFA)
#define EEPROM_24AA02E48T_WRITE_TIME_MS (5)

uint64_t eepromGetEUI48(void);
bool eepromWriteByte(uint8_t address, uint8_t data);
bool eepromWritePage(uint8_t address, uint8_t* data, uint8_t count);

uint8_t eepromReadByte(void);
uint8_t eepromReadByte(uint8_t address);
uint8_t eepromRead(uint8_t* data, uint8_t count);
uint8_t eepromRead(uint8_t address, uint8_t* data, uint8_t count);

#endif // TDG_24AA02E48T_H
