#ifndef TDG_MAX7219_h
#define TDG_MAX7219_h

#define MAX_REG_NOP 0x00
#define MAX_REG_DIGIT_BASE 0x01
#define MAX_REG_DECODE_MODE 0x09
#define MAX_REG_INTENSITY 0x0a
#define MAX_REG_SCAN_LIMIT 0x0b
#define MAX_REG_SHUTDOWN 0x0c
#define MAX_REG_DISPLAY_TEST 0x0f

#define MAX_VAL_DISPLAY_NORMAL 0x00
#define MAX_VAL_DISPLAY_TEST 0x01

#define MAX_VAL_DECODE_NO 0x00
#define MAX_VAL_DECODE_DIGIT_0 0x01
#define MAX_VAL_DECODE_DIGIT_0_3 0x0f
#define MAX_VAL_DECODE_ALL 0xff

#define MAX_VAL_SHUTDOWN_OFF 0x00
#define MAX_VAL_SHUTDOWN_ON 0x01

#define MAX_VAL_SCAN_LIMIT_ALL 0x07

void maxInit(int pin);
void maxTransfer(uint8_t command, uint8_t data);

#endif // TDG_MAX7219_h
