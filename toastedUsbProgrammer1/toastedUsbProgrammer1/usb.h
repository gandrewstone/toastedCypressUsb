#ifndef usbH
#define usbH

#include "board.h"


typedef struct
  {
  uint8_t type;
  uint8_t request;
  uint16_t  value;
  uint16_t  index;
  uint16_t  length;
  } UsbControlRequest;

// What configuration did the host set us into?
extern uint8_t usbCfg;

bool usbStart(void);
void usbStop(void);
uint8_t usbGo(void);  // Demuxes the configuration

// Get USB frame #
uint16_t usbGetFrame(void);

// Wait for ACK, return 0 if configuration changed or timeout.
uint8_t usbAckWait(uint8_t endpt,uint32_t waitTime);

// Read up to nbytes from the ep.  Return number of bytes read
uint16_t usbRead(uint8_t ep, uint16_t nbytes, uint8_t* buf);
uint16_t usbWrite(uint8_t ep,uint16_t nbytes, uint8_t* buf);

#endif
