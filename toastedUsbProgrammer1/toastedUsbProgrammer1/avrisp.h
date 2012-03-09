#ifndef AVRISPH
#define AVRISPH


extern void avrIspTest(void);
extern void spiInit(unsigned char spd);
extern unsigned char spiXchg(unsigned char val);


extern unsigned long int spiCmd(unsigned long int cmd);
extern unsigned long int spiCmdc(unsigned char a,unsigned char b,unsigned char c,unsigned char d);

extern unsigned long int avrGetDeviceSig(void);
extern bool avrEnterProgramMode(void);

#define ATMEGA328P_SIG  0x000f951EUL

#define FLASH_PAGE_SIZE 64

#define CmdChipErase           0xAC800000UL
#define CmdReadFlashLow(addr) (0x20000000UL | (((unsigned long int)(addr))<<8))
#define CmdReadFlashHigh(addr)     (0x28000000UL | (((unsigned long int)(addr))<<8))

#define CmdWriteFlashLow(addr,val)    (0x60000000UL | (((unsigned long int)(addr))<<8) | (val&255))
#define CmdWriteFlashHigh(addr,val)   (0x68000000UL | (((unsigned long int)(addr))<<8) | (val&255))

#define CmdReadEeprom(addr)           (0xa0000000UL | (((unsigned long int)(addr))<<8))
#define CmdWriteEeprom(addr,val)      (0xc0000000UL | (((unsigned long int)(addr))<<8) | (val&255))

#define CmdLoadMemPageLow(addr,val)   (0x40000000UL | (((unsigned long int)(addr)&255)<<8) | (val&255))
#define CmdLoadMemPageHigh(addr,val)  (0x48000000UL | (((unsigned long int)(addr)&255)<<8) | (val&255))
#define CmdWriteFlashPage(addr)   (0x4C000000UL | (((unsigned long int)(addr))<<8))

#define CmdWriteLockBits(val) (0xACE00000UL | (val&255))
#define CmdWriteFuseLowBits(val) (0xACA00000UL | (val&255))
#define CmdWriteFuseHighBits(val) (0xACA80000UL | (val&255))
#define CmdWriteFuseExtendedBits(val) (0xACA40000UL | (val&255))


#define CmdPollReady 0xF0000000UL

#endif
