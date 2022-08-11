#include <SPI.h>
#include "max7219.h"

static int csPin = 10;

void maxInit(int pin){
  csPin = pin;
  pinMode(csPin, OUTPUT);
  digitalWrite(csPin, HIGH);
  SPI.begin();
  SPI.setBitOrder(MSBFIRST);
  
  maxTransfer(MAX_REG_SHUTDOWN, MAX_VAL_SHUTDOWN_ON);
  maxTransfer(MAX_REG_SCAN_LIMIT, MAX_VAL_SCAN_LIMIT_ALL);
  maxTransfer(MAX_REG_INTENSITY, 0x01);
  maxTransfer(MAX_REG_DECODE_MODE, MAX_VAL_DECODE_NO);
  maxTransfer(MAX_REG_DISPLAY_TEST, MAX_VAL_DISPLAY_NORMAL);  
}

void maxTransfer(uint8_t command, uint8_t data) {
  digitalWrite(csPin, LOW);
  SPI.beginTransaction(SPISettings(16000000, MSBFIRST, SPI_MODE0));
  SPI.transfer(command);
  SPI.transfer(data);
  SPI.endTransaction();
  digitalWrite(csPin, HIGH);
}
