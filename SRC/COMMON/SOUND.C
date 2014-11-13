/****************************************************************************/
/*  SOUND.C 15 JAN 89                                                       */
/*  Updated : Suttipong Kanakakorn
              Sun  08-06-1989  18:30:55
              Mon  10-23-1989  23:18:49                                     */
/****************************************************************************/
#include <bios.h>
#include <dos.h>

#define CC 262*2
#define DD 294*2
#define EE 330*2
#define FF 349*2
#define GG 392*2
#define AA 440*2
#define BB 494*2
#define C 262
#define D 294
#define E 330
#define F 349
#define G 392
#define A 440
#define B 494
#define Cc 262/2
#define Dd 294/2
#define Ee 330/2
#define Ff 349/2
#define Gg 392/2
#define Aa 440/2
#define Bb 494/2

#ifdef NOSOUND

void soundnoise(unsigned freq,unsigned time) {}
void errorsound(void) {}

#else

void biosdelay(long millisecs);

void soundnoise(unsigned freq,unsigned time)
{
    sound(freq);
    biosdelay((long)time);
    nosound();
}

void errorsound(void)
{
    /* soundnoise(C*2,300); */
    soundnoise(C,40);
    soundnoise(D,40);
    soundnoise(E,40);
    soundnoise(F,40);
    soundnoise(G,40);
    soundnoise(A,40);
    soundnoise(B,40);
    soundnoise(C*2,40);
}

/* we can not use delay() because it call dos, so it is not reentrant */

void biosdelay(long millisecs)
{
    long i;

    i = biostime(0, 0);
    millisecs = i + 182L * millisecs / 10000L;
    if (millisecs == i) millisecs++;
    do {
	i = biostime(0, 0);
    } while (i < millisecs);
}

#endif
