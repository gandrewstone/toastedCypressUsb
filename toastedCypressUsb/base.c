#include "toastedLightuinoUsb.h"


#ifdef EMBED

unsigned char regShadow[5] = {0,0,0,0,0};

void pinMode(char port, char pinmask, char mode)
{
  M8C_DisableGInt;
  M8C_SetBank1;
  switch(port)
    {
    case 0:
      //if (mode&0x4) PRT0DM2 |= 1<<pinmask;
      //else PRT0DM2 &= ~(1<<pinmask);
      if (mode&0x2) PRT0DM1 |= pinmask;
      else PRT0DM1 &= ~pinmask;
      if (mode&0x1) PRT0DM0 |= pinmask;
      else PRT0DM0 &= ~pinmask;  
	  break;
    case 1:
      //if (mode&0x4) PRT1DM2 |= 1<<pinmask;
      //else PRT1DM2 &= ~(1<<pinmask);
      if (mode&0x2) PRT1DM1 |= pinmask;
      else PRT1DM1 &= ~pinmask;
      if (mode&0x1) PRT1DM0 |= pinmask;
      else PRT1DM0 &= ~pinmask;  
	  break;
    case 2:
      //if (mode&0x4) PRT2DM2 |= 1<<pinmask;
      //else PRT2DM2 &= ~(1<<pinmask);
      if (mode&0x2) PRT2DM1 |= pinmask;
      else PRT2DM1 &= ~pinmask;
      if (mode&0x1) PRT2DM0 |= pinmask;
      else PRT2DM0 &= ~pinmask;  
	  break;	  
  }
  M8C_SetBank0;
  M8C_EnableGInt;
}

#else
#ifdef SIM
uint8_t PRT0DR=0;
uint8_t PRT1DR=0;
uint8_t PRT2DR=0;

void pinMode(char port, char pinmask, char mode)
{
}


#endif
#endif
