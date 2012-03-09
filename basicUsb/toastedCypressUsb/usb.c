#ifdef SIM
#include <exception>
using namespace std;
#include <errno.h>
#include <stdlib.h> /* for exit() */
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h> /* memset() */
#include <sys/time.h> /* select() */
#define SIZED_TYPES

#define UT_CUSTOM_OUT_PORT 12712
#define UT_CUSTOM_IN_PORT 12713

#endif

#include "usb.h"
#include "tick.h"

uint8_t usbCfg=0;

#ifdef EMBED
#include "USBFS.h"

#pragma  ioport   USB_MISC_CR:    0x1BD
BYTE              USB_MISC_CR;

uint16_t usbGetFrame(void)
{
    uint16_t result=0;
    M8C_SetBank0;
    result = (USB_SOF1)&7;
    result <<= 8;
    result |= (USB_SOF0);
    return result;
}

bool usbStart(void)
{
    //char i = 0;  
	//char buf = 0;
	
    M8C_EnableGInt;                   //Enable Global Interrupts  
	
	USBFS_Start(0, USB_5V_OPERATION); //Start USBFS Operation using device 0  
	  								  //and with 5V operation

	//USBFS_Start(1, USB_5V_OPERATION); //Start USBFS Operation using device 1  
	  								  //and with 5V operation  
                                      
    //delay(1000*mSec);
    //USBFS_Stop();
    //USB_CR0 = 0x80;  // Blow away the USB address, forcing reenumeration
    //USBFS_Stop();
    
#if 0    
    M8C_SetBank1;
    USB_CR1 = 0x03;  // EnableLock | RegulatorEnable
    USB_MISC_CR = 0x4 + 0x2 + 1;  // USB_SE_EN | USB_ON | USB_CLK_ON
    M8C_SetBank0;
#endif    
    
    
    
	return true;
}

void usbStop(void)
  {
  USBFS_Stop();
  }
  

uint8_t usbGo(void)
{
    char tmp;
    //bitSet(UsbLed);
	tmp = USBFS_bGetConfiguration(); 
    //bitClr(UsbLed);
    if (tmp != usbCfg)  // Switched
	  {
	  if (tmp == USB_CONFIG_SERIAL)
		{
		char buf=0;
        //bitSet(GrnLed);
		//Enumeration is completed load endpoint 1. Do not toggle the first time  
		USBFS_LoadInEP(USB_SERIAL_IN_ENDPT, &buf, 0, USB_NO_TOGGLE);
		USBFS_LoadInEP(USB_SERIAL_CTRL_IN_ENDPT, &buf, 0, USB_NO_TOGGLE);
        USBFS_EnableOutEP(USB_SERIAL_OUT_ENDPT); 
		}
	  else if (tmp == USB_CONFIG_USBTHING)
		{
		char buf=0;
        //RedLedReg |= RedLedMsk;
		//Enumeration is completed load endpoint 1. Do not toggle the first time  
		USBFS_LoadInEP(USB_CUSTOM_IN_ENDPT, &buf, 1, USB_NO_TOGGLE); 
        USBFS_EnableOutEP(USB_CUSTOM_OUT_ENDPT);
		}
#if 0        
	  else if (tmp == USB_CONFIG_HID)  // Keyboard/mouse
	    {  
        //GrnLedReg |= GrnLedMsk;
        //RedLedReg &= ~RedLedMsk;
		usbHidInit();
		}
#endif 
	  else
	    {
	    //RedLedReg &= ~RedLedMsk;
        //GrnLedReg &= ~GrnLedMsk;
		}
	  
	  usbCfg = tmp;
	  }

    if (usbCfg == USB_CONFIG_USBTHING)
	  {
	  //char c[] = "Hello world!";
	  //RedLedReg ^= RedLedMsk;	
  	  //Enumeration is completed load endpoint 1. Do not toggle the first time  
  	  //USBFS_LoadInEP(USB_CUSTOM_IN_ENDPT, c,13, USB_TOGGLE);
	  }
	else if (usbCfg == USB_CONFIG_HID)
	  {
	  //GrnLedReg ^= GrnLedMsk;
	  }
	else
	  {
	  //GrnLedReg |= GrnLedMsk;

	  }

	return usbCfg;
}

uint8_t usbAckWait(uint8_t endpt,uint32_t waitTime)
{
  unsigned char ret = 0;
  unsigned long int start = tick;
  unsigned char tmp;
  unsigned char i;
  
  while(!ret)
    {
     if (!(ret=USBFS_bGetEPAckState(endpt)))
       {
       if (USBFS_bGetConfiguration()!= usbCfg) // Oh no... config changed!
         { 
           return 0;  // Abort
         }
       tmp = USBFS_bGetEPState(endpt);
       //if (tmp == IN_BUFFER_EMPTY) return 1;
       if (tmp) return 1;
       
       if ((waitTime>0)&&(delta(start,tick)>waitTime))
         {
           return 0; // Timeout
         }
      
       #if 0
       blinkLed(RedLed,400*mSec);
       for (i=0;i<tmp;i++) 
         { 
         blinkLed(GrnLed,200*mSec);
         delay (200*mSec);
         }
       #endif
       }
       asm("nop"); asm("nop"); asm("nop");
    }
 return ret;
}


uint16_t usbWrite(uint8_t ep,uint16_t nbytes, uint8_t* buf)
  {
    // Wait for prior data to be read
    // skip if the configuration is switched or something like that
  if (1) //usbAckWait(ep,1000*mSec)  != 0)
    {
    M8C_DisableGInt;
    USBFS_LoadInEP(ep, buf,nbytes, USB_TOGGLE);
    M8C_EnableGInt;
    return nbytes;
    }
  else return 0;
  }

uint16_t usbRead(uint8_t ep, uint16_t nbytes, uint8_t* buf)
  {
  unsigned char bytesAvail=0;
 
  if (USBFS_bGetEPAckState(ep))   // Something arrived
    {
    M8C_DisableGInt;          

    //bytesAvail = USBFS_wGetEPCount(USB_SERIAL_OUT_ENDPT);
    //if (bytesAvail > INPUTBUFSZ) bytesAvail=INPUTBUFSZ;
    if (1) //bytesAvail)
      {
        asm("nop"); asm("nop");
        bytesAvail = USBFS_bReadOutEP(ep, buf, nbytes); 
        M8C_EnableGInt;  
        //delay(10*mSec); 
        asm("nop"); asm("nop");
        // Let the next message in  
        USBFS_EnableOutEP(ep);
        //bitClr(GrnLed);
      }
    }
    return bytesAvail;
  }


#if 0    
    if (1)
      {
      int state;
      int count;      
      int ep = USB_SERIAL_OUT_ENDPT;
      state = USBFS_bGetEPState(ep);
      if (state & EVENT_PENDING)
        {
        int i;
        count = USBFS_wGetEPCount(ep);
        for (i=0;i<count;i++)
          {
          bitSet(UsbLed);
          delay(50*mSec);
          bitClr(UsbLed); 
          delay(50*mSec);
          }
        }
      if (state & NO_EVENT_ALLOWED)
        {
        bitSet(UsbLed);
        bitSet(RedLed);
        
        delay(500*mSec);
        bitClr(UsbLed);  
        bitClr(RedLed);
        delay(100*mSec);
        }
      if (state & IN_BUFFER_EMPTY)
        {
        bitSet(UsbLed);
        bitSet(RedLed);
        
        delay(100*mSec);
        bitClr(UsbLed);  
        bitClr(RedLed);
        delay(100*mSec);
        }
      }
#endif

#else
#ifdef SIM  // These commands only exist on the embedded side, so if we arent simulating, don't implement them

class UsbError:public exception
{
public:
    UsbError(int err, const char* str);

    int err;
    char s[256];
    
};


UsbError::UsbError(int er, const char* str):exception()
{
    err = er;
    strncpy(s,str,255);
    s[255]=0;
    
    printf("UsbError Err: %d str: %s\n",err,str);
    
}



#define Error(err,str) UsbError(err,str)

int sockets[4];
struct sockaddr_in remoteServAddr;

void initSockets()
{
  int sd;
  struct sockaddr_in servAddr;
  struct hostent *h;

  if ((sd=socket(AF_INET, SOCK_DGRAM, 0))<0)
    throw Error(errno, "Cannot create or open server socket");

  /* bind local server port */
  servAddr.sin_family = AF_INET;
  servAddr.sin_addr.s_addr = htonl(INADDR_ANY);
  servAddr.sin_port = htons(UT_CUSTOM_OUT_PORT);
  int rc = bind (sd, (struct sockaddr *) &servAddr,sizeof(servAddr));
  if(rc<0) throw Error(errno, "Cannot bind server socket");
   
  sockets[UT_CUSTOM_OUT_ENDPT] = sd;

 

  struct sockaddr_in cliAddr;

 /* get server IP address (no check if input is IP address or DNS name */
  h = gethostbyname("localhost");
  if(h==NULL) throw Error(errno, "unknown host");

  remoteServAddr.sin_family = h->h_addrtype;
  memcpy((char *) &remoteServAddr.sin_addr.s_addr, h->h_addr_list[0], h->h_length);
  remoteServAddr.sin_port = htons(UT_CUSTOM_IN_PORT);

  sd = socket(AF_INET,SOCK_DGRAM,0);
  if (sd<0) throw Error(errno, "Cannot create or open server socket");

  cliAddr.sin_family = AF_INET;
  cliAddr.sin_addr.s_addr = htonl(INADDR_ANY);
  cliAddr.sin_port = htons(0);
  
  rc = bind(sd, (struct sockaddr *) &cliAddr, sizeof(cliAddr));
  if (rc<0) throw Error(errno, "Cannot create or open server socket");
    
   sockets[UT_CUSTOM_IN_ENDPT] = sd;
  
  //  if ((sockets[UT_CUSTOM_IN_ENDPT] = msgget(vendorId*0x10000 + productId + UT_CUSTOM_IN_ENDPT,  IPC_CREAT + 0777))== -1)
  //    throw Error(errno, "Cannot create or open message sockets");
  
  //if ((sockets[UT_CUSTOM_OUT_ENDPT] = msgget(vendorId*0x10000 + productId + UT_CUSTOM_OUT_ENDPT, IPC_CREAT + 0777))== -1)
  //  throw Error(errno, "Cannot open message output sockets");
  
}

// Wait for ACK, return 0 if configuration changed or timeout.
uint8_t usbAckWait(uint8_t endpt,uint32_t waitTime)
{

}


// Read up to nbytes from the ep.  Return number of bytes read
uint16_t usbRead(uint8_t ep, uint16_t nbytes, uint8_t* buf)
{
    
   int16_t len = recvfrom(sockets[ep&0x7f], buf, nbytes, 0, NULL, 0);
   if (len<0) throw Error(errno, "recvfrom error");
   
   return len;   
}

uint16_t usbWrite(uint8_t ep,uint16_t nbytes, uint8_t* buf)
{
    int rc = sendto(sockets[ep&0x7f],buf,nbytes,0,(struct sockaddr *) &remoteServAddr, sizeof(remoteServAddr));
    if (rc == -1) throw Error(errno,"sendto failed");
}

bool usbStart(void)
{
    initSockets();
    
}


uint8_t usbGo(void)
{
    usbCfg =USB_CONFIG_USBTHING;
    return USB_CONFIG_USBTHING;
}


#endif
#endif
