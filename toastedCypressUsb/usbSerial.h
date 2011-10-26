#include "board.h"
#include "usb.h"

#define USB_ACM_RESPONSE_AVAILABLE 1

extern volatile uint8_t serialCtrlLineState;
extern volatile uint16_t serialDtrSetCount;

extern void cdcAcmSetNetCnxn(UsbControlRequest* buf, unsigned char state);

extern void cdcAcmSetSerialState(UsbControlRequest* buf, unsigned char state);


// Send a notification on the interrupt endpoint saying that data is ready
uint8_t sendResponseNotification(void);

void usbSerialSetup(void);
void SendSerialState(void);

typedef struct
{
  uint32_t upstreamBitRate;
  uint32_t downstreamBitRate;
} CdcAcmConnectionSpeedChange;

typedef struct
{
  uint32_t dataRate;
  uint8_t stopBits;
  uint8_t parity;
  uint8_t dataBits;
} CdcAcmLineCoding;

enum
  {
  CDC_ACM_REQ_TYPE = 0xA1,
  CDC_ACM_NOTIF_TYPE =0xA1,
  CDC_ACM_RESP_TYPE = 0x21,
  };

enum
{
CDC_ACM_RTS = 0x2,
CDC_ACM_DTR = 0x1
};


typedef enum
  {
  CDC_ACM_END_ENCAPSULATED_COMMAND                 =    0x00,
  CDC_ACM_GET_ENCAPSULATED_RESPONSE                =    0x01,
  CDC_ACM_SET_COMM_FEATURE                         =    0x02,
  CDC_ACM_GET_COMM_FEATURE                         =    0x03,
  CDC_ACM_CLEAR_COMM_FEATURE                       =    0x04,
    
  CDC_ACM_SET_AUX_LINE_STATE                       =    0x10,
  CDC_ACM_SET_HOOK_STATE                           =    0x11,
  CDC_ACM_PULSE_SETUP                              =    0x12,
  CDC_ACM_SEND_PULSE                               =    0x13,
  CDC_ACM_SET_PULSE_TIME                           =    0x14,
  CDC_ACM_RING_AUX_JACK                            =    0x15,
    
  CDC_ACM_SET_LINE_CODING                          =    0x20,
  CDC_ACM_GET_LINE_CODING                          =    0x21,
  CDC_ACM_SET_CONTROL_LINE_STATE                   =    0x22,
  CDC_ACM_SEND_BREAK                               =    0x23,
    
  CDC_ACM_SET_RINGER_PARMS                         =    0x30,
  CDC_ACM_GET_RINGER_PARMS                         =    0x31,
  CDC_ACM_SET_OPERATION_PARMS                      =    0x32,
  CDC_ACM_GET_OPERATION_PARMS                      =    0x33,
  CDC_ACM_SET_LINE_PARMS                           =    0x34,
  CDC_ACM_GET_LINE_PARMS                           =    0x35,
  CDC_ACM_DIAL_DIGITS                              =    0x36,
  CDC_ACM_SET_UNIT_PARAMETER                       =    0x37,
  CDC_ACM_GET_UNIT_PARAMETER                       =    0x38,
  CDC_ACM_CLEAR_UNIT_PARAMETER                     =    0x39,
  CDC_ACM_GET_PROFILE                              =    0x3A,
    
  CDC_ACM_SET_ETHERNET_MULTICAST_FILTERS           =    0x40,
  CDC_ACM_SET_ETHERNET_POWER_MANAGEMENT_PATTERN_FILTER =    0x41,
  CDC_ACM_GET_ETHERNET_POWER_MANAGEMENT_PATTERN_FILTER =    0x42,
  CDC_ACM_SET_ETHERNET_PACKET_FILTER               =    0x43,
  CDC_ACM_GET_ETHERNET_STATISTIC                   =    0x44,
    
  CDC_ACM_SET_ATM_DATA_FORMAT                      =    0x50,
  CDC_ACM_GET_ATM_DEVICE_STATISTICS                =    0x51,
  CDC_ACM_SET_ATM_DEFAULT_VC                       =    0x52,
  CDC_ACM_GET_ATM_VC_STATISTICS                    =    0x53
      
  } CdcAcmRequests;


typedef enum
  {
  CDC_ACM_NETWORK_CONNECTION  = 0x0, //value: 0 � Disconnect 1 � Connected
  CDC_ACM_RESPONSE_AVAILABLE  = 1, 
  CDC_ACM_AUX_JACK_HOOK_STATE = 8,           // 0 � On hook 1 � Off hook
  CDC_ACM_RING_DETECT         = 9,     
  CDC_ACM_SERIAL_STATE        = 0x20, 
  CDC_ACM_CALL_STATE_CHANGE   = 0x28,
  CDC_ACM_LINE_STATE_CHANGE   = 0x29,
  CDC_ACM_CONNECTION_SPEED_CHANGE = 0x2A
  } CdcAcmNotifications;


typedef enum
  {
  CDC_ACM_DCD_BIT           = 0,  // Data carrier detect
  CDC_ACM_DSR_BIT           = 1,  // Data set ready
  CDC_ACM_BREAK_BIT         = 2,
  CDC_ACM_RING_BIT          = 3,
  CDC_ACM_FRAMING_ERROR_BIT = 4,
  CDC_ACM_PARITY_ERROR_BIT  = 5,
  CDC_ACM_OVERRUN           = 6
  } CdcAcmSerialStateData;
