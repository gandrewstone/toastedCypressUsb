#define FIFO_BUF_SIZE 64
typedef struct
{
  char buf[FIFO_BUF_SIZE];
  unsigned char start;
  unsigned char end;
} FifoBuf;

extern void fifoCtor(FifoBuf*ths);
extern void fifoPush(FifoBuf* ths, char c);
extern void fifoWrite(FifoBuf* ths, char* c, unsigned char len);
extern void fifoPushStr(FifoBuf* ths, char* s);
extern int fifoPop(FifoBuf* ths);  // returns 0xffff if buf empty
extern unsigned char fifoFull(FifoBuf*ths);
