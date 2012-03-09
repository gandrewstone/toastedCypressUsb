#include "board.h"

#ifdef EMBED
#include "Timer16.h"
#endif

#include "tick.h"

volatile uint32_t tick=0;

// Return the difference between now and some start time
uint32_t delta(uint32_t start, uint32_t now)
{
  uint32_t tmp = now-start;
  if (start < now) return tmp;
  else
  {
    return 0xFFFFFFFFUL - tmp;
  }
}

#ifdef EMBED

#pragma interrupt_handler Timer16_ISR;  
void              	Timer16_ISR( void )
{ 
  // When entering an ISR, interrupts are disabled, and the bank is set to 0
  // When leaving prior state is restored.

  //M8C_SetBank0;
  //if (INT_CLR0 & 0x8) -- no its not pending is happening now!
  tick++;
  //M8C_SetBank0;
  // Toggle RED LED: 
  //if ((IrInputReg&IrInputMsk)>0)
  //  RedLedReg = RedLedReg | RedLedMsk;
  //else RedLedReg = RedLedReg & ~RedLedMsk;
  
  //if ((tick&1023) ==0) RedLedReg = RedLedReg ^ RedLedMsk;
}
void tickStart(void)
{
    M8C_EnableGInt;
	Timer16_SetMode(0);
	Timer16_SetPeriod(TICK_COUNT);
    Timer16_EnableInt();
	Timer16_Start();
}



void blinkLed(uint8_t reg, uint8_t msk, uint32_t time,uint16_t count)
{
    unsigned int i;
    for (i=0;i<count;i++)
        {
//GAS to do
        delay(time);   

        delay(time);
        }
}
        
void delay(uint32_t time)
  {
  uint32_t until = time + tick;
  while (tick < until) {}
  }
#else
#ifdef SIM
void tickStart(void) 
{
}


void blinkLed(uint8_t led, uint32_t time,uint16_t count)
{
}

void delay(uint32_t time)
  {
  }

#endif

#endif
