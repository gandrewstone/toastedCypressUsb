//----------------------------------------------------------------------------
// C main line
//----------------------------------------------------------------------------

#include "board.h"
#include "tick.h"
#include "usb.h"
#include "usbSerial.h"
#include "offchipboot.h"
#include "fifobuf.h"

unsigned char usbCmd[64];

extern void msgDispatch(uint8_t* msg, uint8_t len);

void loopback(void)
{
  uint8_t letter='p';
  uint32_t lasttick = tick;
  uint16_t c;
  uint16_t resetCount=0; // = serialDtrSetCount;
  uint8_t lastLineState = serialCtrlLineState;
  uint8_t loop=2;
    
  while (loop) 
    {
    unsigned int amt;
    
    //EP2_CR0 =  0xf;
    //EP3_CR0 = 0xb;
    //bitSet(RedLed);
    usbGo();
    //bitClr(RedLed);
    if (usbCfg != USB_CONFIG_SERIAL) return;
            
    if (USBFS_bCheckActivity())
      {
      c = pollch();
      if (c != 0xffff) 
        {
        myputch(c);
        }
      }
    if (lasttick < tick)
      {
      SendSerialState();
      lasttick = tick + 0x4000UL;
      bitTgl(UsbLed);
      //delay(50*mSec);
      //bitClr(UsbLed);
    //delay(50*mSec);  
      myputch(letter);
      if (letter=='z') letter='A';
      else letter++;
      }    
    }  
}

 
 uint8_t serial2Spi(void)
{
  FifoBuf fromUsb;
  uint8_t    bufPos=0;
  uint8_t letter='A';
  uint8_t ret=0;
  uint32_t lasttick = tick;
  uint16_t c;
  uint16_t resetCount=0; // = serialDtrSetCount;
  uint8_t lastLineState = serialCtrlLineState;
  uint8_t loop=2;
  uint16_t usbFrame=0;
  uint32_t oldTick =tick;
  
  fifoCtor(&fromUsb);
  
  //bitSet(UsbLed);
  pinMode(RedLedReg, RedLedMsk, PinModeDigitalInput);
  
  pinMode(SpiClkRegNum,SpiClkMsk,PinModeDigitalInput);
  bitSet(SpiClk);
  
  pinMode(SpiMosiRegNum,SpiMosiMsk,PinModeDigitalInput);
  bitSet(SpiMosi);
  
  pinMode(SpiMisoRegNum,SpiMisoMsk,PinModeOutput);
  asm("nop"); asm("nop");
  SPIS_Start(SPIS_SPI_MODE_0 | SPIS_SPI_MSB_FIRST);
  M8C_SetBank1;
  IO_CFG |=  IO_CFG_SPICLK_ON_P10 | IO_CFG_STRONG_DRIVE;
  SPI_CFG = SpiBypass | SpiIntSel | SpiSlave | SpiSS_EN_;  
  //SpiSS_EN_ = (1<<2),
  
  M8C_SetBank0;
  delay(1*mSec);  
  SPI_CR = SPI_CR_Enable;
  delay(5*mSec);
  
  while (loop) 
    {
    unsigned int amt;
    uint16_t curFrame;
    uint8_t spiCr=0;
    
    //EP2_CR0 =  0xf;
    //EP3_CR0 = 0xb;
    //bitSet(RedLed);
    usbGo();
    //bitClr(RedLed);
    if (usbCfg != USB_CONFIG_SERIAL) {loop=0; break; }
    else if ((tick-oldTick > 0xfff)||(tick<oldTick))  // If the frame hasn't changed, USB is disconnected
      {
      oldTick = tick;
      curFrame = usbGetFrame();
      if (curFrame == usbFrame) {loop--; ret=1; break;}
      else loop=2;
      usbFrame=curFrame;
      }
    
    // Echo:
    //if (resetCount != serialDtrSetCount) loop=0;
    if ((lastLineState&CDC_ACM_DTR) != (serialCtrlLineState&CDC_ACM_DTR))
      {
      lastLineState =  serialCtrlLineState;
      loop=0;
      if ((serialCtrlLineState & CDC_ACM_DTR)==1) loop=0;  // Back to bootloader
      }
    
    // If I turn off the USB LED, then turn it on again after some time
    // to make it blink instead of flicker.
    if ((tick&0xff)==0) { bitSet(BluLed); bitClr(UsbLed); }
    
    if ((!fifoFull(&fromUsb)) && USBFS_bCheckActivity())
      {
      c = pollch();
      if (c != 0xffff) 
        {
        //bitTgl(UsbLed);
        //myputch(c);  // For debugging, echo to serial
        fifoPush(&fromUsb,c);
        }
      }

    if (lasttick < tick)
      {
      lasttick = tick + 0x1000UL;
      if (bufPos) putNch(usbCmd,bufPos);
      bufPos = 0;
      }
    
      
    #if 1  // Outputs letters to the USB for debugging
    if (lasttick < tick)
    {
      SendSerialState();
      lasttick = tick + 0x4000UL;
      //bitTgl(UsbLed);
      //delay(50*mSec);
      //bitClr(UsbLed);
    //delay(50*mSec);  
      myputch(letter);
      if (letter=='z') letter='A';
      else letter++;
      }
    #endif
     

    //delay(1*mSec);
    
   //if(( SPIS_bReadStatus() & (SPIS_SPIS_SPI_COMPLETE | SPIS_RX_BUFFER_FULL) )
   
   spiCr = SPI_CR;
   if (spiCr & SPI_CR_RX_Reg_Full) //SPIS_SPIS_SPI_COMPLETE)
     {
     
     if (1) //spiCr&SPI_CR_TX_Reg_Empty)
       {
       c = fifoPop(&fromUsb);
       if (c != 0xFFFF) 
         {
         SPI_TXR = c;
         bitTgl(UsbLed);
         }
       else SPI_TXR = 0xff;
       }
       
     if (1) //spiCr&SPI_CR_RX_Reg_Full)
       {
       c = SPI_RXR;
       if (c != 0xff) 
         {
         //bitTgl(UsbLed);
         usbCmd[bufPos] = c; 
         bufPos++;  
         if (bufPos==64) { putNch(usbCmd, bufPos); bufPos = 0; }
         }//myputch(c);
       }
     }

#if 0
    if(0) // SPIS_bReadStatus() & SPIS_SPIS_SPI_COMPLETE)
      {
            
      /* read the received data */  
      //bData = SPIS_bReadRxData(); 
      c = spiXchg(c);
      myputch(c);
      bitSet(UsbLed);
      delay(100*mSec);

      }
#endif      
    }  
  SPIS_Stop();
  return ret;
}



void main(void)
{   
    uint8_t usbGood=1;
    uint32_t oldTick=0;
    uint16_t usbFrame=0;
    uint8_t i;
    tickStart();
        
    pinMode(SpiClkRegNum,SpiClkMsk,PinModeHighImpedence);
    pinMode(RedLedReg, RedLedMsk, PinModeHighImpedence);

#if 0
    // Blink the time interval
    for(i=0;i<10;i++)
      {
      bitSet(BluLed);
	  bitClr(UsbLed);
      //bitSet(RedLed);
      delay(1000*mSec);
      bitClr(BluLed);
	  bitSet(UsbLed);
      //bitClr(RedLed);
      delay(1000*mSec);
    }
#endif	


    // Blink the version
    for(i=0;i<1;i++)
      {
      bitSet(BluLed);
      //bitSet(RedLed);
      delay(200*mSec);
      bitClr(BluLed);
      //bitClr(RedLed);
      delay(200*mSec);
    }
    for(i=0;i<2;i++)
      {
      bitSet(BluLed);
      //bitSet(RedLed);
      delay(100*mSec);
      bitClr(BluLed);
      //bitClr(RedLed);
      delay(100*mSec);
    }

    // Take AVR out of reset
    //pinMode(IcspResetRegNum, IcspResetMsk, PinModeHighImpedence);
    bitSet(IcspReset); 

    usbSerialSetup();
    usbStart();

#ifndef SIM
    M8C_DisableGInt;
    M8C_SetBank1;
    IO_CFG |=  IO_CFG_SPICLK_ON_P10 | IO_CFG_STRONG_DRIVE;
    IO_CFG &= ~IO_CFG_REGULATOR_OUTPUT_EN;
    M8C_SetBank0;
    M8C_EnableGInt;
#endif

  while (1)
    {
    unsigned int amt;
    unsigned char c;

    usbGo();
    bitSet(IcspReset);
    
    if ((tick-oldTick > 0xfff)||(tick<oldTick)||(!usbGood))  // If the frame hasn't changed, USB is disconnected
      {
      uint16_t curFrame = usbGetFrame();
      oldTick = tick;
      if (curFrame == usbFrame) usbGood=0;
      else usbGood=1;
      usbFrame=curFrame;
      }
 
    if ((usbCfg == USB_CONFIG_SERIAL)&&(usbGood))
      {
      bitSet(BluLed);
	  bitClr(UsbLed);
      c = serial2Spi();
      usbGo();
      //if ((c!=1)&&(usbCfg == USB_CONFIG_SERIAL))
	  bitClr(BluLed);
	  bitSet(UsbLed);
	  bootloader();      
      }
    else
      {
      bitClr(BluLed);
	  bitClr(UsbLed);
      delay(200*mSec);
	  bitSet(BluLed);
	  bitSet(UsbLed);
	  delay(200*mSec);
         // Take AVR out of reset, no USB is connected
      //pinMode(IcspResetRegNum, IcspResetMsk, PinModeHighImpedence);
      //bitSet(IcspReset); 
      }

#if 0
    if (usbCfg == USB_CONFIG_USBTHING)
      {
      //unsigned char tmp[8];
      //unsigned char* usbCmd = &tmp[0];
      //unsigned char usbCmd[8];
      unsigned char len;
      unsigned char cmd = 0;
      unsigned char swap = 0;
      amt = 100;
      DbgPrint("Top level custom config dispatch loop\n");
      len = usbRead(USB_CUSTOM_OUT_ENDPT, UsbThingLongestMsg, usbCmd);
      msgDispatch(usbCmd,len);
      }
#endif    
    }


}
