#ifndef toastedUsbThingH
#define toastedUsbThingH

#ifndef __GNUC__
#define EMBED
#endif

#define UsbThumbProgrammer
//#define Lightuino

// Essential CPU defines
#ifdef EMBED
#include <m8c.h>        // part specific constants and macros
#include "PSoCAPI.h"    // PSoC API definitions for all User Modules
#include "PSoCDynamic.h"
#endif

// General defines to make standard C palatable...
#ifdef EMBED
enum
{
true = 1,
false= 0
};
#else
#define __flash
// Already defed in stdint.h
//#define SIZED_TYPES
#endif

#ifndef SIZED_TYPES
#define SIZED_TYPES
#ifdef EMBED
typedef unsigned char uint8_t;
typedef unsigned short int uint16_t;
typedef unsigned long int uint32_t;

typedef char int8_t;
typedef int int16_t;
typedef long int int32_t;
typedef unsigned char bool;
#else
#include <stdint.h>
#endif
typedef bool boolean;
#endif

// Defines to help PSOC make sense...
#define cli() M8C_DisableGInt
#define sei() M8C_EnableGInt


enum 
{
PinModeResistiveDrain = 0,   /* Resistive high, drive down */
PinModeOutput = 1,    /* Push/Pull, Strong, drives both high & low */
PinModeHighImpedence = 2,
PinModeDigitalInput = 3,
PinModeOpenDrain = 3,   /* high z when high, drives down (i2c) */
};

extern void pinMode(char port, char pinmask, char mode);

#define REG Reg

#define echo(__x) __x
#define shadow(x) x##Shadow
#define ccat(x,y) x##_##y

#define HIGH 1
#define LOW  0
#define digitalWrite(_bit, _val) do { if (_val) bitSet(_bit); else bitClr(_bit); } while(0)

//#define bitSet(bit) do { shadow(ccat(bit,REG)) |= bit##Msk; bit##Reg = shadow(ccat(bit,REG)); } while(0)
//#define bitClr(bit) do { shadow(bit##Reg) &= ~bit##Msk; bit##Reg = shadow(bit##Reg); } while(0)
//#define bitTog(bit) do { shadow(bit##Reg) ^= bit##Msk; bit##Reg = shadow(bit##Reg); } while(0)

#define bitSet(bit) do { regShadow[bit##RegNum] |= bit##Msk; bit##Reg = regShadow[bit##RegNum]; } while(0)
#define bitClr(bit) do { regShadow[bit##RegNum] &= ~(bit##Msk); bit##Reg = regShadow[bit##RegNum]; } while(0)
#define bitTgl(bit) do { regShadow[bit##RegNum] ^= bit##Msk; bit##Reg = regShadow[bit##RegNum]; } while(0)

#define bitGet(bit) (bit##Reg & bit##Msk)
//#define bitSet(bit) do { regShadow[bit##RegNum] |= bit##Msk; bit##Reg = regShadow[bit##RegNum]; } while(0)

extern unsigned char regShadow[5];

extern unsigned char PRT0DRShadow;
extern unsigned char PRT1DRShadow;

#ifdef SIM
extern uint8_t PRT0DR;
extern uint8_t PRT1DR;
extern uint8_t PRT2DR;
#define M8C_SetBank0
#define M8C_SetBank1

#define DbgPrint printf
#else
#define DbgPrint 
#endif

// Set pin to CPU, Strong
#ifdef Lightuino
#define UsbLedId 0
#define UsbLedRegNum 2
#define UsbLedMsk (1<<3) 
#define UsbLedReg PRT2DR

#define BluLedId 0
#define BluLedRegNum 2
#define BluLedMsk (1<<3) 
#define BluLedReg PRT2DR

//#define RedLedId 1
//#define RedLedRegNum 1
#define RedLedMsk (1<<0)
#define RedLedReg PRT1DR
#endif
#ifdef UsbThumbProgrammer
#define UsbLedId 0
#define UsbLedRegNum 1
#define UsbLedMsk (1<<4) 
#define UsbLedReg PRT1DR

#define BluLedId 0
#define BluLedRegNum 0
#define BluLedMsk (1<<4) 
#define BluLedReg PRT0DR

//#define RedLedId 1
//#define RedLedRegNum 1
#define RedLedMsk (1<<4)
#define RedLedReg PRT1DR
#endif

#define FtdiRxRegNum 0
#define FtdiRxMsk (1<<3) 
#define FtdiRxReg PRT0DR

#define SpiClkRegNum 1
#define SpiClkReg PRT1DR
#define SpiClkMsk (1<<0)

#define SpiMosiRegNum 1
#define SpiMosiReg PRT1DR
#define SpiMosiMsk (1<<1)

#define SpiMisoRegNum 1
#define SpiMisoReg PRT1DR
#define SpiMisoMsk (1<<5)

#ifdef Lightuino
#define IcspResetRegNum  1
#define IcspResetReg  PRT1DR
#define IcspResetMsk (1<<4)
#endif
#ifdef UsbThumbProgrammer
#define IcspResetRegNum  1
#define IcspResetReg  PRT1DR
#define IcspResetMsk (1<<7)
#endif

#define CtsRegNum 0
#define CtsReg  PRT0DR
#define CtsMsk (1<<1)

#define TxRegNum 0
#define TxReg  PRT0DR
#define TxMsk (1<<7)

#define RxRegNum 0
#define RxReg  PRT0DR
#define RxMsk (1<<3)

#define RtsRegNum 2
#define RtsReg  PRT2DR
#define RtsMsk (1<<5)


#define CpuSpd (24000000UL/8UL)
#define TICK_PERIOD_USEC (100UL)

#define TICK_COUNT  (CpuSpd/100000UL*(TICK_PERIOD_USEC/10UL))    //(CpuSpd/10000UL)   // Make the timer fire at 50 uS
#define uSecPerTick TICK_PERIOD_USEC

// units to functions like delay that take a tick interval
#define mSec (1000UL/TICK_PERIOD_USEC)              //(CpuSpd/1000UL)/TICK_COUNT    //(20UL/8UL)
#define Sec (1000UL*mSec)

//#define TicksPerMs 10UL

extern volatile uint32_t tick;





enum
{
  IO_CFG_SPICLK_ON_P10 = (1<<2),
  IO_CFG_STRONG_DRIVE  = (1<<7),
  IO_CFG_REGULATOR_OUTPUT_EN = (1<<1)
  };

enum
    {
    SpiClockSelShift = 5, // Clock Selection. These bits determine the operating frequency of the SPI Master.
    /*
    000b - SysClk / 2
    001b - SysClk / 4
    010b - SysClk / 8
    011b - SysClk / 16
    100b - SysClk / 32
    101b - SysClk / 64
    110b - SysClk / 128
    111b - SysClk / 256
    */
    SpiBypass      = (1<<4),  //- Bypass Synchronization. This bit determines if the inputs are synchronized to SYSCLK.
    SpiSS_         = (1<<3),  //- Slave Select. This bit determines the logic value of the SS_ signal when the SS_EN_ signal is asserted (SS_EN_ = 0).
    SpiSS_EN_      = (1<<2),  //- Slave Select Enable. This active low bit determines if the slave select (SS_) signal is driven internally. If it is driven internally, its logic level is determined by the SS_ bit. If it is driven externally, its logic level is determined by the external pin.
    SpiIntSel      = (1<<1),  // - Interrupt Select. This bit selects which condition produces an interrupt, whether it is based off of the TX Reg Empty condition or the SPI Complete condition.
    SpiSlave       = (1<<0)   //- This bit determines whether the block functions as a master or slave.
    };



#define USB_CONFIG_SERIAL 1
#define USB_CONFIG_USBTHING 2
#define USB_CONFIG_HID 3

#define USB_IFC_USBTHING 2

// Set these to your configured endpoints
enum
{
  USB_HID_MOUSE_ENDPT = 2,
  USB_HID_KB_ENDPT =   1,
  
  USB_CUSTOM_OUT_ENDPT = 2,
  USB_CUSTOM_IN_ENDPT = 1,
  
  USB_SERIAL_CTRL_IN_ENDPT = 1,
  USB_SERIAL_IN_ENDPT = 2,
  USB_SERIAL_OUT_ENDPT = 3,
  }; 
  

typedef enum     
  {
#ifdef EMBED      
    TCK = 0,
    TMS = 1,
    TDO = 2,
    TDI = 3,
    
    CTS = 0,
    TX  = 1,
    RX  = 2,
    RTS = 3,

    MOSI = 4,
    MISO = 5,
    SCK  = 6,
    RST  = 7,
    
    REDLED = 8,
    GRNLED = 9,
#endif
    
     UsbThingTCK = 0,
     UsbThingTMS = 1,
     UsbThingTDO = 2,  // out from the chip (into the computer)
     UsbThingTDI = 3,  // In to the chip
    
     UsbThingCTS = 0,
     UsbThingTX  = 1,
     UsbThingRX  = 2,
     UsbThingRTS = 3,

     UsbThingMOSI = 4,
     UsbThingMISO = 5,
     UsbThingSCK  = 6,
     UsbThingRST  = 7,
    
     UsbThingREDLED = 8,
     UsbThingBLULED = 9,
    
    
  } JtagPins;

enum
  {
    UsbThingCommVerId = 0x2701,
    UsbThingCommVerIdByteSwap = 0x0127,
    UsbThingLongestMsg = 64,
    UsbThingControlRequestTypeBitBang = 1,
    UsbThingControlRequestTypeRead = 2,
    UsbThingControlRequestTypeConfigPins = 3,
    UsbThingControlRequestTypeClockIn = 4,
    UsbThingControlRequestTypeStartSlimVme = 5,
    UsbThingControlRequestTypeDataFile = 6,  // Same cmd # for req and response
    UsbThingControlRequestTypeComplete = 7, 

    // From the usbThing to computer
    UsbThingStdout=0x80,
    UsbThingLog=0x90,
    UsbThingIR=0xA0,
    
    // From the computer to the usbThing
    UsbThingStdin=0xB0,
    UsbThingCtrl=0xF0,   // Enable/Disable functions (like the IR)

    // Bidirectional
    UsbThingI2c=0x20,
    UsbThingSPI=0x30,
    UsbThingSerial=0x40,

    // files you can request
    UsbThingSlimVmeDataFile = 0,
    UsbThingSlimVmeAlgoFile = 1,
  };

typedef struct
{
  uint16_t verid;
  uint8_t  cmd;
  uint8_t  txn;
} UsbThingHdr;

typedef struct
{
  UsbThingHdr hdr;
  uint16_t modify;
  uint16_t value;
} UsbThingBitBang;

typedef struct
{
  UsbThingHdr hdr;
  uint16_t pins;
} UsbThingRead;

typedef struct
{
  UsbThingHdr hdr;
  uint8_t dataInPin;
  uint8_t clkPin;
  uint8_t len;
  uint8_t xtra;
  // The rest of the packet is the data to clock in
} UsbThingClockIn;

typedef struct
{
  UsbThingHdr hdr;
  uint32_t algoLen;
  uint32_t dataLen;
} UsbThingStartSlimVme;

typedef struct
{
  UsbThingHdr hdr;
    uint32_t offset;
    uint32_t len;    
  uint8_t file;
} UsbThingDataReq;
// Response only contains the header.

#define sswap(x) (((x&255)<<8) | (x>>8))
#define lswap(x) (((x&0x000000FF)<<24)|((x&0x0000FF00)<<8)|((x&0x00FF0000)>>8)|((x&0xFF000000)>>24))

#define sendianfix(x,swap) ((swap)? sswap(x):x)
#define lendianfix(x,swap) ((swap)? lswap(x):x)


#endif
