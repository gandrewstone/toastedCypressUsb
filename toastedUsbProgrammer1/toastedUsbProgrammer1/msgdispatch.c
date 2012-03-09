#include "board.h"
#include "usb.h"
#include "tick.h"

//#include "slim_pro.h"
#ifdef SLIMVME
void slimVmeStart(uint32_t algoSize, uint32_t dataSize);
#endif

void msgDispatch(uint8_t* usbCmd, uint8_t len)
{
    uint8_t cmd = 0;
    uint8_t swap = 0;

    if (len >= sizeof(UsbThingHdr))
    {
        UsbThingHdr* hdr;
        
        hdr = (UsbThingHdr*) usbCmd;
        
        // Verify the version and discover endian
        if (hdr->verid == UsbThingCommVerId)
        {
            cmd = hdr->cmd;
        }
        else if (hdr->verid == UsbThingCommVerIdByteSwap)
        {
            //
            cmd = hdr->cmd;
            swap = 1;
        }
    }
        
    if (cmd == UsbThingControlRequestTypeRead)
    {
        if (len >= sizeof(UsbThingRead))
        {
            UsbThingRead* msg;
            unsigned int read;
            unsigned int output = 0;
            msg = (UsbThingRead*) usbCmd;
            if (swap) read = sswap(msg->pins);
            else read = msg->pins;
            //blinkLed(GrnLedId,1000*mSec,4);
            if (read&(1<<UsbThingRX))
            {
                //blinkLed(RedLedId,1000*mSec,4);
                if (bitGet(Rx)) output |= (1<<UsbThingRX);
            }
            
            if (swap) msg->pins = sswap(output);
            else msg->pins = output;
            usbWrite(USB_CUSTOM_IN_ENDPT, sizeof(UsbThingRead), (uint8_t*) msg);
        }
    }
    else if (cmd == UsbThingControlRequestTypeBitBang)
    {
        //blinkLed(GrnLedId,50*mSec,5);
        if (len >= sizeof(UsbThingBitBang))
        {
          
            UsbThingBitBang* bb;
            unsigned int mod;
            unsigned int val;
            //blinkLed(GrnLedId,1000*mSec,4);
            bb = (UsbThingBitBang*) usbCmd;
            if (swap)
            {
                mod = sswap(bb->modify);
                val = sswap(bb->value);
            }
            else
            {
                mod = bb->modify;
                val = bb->value;
            }
            
            if (mod&(1<<UsbThingCTS))
            {
                //blinkLed(RedLedId,1000*mSec,4);
                if (val&(1<<UsbThingCTS)) 
                {
                    bitSet(Cts);
                    //blinkLed(GrnLedId,1000*mSec,4);
                }
                else bitClr(Cts);
            }
            if (mod&(1<<UsbThingTX))
            {
                if (val&(1<<UsbThingTX)) bitSet(Tx);
                else bitClr(Tx);
            }
            if (mod&(1<<UsbThingRX))
            {
                if (val&(1<<UsbThingRX)) bitSet(Rx);
                else bitClr(Rx);
            }
            if (mod&(1<<UsbThingRTS))
            {
                if (val&(1<<UsbThingRTS)) bitSet(Rts);
                else bitClr(Rts);
            }
            if (mod&(1<<UsbThingMOSI))
            {
                if (val&(1<<UsbThingMOSI)) bitSet(SpiMosi);
                else bitClr(SpiMosi);
            }
            if (mod&(1<<UsbThingMISO))
            {
                if (val&(1<<UsbThingMISO)) bitSet(SpiMiso);
                else bitClr(SpiMiso);
            }
            if (mod&(1<<UsbThingSCK))
            {
                if (val&(1<<UsbThingSCK)) bitSet(SpiClk);
                else bitClr(SpiClk);
            }
            if (mod&(1<<UsbThingRST))
            {
                if (val&(1<<UsbThingRST)) bitSet(IcspReset);
                else bitClr(IcspReset);
            }
#if 0            
            if (mod&(1<<UsbThingBLULED))
            {
                if (val&(1<<UsbThingBLULED)) bitSet(UsbLed);
                else bitClr(UsbLed);
            }  
#endif            
            #if 0
            if (mod&(1<<UsbThingREDLED))
            {
                if (val&(1<<UsbThingREDLED)) bitSet(RedLed);
                else bitClr(RedLed);
            }
            #endif
            //blinkLed(RedLedId,300*mSec,4);

        }
    }
    else if (cmd == UsbThingControlRequestTypeClockIn)
    {
        UsbThingClockIn* msg = (UsbThingClockIn*) usbCmd;
        uint8_t* databyte;
        uint8_t i;
        uint8_t j;
        //bitSet(RedLed);
        for (i=0,databyte=usbCmd+sizeof(UsbThingClockIn);i<msg->len;i++,databyte++)
        {
            uint8_t bit;
            uint8_t d = usbCmd[sizeof(UsbThingClockIn)+i]; //*databyte;
            for (bit=0;bit<8;bit++,d<<=1)
            {
                if (d & 0x80)
                {
                    bitSet(Rts);
                }
                else bitClr(Rts);
                //delay(1);
                bitSet(Cts);
                //delay(10);
                //delay(1000 * mSec);
                bitClr(Cts);
                //delay(10);
                //delay(500 * mSec);
            }
        }
        //bitClr(RedLed);  
    }
#ifdef SLIMVME    
    else if (cmd == UsbThingControlRequestTypeStartSlimVme)
    {
        UsbThingStartSlimVme* req = (UsbThingStartSlimVme*) usbCmd;
        
        slimVmeStart(lendianfix(req->algoLen,swap),lendianfix(req->dataLen,swap));        
    }
#endif    
    
    else blinkLed(BluLedReg,BluLedMsk,200*mSec,cmd);
}
