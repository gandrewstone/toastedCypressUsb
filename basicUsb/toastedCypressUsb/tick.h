#ifndef TICKH
#define TICKH

#include "board.h"

// The current count
extern volatile uint32_t tick;

// Start counting
extern void tickStart(void);

// Pass the time in ticks; use the mSec and Sec macros to convert to ticks...
// For example delay(10*mSec);
extern void delay(uint32_t time);

// Turn on a led, wait the time, turn it off.
extern void blinkLed(uint8_t reg, uint8_t msk, uint32_t time,uint16_t count);

// Return the difference between now and some start time
uint32_t delta(uint32_t start, uint32_t now);

// The ISR
extern void toastedGpioIsr(void);

#endif
