
#include "board.h"
#include "tick.h"
#include "usb.h"

void spiInit(unsigned char spd)
  {
 #ifdef EMBED
    SPIM_Stop();
    delay(10*mSec);
    pinMode(SpiClkRegNum,SpiClkMsk,PinModeOutput);
    pinMode(SpiMosiRegNum,SpiMosiMsk,PinModeOutput);
    pinMode(SpiMisoRegNum,SpiMisoMsk,PinModeDigitalInput);
    asm("nop"); asm("nop");
    delay(50*mSec);
    M8C_DisableGInt;
    M8C_SetBank1;

    IO_CFG |=  IO_CFG_SPICLK_ON_P10 | IO_CFG_STRONG_DRIVE;
    IO_CFG &= ~IO_CFG_REGULATOR_OUTPUT_EN;
    asm("nop");
    // Note different clock values don't work need to find out why
    SPI_CFG = (spd << SpiClockSelShift) | SpiBypass; // | SpiIntSel | SpiSS_EN_ | SpiSS_;
    //SPI_CFG = SpiBypass | SpiIntSel | SpiSlave | SpiSS_EN_;  
    M8C_SetBank0;
    M8C_EnableGInt;
    
    bitSet(SpiMiso);  // Set it high so it is an "input" pin.

    SPIM_Start(SPIM_SPIM_MODE_0 | SPIM_SPIM_MSB_FIRST);
    //SPI_CR |= 1;
    M8C_SetBank0;
#endif    
  }

unsigned char spiXchg(unsigned char val)
  {
#ifdef EMBED      
  unsigned char i;
  //unsigned long int bail = 0;
  M8C_DisableGInt;
  //if (SPI_CR & SPI_CR_RX_Reg_Full) i = SPI_RXR;  // grab it and dump it.

  // Start a new SPI txn
  SPI_TXR = val;
  // Solve the TX_BUFFER EMPTY bug
  for(i=0;i<10;i++) asm("nop");
  //asm("nop"); asm("nop"); asm("nop"); asm("nop"); asm("nop"); asm("nop");
#if 0  
  while (((SPI_CR & SPI_CR_RX_Reg_Full)==0)&&(bail<0x7f)) 
    {
    bail++;
    for(i=0;i<5;i++) asm("nop");
    }
#endif    
  i = SPI_RXR;
#if 0  
  for(i=0;i<10;i++) asm("nop");
#endif  
  M8C_EnableGInt;
  return i;
#else
  return 0;
#endif
  }

unsigned long int spiCmdc(unsigned char a,unsigned char b,unsigned char c,unsigned char d)
  {
     unsigned long int ret=0;
     //ret = spiXchg(a); 
     //ret = (ret<<8) | spiXchg(b); 
     //ret = (ret<<8) | spiXchg(c); 
     //ret = (ret<<8) | spiXchg(d);
     ret = (((unsigned long int) spiXchg(a))<<24) | 
     (((unsigned long int) spiXchg(b))<<16) | 
     (((unsigned long int) spiXchg(c))<<8) |
     ((unsigned long int) spiXchg(d));  
     
     return ret;    
  }

unsigned long int spiCmd(unsigned long int cmd)
  {
     unsigned long int ret=0;
     ret = spiXchg(cmd>>24); 
     ret = (ret<<8) | spiXchg((cmd>>16)); 
     ret = (ret<<8) | spiXchg((cmd>>8)); 
     ret = (ret<<8) | spiXchg(cmd);
     return ret;
  }


bool avrEnterProgramMode(void)
  {
    uint8_t i;
    uint8_t retry=3;
    unsigned long int result;
    //pinMode(IcspResetRegNum, IcspResetMsk, PinModeOutput);
    while(retry)
      {
      retry--;
    //pinMode(IcspResetRegNum, IcspResetMsk, PinModeHighImpedence);
      digitalWrite(IcspReset,HIGH);
      digitalWrite(SpiClk, LOW);
      delay(40*mSec);
      digitalWrite(IcspReset,LOW);
      delay(40*mSec);
      for (i=0;i<5;i++)
        {
        result = spiCmdc(0xAC,0x53,0x00,0x00);

        if (((result>>8)&0xff) == 0x53) return true;
        spiCmd(0); // Clear it out and try again
        }
      }
    return false;
  }

unsigned long int avrGetDeviceSig(void)
  {
    char i;
    unsigned long int ret=0;
    spiCmd(0UL);
    for (i=0;i<3;i++)
      {
      //ret = (ret<<8) | ((unsigned long int) (spiCmdc(0x30,0,2-i,0)&255));
      ret = (ret<<8) | ((unsigned long int) (spiCmd(0x30000000UL | ((2UL-i)<<8))&255));
      }
    return ret;
  }

#ifdef TEST
void avrIspTest(void)
  {
    char i;
    char cr[] = "\n";
    char num2str[10];
    unsigned long int sig=0;
    //bitSet(RedLed);
    spiInit();
    //bitClr(RedLed);
    //bitSet(GrnLed);
    
    // Retry 10 times
    avrEnterProgramMode();
    for(i=0;((i<10)&&(sig==0));i++)
      {
    //bitSet(RedLed);
      sig = avrGetDeviceSig();
    //bitClr(RedLed);
    //bitClr(GrnLed);  
      }
      
    print(flash2ram("Device Signature: "));
    num2str[8]=0;
    uli2HexStr(sig,num2str);
    print(num2str);
    print(cr);
   
    
    digitalWrite(SpiClk, LOW);
    digitalWrite(IcspReset,HIGH);
    delay(50*mSec);
  }
#endif
