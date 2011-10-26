#include "fifobuf.h"

void fifoCtor(FifoBuf*ths)
{
  ths->start=0;
  ths->end = 0;
}

unsigned char fifoFull(FifoBuf*ths)
{
  return (((ths->end + 1) % FIFO_BUF_SIZE) == ths->start);
}

void fifoPush(FifoBuf* ths, char c)
  {
  if ((ths->end + 1) % FIFO_BUF_SIZE != ths->start)
    {
      ths->buf[ths->end] = c;
      ths->end = (ths->end + 1) % FIFO_BUF_SIZE;
    }
  else
    {
      // Buffer full
    }
    //fifoWrite(ths,&c,1);
  }

void fifoWrite(FifoBuf* ths, char* c, unsigned char len)
{
  unsigned char i;
  for (i=0;i<len;i++)
    {
      fifoPush(ths,c[len]);
    }
#if 0
  while(len)  // Tail recursion
    {
      if (ths->tail>ths->head)  // The good data is between tail and head
        {
          unsigned char tillEnd = min(FIFO_BUF_SIZE-ths->tail,len);
          memcpy(buf+ths->tail,c,tillEnd);
          len-=tillEnd;
          c += tillEnd;
          ths->tail+=tillEnd;
          if (ths->tail>=FIFO_BUF_SIZE) ths->tail -= FIFO_BUF_SIZE;
         }
      else if (ths->tail < ths->head)  // The good data is from head to end, and 0 to tail.
        {
          unsigned char tillEnd = min(,len);
        }
      else
        {
        }
    }
#endif
}

void fifoPushStr(FifoBuf* ths, char* s)
  {
    while(*s!=0) { fifoPush(ths,*s); s++;}
  }

int fifoPop(FifoBuf* ths)
  {
  if (ths->end != ths->start)
      {       
      char temp = ths->buf[ths->start];
      ths->start = (ths->start + 1) % FIFO_BUF_SIZE;
      return(temp);
      }
      //otherwise, the buffer is empty; return an error code
  return 0xffff;  
  }