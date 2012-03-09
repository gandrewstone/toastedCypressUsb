/**********************************************************/
/* Serial Bootloader for Atmel megaAVR Controllers        */
/*                                                        */
/* tested with ATmega328                                  */
/* should work with other mega's, see code for details    */
/*                                                        */
/* ATmegaBOOT.c                                           */
/*                                                        */
/* 20100310: G. Andrew Stone added PWM pulsing of pin 13  */
/*           LED while loader is waiting for a sketch to  */
/*           come.  Also added feature where double-      */
/*           clicking reset button jumps directly to      */
/*           the sketch.  It is possible (extremely       */
/*           unlikely) for a sketch to set the right      */
/*           bytes in RAM right to cause this behavior    */
/*           without the user choosing it.                */
/*           In that case, depower board, depress the     */
/*           reset button and power the board WHILE       */
/*           holding the reset button pressed.  This      */
/*           will kick you back to the bootloader.        */
/* 20090325: BBR applied ADABoot fixes to the 328 enabled */
/*           bootloader code of Arduino-0014. Many ADABOOT*/
/*           included by DAM under WATCHDOG Mods #ifdef   */
/* 20080813: BBR tweaked flash_led()  timing and          */
/*           shortened timeout n Makefile                 */
/* 20071005: BBR tweaked delay values in flash_led()      */
/* 20071004: B Riley - modified flash_led() amd its invo- */
/*           cation to give unique signature and version  */
/*           for ADABOOT. Search on ADABOOT for changes   */
/* 20071003: Limore Fried and B Riley, hacks to timeout,  */
/*           upload progress detector on Dig13, BL abort  */
/*           of serial port                               */
/* 20070626: hacked for Arduino Diecimila (which auto-    */
/*           resets when a USB connection is made to it)  */
/*           by D. Mellis                                 */
/* 20060802: hacked for Arduino by D. Cuartielles         */
/*           based on a previous hack by D. Mellis        */
/*           and D. Cuartielles                           */
/*                                                        */
/* Monitor and debug functions were added to the original */
/* code by Dr. Erik Lins, chip45.com. (See below)         */
/*                                                        */
/* Thanks to Karl Pitrich for fixing a bootloader pin     */
/* problem and more informative LED blinking!             */
/*                                                        */
/* For the latest version see:                            */
/* http://www.chip45.com/                                 */
/*                                                        */
/* ------------------------------------------------------ */
/*                                                        */
/* based on stk500boot.c                                  */
/* Copyright (c) 2003, Jason P. Kyle                      */
/* All rights reserved.                                   */
/* see avr1.org for original file and information         */
/*                                                        */
/* This program is free software; you can redistribute it */
/* and/or modify it under the terms of the GNU General    */
/* Public License as published by the Free Software       */
/* Foundation; either version 2 of the License, or        */
/* (at your option) any later version.                    */
/*                                                        */
/* This program is distributed in the hope that it will   */
/* be useful, but WITHOUT ANY WARRANTY; without even the  */
/* implied warranty of MERCHANTABILITY or FITNESS FOR A   */
/* PARTICULAR PURPOSE.  See the GNU General Public        */
/* License for more details.                              */
/*                                                        */
/* You should have received a copy of the GNU General     */
/* Public License along with this program; if not, write  */
/* to the Free Software Foundation, Inc.,                 */
/* 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA */
/*                                                        */
/* Licence can be viewed at                               */
/* http://www.fsf.org/licenses/gpl.txt                    */
/*                                                        */
/* Target = Atmel AVR m128,m64,m32,m16,m8,m162,m163,m169, */
/* m8515,m8535. ATmega161 has a very small boot block so  */
/* isn't supported.                                       */
/*                                                        */
/* Tested with m168                                       */
/**********************************************************/

/* $Id$ */
// stty -F /dev/ttyUSB2 speed 19200
// gcc -g offchipboot.c -o offchipboot
/* some includes */

#ifdef EMBED
#include <m8c.h>        // part specific constants and macros
#include "PSoCAPI.h"    // PSoC API definitions for all User Modules
#include "avrisp.h"
#include "usbHid.h"
#endif

#include "offchipboot.h"
#include "board.h"
#include "tick.h"

//#undef bitTgl
//#undef bitSet
//#undef bitClr
//#define bitTgl(x) 
//#define bitSet(x) 
//#define bitClr(x) 

#define ADABOOT
#define SIMULATION


#define SIG1	0x1E	// Yep, Atmel is the only manufacturer of AVR micros.  Single source :(
#define SIG2	0x95
#define SIG3	0x0F

#define PAGE_SIZE	0x40U	//64 words
#define MAX_ERROR_COUNT 200
/* SW_MAJOR and MINOR needs to be updated from time to time to avoid warning message from AVR Studio */
/* never allow AVR Studio to do an update !!!! */
#define HW_VER	 0x02
#define SW_MAJOR 0x01
#define SW_MINOR 0x10

/* some variables */
union address_union {
	uint16_t word;
	uint8_t  byte[2];
} address;

union length_union {
	uint16_t word;
	uint8_t  byte[2];
} length;

struct flags_struct {
	unsigned eeprom : 1;
	unsigned rampz  : 1;
} flags;

uint8_t buff[256];
uint8_t address_high;

//#define app_start ((void(*)())0);
uint8_t error_count = 0;

uint8_t devId[]   = { 0x14,SIG1,SIG2,SIG3,0x10};
uint8_t pgmrId[]  = { 0x14,'A','V','R',' ','I','S','P',0x10};
uint8_t endStmt[] = { 0x14,0x10};
uint8_t failedStmt[] = { 0x14,0x11};



/* main program starts here */
void bootloader(void)
{
#ifdef ADABOOT
  char firstcharzero=0;
#endif
  uint8_t ch,ch2;
  uint16_t w;

  error_count = 0;
  initSerial();


  /* forever loop */
  for (;;) 
    {
    bitTgl(UsbLed);

      if (error_count >= MAX_ERROR_COUNT)  // Probably normal serial traffic is causing this.
        {
          releaseChipAccess();
          return;
          //initSerial();
          //error_count = 0;
        }

      /* get character from UART */
      ch = bootldrgetch();
      if (ch ==0)
        {
        releaseChipAccess();
        return;
        }
      /* A bunch of if...else if... gives smaller code than switch...case ! */

#if  0 //def ADABOOT
      // Hello is anyone home ?   lady ada hack -  BBR
      if(ch=='0') {
        firstcharzero = 1; // we got an appropriate bootloader instruction
        nothing_response();
      } 
	
      else if (firstcharzero == 0) {
        // the first character we got is not '0', lets bail!
        // autoreset via watchdog (sneaky!)
        // return();
      }	
#else

      /* Hello is anyone home ? */ 
      if(ch=='0') 
        {
          nothing_response();
	    }
#endif

      /* Request programmer ID */
      /* Not using PROGMEM string due to boot block in m128 being beyond 64kB boundry  */
      /* Would need to selectively manipulate RAMPZ, and it's only 9 characters anyway so who cares.  */
      else if(ch=='1') {
        if (bootldrgetch() == ' ') {
          putNch(pgmrId,9);
        } else {
          ++error_count;
        }
      }


      /* AVR ISP/STK500 board commands  DON'T CARE so default nothing_response */
      else if(ch=='@') {
        ch2 = bootldrgetch();
        if (ch2>0x85) bootldrgetch();
        nothing_response();
      }


      /* AVR ISP/STK500 board requests */
      /* Cmnd_STK_GET_PARAMETER 0x41 */
      else if(ch=='A') {
        ch2 = bootldrgetch();
        if(ch2==0x80) byte_response(HW_VER);		// Hardware version
        else if(ch2==0x81) byte_response(SW_MAJOR);	// Software major version
        else if(ch2==0x82) byte_response(SW_MINOR);	// Software minor version
        else if(ch2==0x98) byte_response(0x03);		// Unknown but seems to be required by avr studio 3.56
        else byte_response(0x00);				// Covers various unnecessary responses we don't care about
      }


      /* Cmnd_STK_SET_DEVICE 0x42
      Device Parameters  DON'T CARE, DEVICE IS FIXED  */
      else if(ch=='B') {
        getNch(20);
        nothing_response();
      }


      /* Cmnd_SET_DEVICE_EXT 0x45 */
      else if(ch=='E') {
        getNch(5);
        nothing_response();
      }

      /* P: Enter programming mode  */
      else if(ch=='P')
        {
		bitClr(BluLed);
		delay(200);
        if (!initChipAccess(&devId[1]))
          {
          failed_response();
          }
        else nothing_response();
		bitSet(BluLed);
        }
        
      /* R: Erase device, don't care as we will erase one page at a time anyway.  */
      else if(ch=='R')
        {
        eraseDevice();
        nothing_response();
        }


      /* Leave programming mode  */
      else if(ch=='Q') 
        {
        nothing_response();
        delay(5*mSec);
        releaseChipAccess();        
        allDone();
        return;
#ifdef WATCHDOG_MODS
        assert(0);
#endif
        }


      /* Set address, little endian. EEPROM in bytes, FLASH in words  */
      /* Perhaps extra address bytes may be added in future to support > 128kB FLASH.  */
      /* This might explain why little endian was used here, big endian used everywhere else.  */
      else if(ch=='U') {
        address.word = myget2LE();
        //address.byte[0] = bootldrgetch();
        //address.byte[1] = bootldrgetch();
        nothing_response();
      }


      /* Universal SPI programming command. Used for fuses and lock bits.  */
      else if(ch=='V') {
        unsigned char spicmdbuf[4];
        bootldrgetNch(spicmdbuf,4);   

        // Call my erase device embedded routine if the erase device SPI cmd comes in
        if ((spicmdbuf[0] == 0xac) && (spicmdbuf[1] == 0x80))
          {
          eraseDevice();
          byte_response(0);
          }
        // Don't let them program the fuse bits into a non-working clock, or turn off programming mode
        // This SPI on this chip can't go slow enough to work with very slow AVR clocks (<=1mhz)
        else if ((spicmdbuf[0] == 0xAC) && (spicmdbuf[1] == 0xA0) && (((spicmdbuf[3]&0x80) == 1) || ((spicmdbuf[3]&0x03) == 3)))
          {
		  failed_response();
		  for (ch=0;ch<10;ch++) 
		    { 
			bitClr(BluLed); bitSet(UsbLed); 
			delay(10*mSec); 
			bitSet(BluLed); bitClr(UsbLed);
			delay(10*mSec);
			}
		  ch='V';          
          }       
        else
          {
          byte_response(issueSpiCmd(spicmdbuf));
          }
      }


      /* Write memory, length is big endian and is in bytes  */
      else if(ch=='d') {
        length.word = myget2BE();
        //length.byte[1] = bootldrgetch();
        //length.byte[0] = bootldrgetch();
        flags.eeprom = 0;
        if (bootldrgetch() == 'E') flags.eeprom = 1;
        for (w=0;w<length.word;w++) 
          {
            buff[w] = bootldrgetch();	                        // Store data in buffer, can't keep up with serial data stream whilst programming pages
          }
        if (bootldrgetch() == ' ') 
          {
            if (flags.eeprom) {		                //Write to EEPROM one byte at a time
              address.word <<= 1;
              for(w=0;w<length.word;w++) {
                eeprom_write_byte((void *)address.word,buff[w]);
                address.word++;
              }			
            }
            else {					        //Write to FLASH one page at a time
              if (address.byte[1]>127) address_high = 0x01;	//Only possible with m128, m256 will need 3rd address byte. FIXME
              else address_high = 0x00;

              address.word = address.word << 1;	        //address * 2 -> byte location
              /* if ((length.byte[0] & 0x01) == 0x01) length.word++;	//Even up an odd number of bytes */
              //if ((length.byte[0] & 0x01)) length.word++;	//Even up an odd number of bytes
              if (length.word&1) length.word++; // Even up an odd number of bytes
              flash_write((void*)address.word, length.word, address_high, buff);
            }
            putNch(endStmt,2);
            //myputch(0x14);
            //myputch(0x10);
          } else {
          ++error_count;
        }		
      }


      /* Read memory block mode, length is big endian.  */
      else if(ch=='t') 
        {
        length.word = myget2BE();
        //length.byte[1] = bootldrgetch();
        //length.byte[0] = bootldrgetch();
#if defined(__AVR_ATmega128__)  || defined(__AVR_ATmega1280__)
        if (address.word>0x7FFF) flags.rampz = 1;		// No go with m256, FIXME
        else flags.rampz = 0;
#endif
        address.word = address.word << 1;	        // address * 2 -> byte location
        if (bootldrgetch() == 'E') flags.eeprom = 1;
        else flags.eeprom = 0;
        if (bootldrgetch() == ' ') 
          {		                // Command terminator
          myputch(0x14);
          if (1)
            {		        // Can handle odd and even lengths okay
            if (flags.eeprom) 
              { // Byte access EEPROM read
              for (w=0;w < length.word;w++,address.word++)
                {
                myputch(eeprom_read_byte((void *)address.word));
                }
              }
            else 
              {
              int bpos = 0;
              
              for (w=0;w < length.word;w++,address.word++,bpos++)
                {
                if (!flags.rampz) buff[bpos] = pgm_read_byte_near((void*)address.word);
#if defined(__AVR_ATmega128__) || defined(__AVR_ATmega1280__)
                else buff[bpos] = pgm_read_byte_far(address.word + 0x10000);
                // Hmmmm, yuck  FIXME when m256 arrvies
#endif
                if (bpos == 63)
                  {
                  putNch(buff,64);
                  bpos = -1;
                  }
                }
              if (bpos > 0)
                {
                  putNch(buff,bpos);
                }
              }
            }
          myputch(0x10);
          }
        }


      /* Get device signature bytes  */
      else if(ch=='u') {
        if (bootldrgetch() == ' ') {
          bitSet(UsbLed);
          putNch(devId,5);
        } else {
          ++error_count;
        }
      }


      /* Read oscillator calibration byte */
      else if(ch=='v') {
        byte_response(0x00);
      }


#ifdef MONITOR
      /* here come the extended monitor commands by Erik Lins */
      /* check for three times exclamation mark pressed */
      else if(ch=='!') monitor();
#endif
      else   /* Garbled 1st character */
        {
          //printf("garbled character %d %c\n",ch,ch);
          ++error_count;
        }

    } /* end of forever loop */

}


char gethexnib(void) 
{
	char a;
	a = bootldrgetch(); 
        myputch(a);
	if(a >= 'a') {
		return (a - 'a' + 0x0a);
	} else if(a >= '0') {
		return(a - '0');
	}
	return a;
}


char gethex(void) {
	return (gethexnib() << 4) + gethexnib();
}


void puthex(char ch) 
{
  char ah;

  ah = ch >> 4;
  if(ah >= 0x0a) {
    ah = ah - 0x0a + 'a';
  } else {
    ah += '0';
  }
	
  ch &= 0x0f;
  if(ch >= 0x0a) {
    ch = ch - 0x0a + 'a';
  } else {
    ch += '0';
  }
	
  if (1)
    {
      uint8_t c[2];
      c[0]=ah;
      c[1]=ch;
      putNch(c,2);
      //myputch(ah);
      //myputch(ch);
    }
}


void byte_response(uint8_t val)
{
  static uint8_t c[] = {0x14,0,0x10};
  if (bootldrgetch() == ' ') {
    c[1] = val;
    putNch(c,3);
  } else {
    ++error_count;
  }
}


void nothing_response(void)
{
  if (bootldrgetch() == ' ') {
    putNch(endStmt,2);
  } else {
    ++error_count;
  }
}

void failed_response(void)
{
  if (bootldrgetch() == ' ') {
    putNch(failedStmt,2);
  } else {
    ++error_count;
  }
}

#ifdef ICSPPGMR

#endif




#ifdef MONITOR
void monitor(void)
{
        ch = bootldrgetch();
        if(ch=='!') {
          ch = bootldrgetch();
          if(ch=='!') {

#if defined(__AVR_ATmega128__) || defined(__AVR_ATmega1280__)
            uint16_t extaddr;
#endif
            int i;
            uint8_t addrl, addrh;

#ifdef CRUMB128
            PGM_P welcome = {"ATmegaBOOT / Crumb128 - (C) J.P.Kyle, E.Lins - 050815\n\r"};
#elif defined PROBOMEGA128
            PGM_P welcome = {"ATmegaBOOT / PROBOmega128 - (C) J.P.Kyle, E.Lins - 050815\n\r"};
#elif defined SAVVY128
            PGM_P welcome = {"ATmegaBOOT / Savvy128 - (C) J.P.Kyle, E.Lins - 050815\n\r"};
#elif defined __AVR_ATmega1280__ 
            PGM_P welcome = {"ATmegaBOOT / Arduino Mega - (C) Arduino LLC - 090930\n\r" };
#endif

            /* turn on LED */
            LED_DDR |= _BV(LED);
            LED_PORT &= ~_BV(LED);

            /* print a welcome message and command overview */
            for(i=0; welcome[i] != '\0'; ++i) {
              myputch(welcome[i]);
            }

            /* test for valid commands */
            for(;;) {
              myputch('\n');
              myputch('\r');
              myputch(':');
              myputch(' ');

              ch = bootldrgetch();
              myputch(ch);

              /* toggle LED */
              if(ch == 't') {
                if(bit_is_set(LED_PIN,LED)) {
                  LED_PORT &= ~_BV(LED);
                  myputch('1');
                } else {
                  LED_PORT |= _BV(LED);
                  myputch('0');
                }
              } 

              /* read byte from address */
              else if(ch == 'r') {
                ch = bootldrgetch(); myputch(ch);
                addrh = gethex();
                addrl = gethex();
                myputch('=');
                ch = *(uint8_t *)((addrh << 8) + addrl);
                puthex(ch);
              }

              /* write a byte to address  */
              else if(ch == 'w') {
                ch = bootldrgetch(); myputch(ch);
                addrh = gethex();
                addrl = gethex();
                ch = bootldrgetch(); myputch(ch);
                ch = gethex();
                *(uint8_t *)((addrh << 8) + addrl) = ch;
              }

              /* read from uart and echo back */
              else if(ch == 'u') {
                for(;;) {
                  myputch(bootldrgetch());
                }
              }
#if defined(__AVR_ATmega128__) || defined(__AVR_ATmega1280__)
              /* external bus loop  */
              else if(ch == 'b') {
                myputch('b');
                myputch('u');
                myputch('s');
                MCUCR = 0x80;
                XMCRA = 0;
                XMCRB = 0;
                extaddr = 0x1100;
                for(;;) {
                  ch = *(volatile uint8_t *)extaddr;
                  if(++extaddr == 0) {
                    extaddr = 0x1100;
                  }
                }
              }
#endif

              else if(ch == 'j') {
                app_start();
              }

            } /* end of monitor functions */

          }
        }
      }
      /* end of monitor */

}
#endif


/* end of file ATmegaBOOT.c */
