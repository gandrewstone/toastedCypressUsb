#include "board.h"
#include "usbSerial.h"

volatile uint8_t serialCtrlLineState=0;
volatile uint16_t serialDtrSetCount = 0;

uint8_t sendResponseNotification(void)
  {
    if (usbAckWait(USB_SERIAL_CTRL_IN_ENDPT,100*mSec) != 0)
        {
        UsbControlRequest buf;
        buf.request = CDC_ACM_NOTIF_TYPE;
        buf.type = USB_ACM_RESPONSE_AVAILABLE; 
        buf.value = 0;
        buf.index = 0;
        buf.length = 0;
        USBFS_LoadInEP(USB_SERIAL_CTRL_IN_ENDPT, (unsigned char*) &buf ,sizeof(UsbControlRequest), USB_TOGGLE);
        return true;
        }
    return false;    
  }        
        
void fillUsbControlRequest(volatile UsbControlRequest* req)
{
  unsigned char tmp;
  req->type = EP0_DR0;
  tmp = EP0_DR1;
  req->request = tmp;
  req->value = (EP0_DR3*256) + EP0_DR2;
  req->index = (EP0_DR5*256) + EP0_DR4;
  req->length = (EP0_DR7*256) + EP0_DR6;
  //RedLedReg ^= RedLedMsk;
  EP0_DR0 = 0;
  EP0_DR1 = 0;
  EP0_DR2 = 0;
  EP0_DR3 = 0;
  EP0_DR4 = 0;
  EP0_DR5 = 0;
  EP0_DR6 = 0;  
  EP0_DR7 = 0;
}
  
volatile UsbControlRequest incomingReq;
CdcAcmLineCoding UsbCdcLineCoding;

void usbSerialSetup(void)
{
    UsbCdcLineCoding.dataRate = 9600*4;
    UsbCdcLineCoding.stopBits = 0;  // 1 stop bit
    UsbCdcLineCoding.parity   = 0;
    UsbCdcLineCoding.dataBits = 8;
}

#pragma interrupt_handler usbCtlClassDevice
void usbCtlClassDevice(void)
{
  UsbControlRequest req;
  fillUsbControlRequest(&req);  
}

#pragma interrupt_handler usbCtlClassInterfaceReq
void usbCtlClassInterfaceReq(void)
{
  UsbControlRequest req;
  //bitTgl(UsbLed);
  fillUsbControlRequest(&req);  
}


//#pragma interrupt_handler usbCtlIfc;
//uint8_t usbCtlIfc(void)


#pragma interrupt_handler usbCtlClassInterface
uint8_t usbCtlClassInterface(void)
{
  UsbControlRequest req;
  //bitTgl(UsbLed);
  fillUsbControlRequest(&req);
  if (req.request == CDC_ACM_SET_CONTROL_LINE_STATE)
    {
    serialCtrlLineState &= ~(CDC_ACM_RTS | CDC_ACM_DTR);
    serialCtrlLineState |= req.value&(CDC_ACM_RTS | CDC_ACM_DTR);
    if (req.value & CDC_ACM_RTS)
      {
      //bitSet(UsbLed);
      }
    //else bitClr(UsbLed);
    if (req.value & CDC_ACM_DTR)
      {
      serialDtrSetCount++;
      }
    //else bitClr(RedLed);
    return 1;
    }
  if (req.request == CDC_ACM_GET_LINE_CODING)
    {
    CdcAcmLineCoding lc;
    lc.dataRate = 9600*4;
    lc.stopBits = 0;  // 1 stop bit
    lc.parity   = 0;
    lc.dataBits = 8;
    return 1;
    }  
  if (req.request == CDC_ACM_SET_LINE_CODING)
    {
    
    }
    return 0;
}


// Vendor specific control requests

//#pragma interrupt_handler usbCtrlIfcHost2Device;
void usbCtrlIfcHost2Device(void)
{
  //bitSet(UsbLed);
}

//# interrupt_handler usbCtrlIfcDevice2Host;
void usbCtrlIfcDevice2Host(void)
{
  //bitTgl(UsbLed);
}

//#pragma interrupt_handler usbCtrlHost2Device;  
void usbCtrlHost2Device(void)
{
  //bitSet(UsbLed);
  //RedLedReg ^= RedLedMsk;
  //unsigned char tmp = EP0_DR1;
  fillUsbControlRequest(&incomingReq);

    
  //asm("ljmp USBFS_InitNoDataStageControlTransfer");
  //asm("ljmp USBFS_InitControlWrite");
  
}

//extern unsigned char USBFS_InitNoDataControlTransfer(void);

unsigned char usbCtrlDevice2Host(void)
{
  unsigned char bRequestHandled = FALSE;
  
  fillUsbControlRequest(&incomingReq);
  
  //GrnLedReg ^= GrnLedMsk;
  //asm("ljmp USBFS_InitControlWrite");
  //asm("ljmp USBFS_InitNoDataStageControlTransfer");
  //bRequestHandled = USBFS_InitNoDataControlTransfer();
  //STARTbit =TRUE;
  //asm("ljmp USBFS_NoDataStageControlTransfer");
  return bRequestHandled;
}

typedef struct 
{
  UsbControlRequest hdr;
  uint16_t          uartState;
} UsbSerialStateNotif;

void SendSerialState(void)
  {
  if (1)
    {
        UsbSerialStateNotif buf;
        
        buf.hdr.type        = CDC_ACM_NOTIF_TYPE;
        buf.hdr.request     = CDC_ACM_SERIAL_STATE;
        buf.hdr.index       = sswap(0);
        buf.hdr.value       = 0;
        buf.hdr.length      = sswap(2);
        //buf.uartState = 0;
        buf.uartState = sswap((1<<CDC_ACM_DCD_BIT) | (1<<CDC_ACM_DSR_BIT));
        usbAckWait(USB_SERIAL_CTRL_IN_ENDPT,100*mSec);
        
        USBFS_LoadInEP(USB_SERIAL_CTRL_IN_ENDPT, (unsigned char*) &buf ,sizeof(UsbSerialStateNotif), USB_TOGGLE);
    } 
  
#if 0
    if (1)
    {
        UsbControlRequest buf;
        uint16_t uartState;
        
        buf.type        = CDC_ACM_REQ_TYPE;
        buf.request     = CDC_ACM_SERIAL_STATE;
        buf.index       = 1;
        buf.value       = 0;
        buf.length      = 2;
        
        uartState = (1<<CDC_ACM_DCD_BIT) | (1<<CDC_ACM_DSR_BIT);
        if (usbAckWait(USB_SERIAL_CTRL_IN_ENDPT,200*mSec))
            USBFS_LoadInEP(USB_SERIAL_CTRL_IN_ENDPT, (unsigned char*) &buf ,sizeof(UsbControlRequest), USB_TOGGLE);
        if (usbAckWait(USB_SERIAL_CTRL_IN_ENDPT,200*mSec))
            USBFS_LoadInEP(USB_SERIAL_CTRL_IN_ENDPT, (unsigned char*) &uartState ,sizeof(uint16_t), USB_TOGGLE);
    } 
#endif    
  } 

extern void cdcAcmSetNetCnxn(UsbControlRequest* buf, unsigned char state)
{
  buf->type = CDC_ACM_REQ_TYPE;
  buf->request    = CDC_ACM_NETWORK_CONNECTION;
  buf->index       = 0;
  buf->value       = state;
  buf->length      = 0;
}

extern void cdcAcmDataReady(UsbControlRequest* buf)
{
  buf->type = CDC_ACM_REQ_TYPE;
  buf->request    = CDC_ACM_SERIAL_STATE;
  buf->index       = 0;
  buf->value       = 0;
  buf->length      = 2;
}

