#include "board.h"
/* "main" */
extern void bootloader(void);

#ifndef SIZED_TYPES
#define SIZED_TYPES
typedef unsigned char     uint8_t;
typedef unsigned int      uint16_t;
typedef unsigned long int uint32_t;
typedef char int8_t;
typedef int int16_t;
typedef long int int32_t;

#ifdef EMBED
typedef unsigned char bool;
#endif

typedef bool boolean;
#endif

/* I need this buffer, you may reuse it */
extern uint8_t buff[256];

/* internal function prototypes */
extern void myputch(uint8_t);
extern void putNch(uint8_t* vals, uint8_t len);
extern uint16_t pollch(void);  // nonblocking getch 
extern uint8_t mygetch(void);
extern uint8_t bootldrgetch(void);  // times out, etc.  Only for use in the bootloader
extern uint8_t bootldrgetNch(char* buf,uint8_t len);
extern unsigned int myget2BE(void);
extern unsigned int myget2LE(void);

/* Issue a 4 byte SPI AVR command and return a 1 byte response */
extern unsigned char issueSpiCmd(uint8_t* buf);
extern void getNch(uint8_t);
extern void byte_response(uint8_t);
extern void nothing_response(void);
extern void failed_response(void);

extern char gethex(void);
extern void puthex(uint8_t);
extern void flash_led(uint8_t count); //,uint16_t predelay,uint16_t ontime,uint16_t offtime);
extern void slow_flash_led(uint8_t count);
extern void initSerial(void);


extern uint8_t initChipAccess(unsigned char* devId);
extern void releaseChipAccess(void);
extern void allDone(void);

extern void eraseDevice(void);
extern void eeprom_write_byte(void* address,uint8_t val);
extern void flash_write(void* address, int length, char address_high, uint8_t* buf);
extern char eeprom_read_byte(void* address);
extern char pgm_read_byte_near(void* address);
