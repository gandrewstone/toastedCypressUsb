#include "board.h"
#include "usb.h"
#include "usbSerial.h"
#include "tick.h"
#include "offchipboot.h"
#include "avrisp.h"

/* COMMUNICATION */
#define INPUTBUFSZ 64
unsigned char inputBuf[INPUTBUFSZ];
unsigned char inputEnd=0;
unsigned char inputCur=0;

extern uint8_t error_count;

// 0 if the chip hasn't been erased yet.
unsigned char erased = 0;

union wordU {
	uint16_t word;
	uint8_t  byte[2];
};

unsigned int myget2BE(void)
{
  union wordU w;
  w.byte[0] = mygetch();
  w.byte[1] = mygetch();
  return w.word;
}

unsigned int myget2LE(void)
{
  union wordU w;
  w.byte[1] = mygetch();
  w.byte[0] = mygetch();
  return w.word;
}

#if 0
uint8_t lastLineState=0xff;
void lineStateHandler(void)
{
  if (lastLineState != serialCtrlLineState)
    {
      if ((serialCtrlLineState & CDC_ACM_DTR)==0)
        bitClr(IcspReset);     
      else
        bitSet(IcspReset);
  lastLineState =  serialCtrlLineState;
  }
  
}
#endif

void getchReadUsb(void)
{
  unsigned char bytesAvail=0;
  M8C_DisableGInt;
  if (USBFS_bGetEPAckState(USB_SERIAL_OUT_ENDPT))
    {
    if (1)
      {
        bitSet(UsbLed);      
        asm("nop"); asm("nop");
        bytesAvail = USBFS_bReadOutEP(USB_SERIAL_OUT_ENDPT, inputBuf, INPUTBUFSZ); 
        inputEnd = bytesAvail;
        inputCur=0;
        asm("nop"); asm("nop");
        // Let the next message in
        USBFS_EnableOutEP(USB_SERIAL_OUT_ENDPT);
        bitClr(UsbLed);        
      }
    }
    M8C_EnableGInt;
}

uint16_t pollch(void)
{
//#ifdef EMBED    
  uint16_t ret=0xffff;
  //lineStateHandler();
  if(inputCur==inputEnd)
  {
  //bitSet(RedLed);
  //delay(5*mSec);
  //bitClr(RedLed);
  //delay(5*mSec);
  //usbGo();
  //bitSet(RedLed);
  //delay(5*mSec);
  //lineStateHandler();
  getchReadUsb();
  if (inputCur == inputEnd) 
    {
    return 0xffff;
    }
  }
  ret = inputBuf[inputCur];
  inputCur++;
  //bitClr(RedLed);
  return ret;
//#endif  
}

uint8_t mygetch(void)
{
#ifdef EMBED    
  unsigned char ret=0;
  //delay(5*mSec);
  usbGo();
  //lineStateHandler();
  while(inputCur==inputEnd)
  {
  //bitSet(RedLed);
  //delay(5*mSec);
  //lineStateHandler();
  getchReadUsb();
  }
  ret = inputBuf[inputCur];
  inputCur++;
  //bitClr(RedLed);
  return ret;
#endif  
}

//#define max_time_count (15*1000000UL)
#define max_time_count (3*2*10000UL)

uint8_t bootldrgetNch(char* buf,uint8_t len)
{
  uint8_t i;
  for (i=0;i<len;i++) buf[i] = bootldrgetch();
  return len;
}

uint8_t bootldrgetch(void)
{
#ifdef EMBED    
  uint32_t count = 0;
  unsigned char ret=0;
  //delay(1*mSec);
  //usbGo();
  //lineStateHandler();
  while(inputCur==inputEnd)
  {
  count++;
  if (count>8000)
    {
    if ((count&255)==0) bitSet(UsbLed);
    if ((count&255)==((count>>8)&255)) bitClr(UsbLed);
    if (count > max_time_count)  // too long!  kick out of bootloader
      {
      return 0;
      }
    }
  //lineStateHandler();
  getchReadUsb();
  }
  ret = inputBuf[inputCur];
  inputCur++;
  //bitClr(RedLed);
  return ret;
#endif  
}

void myputch(uint8_t ch)
{
    putNch(&ch,1);
}

void putNch(uint8_t* vals, uint8_t len)
  {
  uint8_t retry=20;  // Wait an extremely long time, but ultimately come back so we don't hang.
  
#ifdef EMBED      
  #if 0
    while ((USBFS_bGetEPState(USB_SERIAL_IN_ENDPT) & IN_BUFFER_FULL)>0)
      {
      bitSet(RedLed);
      //delay(1);
      asm("nop"); asm("nop");  
      }
    bitClr(RedLed);
  #endif
  
  while(retry)
    {
    bitSet(UsbLed);
    retry--;
    if (!usbAckWait(USB_SERIAL_IN_ENDPT,50*mSec))
      {
      sendResponseNotification();
      }
  
    if (usbAckWait(USB_SERIAL_IN_ENDPT,100*mSec))
      {
    // Wait for prior data to be read
    // skip if the configuration is switched or something like that
      M8C_DisableGInt;
      USBFS_LoadInEP(USB_SERIAL_IN_ENDPT, vals,len, USB_TOGGLE);
      M8C_EnableGInt;
      sendResponseNotification();
      retry=0;
      }
    bitClr(UsbLed);     
    }  
        
    
 
#endif   

    //usbAckWait(USB_SERIAL_IN_ENDPT,100*mSec);
    //M8C_DisableGInt;
    //USBFS_LoadInEP(USB_SERIAL_IN_ENDPT, vals,0, USB_TOGGLE);
    //M8C_EnableGInt;

  }

 
  

void getNch(uint8_t count)
{
  while(count--) 
    {
      mygetch();	
    }
}

void allDone(void)
{
}

void initSerial(void)
{
#ifdef EMBED    
  uint16_t uartState;
  UsbControlRequest buf;

  erased = 0;

  // Send that we are connected to something
  cdcAcmSetNetCnxn(&buf,1);
  if (usbAckWait(USB_SERIAL_CTRL_IN_ENDPT,10000*mSec))
    {
       USBFS_LoadInEP(USB_SERIAL_CTRL_IN_ENDPT, (unsigned char*) &buf ,sizeof(UsbControlRequest), USB_TOGGLE);
    }
  else return;
  
  // Send that we are ready to receive
  buf.type = CDC_ACM_REQ_TYPE;
  buf.request = CDC_ACM_SERIAL_STATE;
  buf.value = 0;
  buf.index = 0;
  buf.length = 2;
  if (usbAckWait(USB_SERIAL_CTRL_IN_ENDPT,1000*mSec))
    USBFS_LoadInEP(USB_SERIAL_CTRL_IN_ENDPT, (unsigned char*) &buf ,sizeof(UsbControlRequest), USB_TOGGLE);
  else return;
  
  uartState = (1<<CDC_ACM_DCD_BIT) | (1<<CDC_ACM_DSR_BIT);
  if (usbAckWait(USB_SERIAL_CTRL_IN_ENDPT,1000*mSec))
    USBFS_LoadInEP(USB_SERIAL_CTRL_IN_ENDPT, (unsigned char*) &buf ,sizeof(UsbControlRequest), USB_TOGGLE);
  if (usbAckWait(USB_SERIAL_CTRL_IN_ENDPT,1000*mSec))
    USBFS_LoadInEP(USB_SERIAL_CTRL_IN_ENDPT, (unsigned char*) &uartState ,sizeof(uint16_t), USB_TOGGLE);
#endif

}

/* CHIP ACCESS */
uint8_t initChipAccess(unsigned char* devId)
{
  uint8_t ret = 0;
  unsigned long int sig=0;
  uint8_t i,j,k;
  for(i=3;((i<8)&&(!ret)&&(sig==0));i++)  /* search for the fastest working clock divider */
    {
    spiInit(i);
    ret = avrEnterProgramMode();
    if (ret) 
      {
      for(j=0;((j<5)&&(sig==0));j++)
        {
        sig = avrGetDeviceSig();
        devId[2]=sig>>16;
        devId[1]=sig>>8;
        devId[0]=sig;
        if (sig == 0x1E950F) break;
        else if (sig == 0x0f951e) break;
        else sig =0;
        }
      }
    }
  if (!ret) return ret;
  
#if 0  
  for(i=0;((i<10)&&(sig==0));i++)
      {
      sig = avrGetDeviceSig();
      }
  if (1)
    {
    devId[2]=sig>>16;
    devId[1]=sig>>8;
    devId[0]=sig;
    }
#endif    
  erased = 0;
  return true;
  //return ret;
}

void releaseChipAccess(void)
  {
    digitalWrite(SpiClk, LOW);  
    pinMode(SpiClkRegNum,SpiClkMsk,PinModeHighImpedence);
    //pinMode(IcspResetRegNum, IcspResetMsk, PinModeHighImpedence);
    digitalWrite(IcspReset,HIGH);
    delay(50*mSec);
    erased = 0;
  }

unsigned char issueSpiCmd(uint8_t *buf)
  {
  unsigned long int result = spiCmdc(buf[0],buf[1],buf[2],buf[3]);
  return result;
  }

void eraseDevice(void)
  {
  unsigned long int i=0;
  uint8_t retry = 5;
  //bitSet(RedLed);
  
  while(retry)
    {
    retry--;
    spiCmd(CmdChipErase);
    //delay((6-retry)*9*mSec);
    //bitSet(GrnLed);
    while(((spiCmd(CmdPollReady)&255)==0)&&(i<255)) {i++; delay(1*mSec); }
    if (i!=255)  // Oh no didn't work.  Try again
      {
      retry = 0;
      }
    }
  
  //pinMode(IcspResetRegNum, IcspResetMsk, PinModeHighImpedence);
  digitalWrite(IcspReset,HIGH);  
  delay(10*mSec);
  
  retry=3;
  while (retry>0)
    {
    if (avrEnterProgramMode()) break;
    retry--;
    }
  delay(20*mSec);
  //blinkLed(RedLedId,100*mSec,5);
  erased = 1;
  }



void eeprom_write_byte(void* address,uint8_t val)
{
  //printf("Write EEPROM Addr %p, val %c\n", address, val);
}

void flash_write_page(void* address, int length, char address_high, uint8_t* buf);

void flash_write(void* address, int length, char address_high, uint8_t* buf)
{
  //printf("Write FLASH Addr %p, len %d, high %d\n", address, length, address_high);
  //assert(((int) address)+length < FLASHSIZE);
  //memcpy(&fakeFlash[(int)address],buf,length);
  //return spiCmd(CmdReadFlashLow(address));
  int i,j;

  if (!erased)
    {
    eraseDevice();    
    }
  

  flash_write_page(address,length,address_high,buf);
  /*
  for (i=0;i<length;i+=FLASH_PAGE_SIZE)
    {
      flash_write_page(((uint8_t*)address)+i,FLASH_PAGE_SIZE,address_high,buf+i);
    }
*/
  }

void flash_write_page(void* address, int length, char address_high, uint8_t* b)
  {
  uint8_t retry = 100;
  unsigned long int addr = (unsigned long int) address;
  uint8_t* buf;
  int i,j;
  while (retry)
    {
    buf = b;
    addr = (unsigned long int) address;
    retry--;
    //M8C_DisableGInt;
    for (i=0;i<length;i+=2,addr+=2,buf+=2)
      {
      unsigned long int tmp;
 
      tmp = CmdLoadMemPageLow(addr/2,*buf);
      spiCmd(tmp);
    //for (j=0;i<10;j++) asm("nop");
      tmp = CmdLoadMemPageHigh(addr/2,*(buf+1));    
      spiCmd(tmp);
    //for (j=0;i<10;j++) asm("nop");
      }
    addr = (unsigned long int) address;  
    spiCmd(CmdWriteFlashPage((addr/2)));
#if 0
    if (0)
      {
      unsigned long int cnt = 0;
      while ((cnt<200000)&(spiCmd(CmdPollReady)&255)) cnt++;
      }
#endif      
    //M8C_EnableGInt;
    delay(20*mSec);  // Let the write algorithm work.
    
    // Quick verify by checking byte 0
    //if ((buf[0]==0xff) ||(pgm_read_byte_near(address) != 0xff)) retry=0;
    retry = 0;
    }
}

unsigned char cnt=0;

char eeprom_read_byte(void* address)
{
  //printf("Read EEPROM Addr %p\n", address);
  cnt++;
  return cnt+0x80;  
}

char pgm_read_byte_near(void* address)
{
  //printf("Read byte near %p\n", address);
  //cnt++;
  //return cnt; //fakeFlash[((int) address)];
  if (((unsigned long int)address)&1)
    return spiCmd(CmdReadFlashHigh(((unsigned long int)address)>>1)); 
  else
    return spiCmd(CmdReadFlashLow(((unsigned long int)address)>>1)); 
}


#ifdef	ADABOOT
void slow_flash_led(uint8_t count)
{
  //printf("slow flash led");
  blinkLed(RedLedId, 250*mSec,count);
}
#endif

void flash_led(uint8_t count)
{
  //printf("flash LED %d times", count);
  blinkLed(BluLedReg,BluLedMsk, 100*mSec,count);
}
