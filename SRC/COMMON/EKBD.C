/* ekbd.c : extended keyboard routine
    Written by Suttipong Kanakakorn
    Thu  08-03-1989  16:59:05
ebioskey = encapsulate bioskey for future expanded like macro keyboard
placekey(k) place scancode k to ebioskey keyboard buffer
keypressed() check whether key is pressed or not
*/

#include <bios.h>
#include "cwtype.h"
#include "proto.h"

extern boolean thaimode;

#define KBUFSIZE 128
static int ebioskey_buf[KBUFSIZE];
static int keyb_head = 0;
static int keyb_tail = 0;

int ebioskey(register int cmd)
{
    register int keycode;
    if (cmd == 0) {
        if (keyb_head == keyb_tail)
            return (bioskey(0));
        keycode = ebioskey_buf[keyb_head++];
        if (keyb_head == KBUFSIZE)
            keyb_head = 0;
        return(keycode);
    }
    return (bioskey(cmd));
}

void placekey(register int k)
{
    ebioskey_buf[keyb_tail++] = k;
    if (keyb_tail == KBUFSIZE)
        keyb_tail = 0;
}

boolean keypressed(void)
{
    if (keyb_head == keyb_tail)
        return(asm_keypressed());
    return(YES);
}

/*   Pointer version
#define KBUFSIZE 128
int ebioskey_buf[KBUFSIZE];
int *keyb_head = ebioskey_buf;
int *keyb_tail = ebioskey_buf;

int ebioskey(register int cmd)
{
    register int keycode;
    if (cmd == 0) {
        if (keyb_head == keyb_tail)
            return (bioskey(0));
        keycode = *keyb_head++;
        if (keyb_head == ebioskey_buf + KBUFSIZE)
            keyb_head = ebioskey_buf;
        return(keycode);
    }
    return (bioskey(cmd));
}

void placekey(register int k)
{
    *keyb_tail++ = k;
    if (keyb_tail == ebioskey_buf + KBUFSIZE)
        keyb_tail = ebioskey_buf;
}
*/
