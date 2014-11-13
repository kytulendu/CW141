#include "cwtype.h"
#include "proto.h"

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

static int cusong_data[] = {
    CC,300,  A,300, CC,300,  E,300,  A,300,  G,300,  G,1800,
    CC,300,  A,300,  E,300,  A,300,  G,300,  D,300,  D,1800,
     E,300,  D,300,  E,300, Aa,600,  C,600,  D,1800,
     C,600,  D,900,  E,300,  G,1800,

    CC,300,  A,300, CC,300,  E,300,  A,300,  G,300,  G,1800,
    CC,300,  A,300,  E,300,  A,300,  G,300,  D,300,  D,1800,
     G,300,  E,300,  G,300,  D,600,  E,600,  G,1500,
    Gg,600, Aa,900,  D,300,  C,1800,

     A,600,  G,900,  A,300,  E,1800,
     A,600,  G,900,  E,300,  D,1800,
     E,300,  D,300,  E,300, Aa,600,  C,600,  D,1800,
     E,300,  D,300,  E,300, Gg,600, Aa,600,  C,1800,

    CC,300,  A,300, CC,300,  E,300,  A,300,  G,300,  G,1800,
    CC,300,  A,300,  E,300,  A,300,  G,300,  D,300,  D,1800,
     G,300,  E,300,  G,300,  D,600,  E,600,  G,1500,
    Gg,600, Aa,900,  D,300,  C,1800,

     A,600,  G,900,  A,300,  E,1800,
     A,600,  G,900,  E,300,  D,1800,
     E,300,  D,300,  E,300, Aa,600,  C,600,  D,1800,
     E,300,  D,300,  E,300, Gg,600, Aa,600,  C,1800,

    CC,300,  A,300, CC,300,  E,300,  A,300,  G,300,  G,1800,
    CC,300,  A,300,  E,300,  A,300,  G,300,  D,300,  D,1800,
     G,300,  E,300,  G,300,  D,600,  E,600,  G,1500,
     G,600,  A,900, CC,300, CC,1800,
};

void cusong(void)
{

    register int i;
    for (i = 0; !keypressed() && i < sizeof(cusong_data)/sizeof(int); i += 2) {
        soundnoise(cusong_data[i], cusong_data[i+1]);
    }
}
