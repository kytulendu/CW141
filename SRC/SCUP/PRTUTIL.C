/* Written by Suttipong Kanakakorn Tue  08-29-1989  18:25:25 */
#include "inc.h"
#include "..\common\kbdcode.h"
#include "..\common\cwgrphc.h"

extern int lineperpage;
extern int linespace;
extern int userlineperpage;
extern int pagelength;
extern int printer24pin;
extern int graphicprint;
extern int locpagetitle;
extern int locheading;
extern int locfooting;
extern int extbarprinting;

int find_line_space(void)
{
    register unsigned i, j, k;
/*  [ i = Feeding resolution ( Dots / Inch )     ]
    [ j = Minimum number of dots need for 1 line ]
    [ k = linespace                              ]
*/
    i = (printer24pin) ? 180 : 216;
    if (graphicprint) {
        j = (printer24pin) ? IPL_GRPH_24 : IPL_GRPH_9;
    } else {
        j = (printer24pin) ? IPL_TEXT_24 : IPL_TEXT_9;
    }
    k = (pagelength-1) * i / lineperpage - j;
    extbarprinting = (k>j/2);
    return k;
}

int find_line_perpage(void)
{
    return userlineperpage +
        ((locpagetitle != 0) + (locheading != 0) + (locfooting != 0));
}

int find_max_line_perpage(void)
{
    register int i;
    if (graphicprint)
         i = (printer24pin) ? ML_GRPH_24 : ML_GRPH_9;
    else
         i = (printer24pin) ? ML_TEXT_24 : ML_TEXT_9;
    return i;
}

int find_max_userline_perpage(void)
{
    return find_max_line_perpage() -
        ((locpagetitle != 0) + (locheading != 0) + (locfooting != 0));
}

void set_all_lineperpage(set_linespace_mode mode)
{
    char *find_lineperinch(void);

    static char m1[] = "จำนวนบรรทัดต่อหน้าไม่พอ "
                      "เครื่องจะเปลี่ยนให้เป็นจำนวนสูงสุดเท่าที่จะยอมได้";
    static char m2[] = "กดปุ่ม ESC เพื่อทำงานต่อไป";

    if (userlineperpage > find_max_userline_perpage()) {
        userlineperpage = find_max_userline_perpage();
        if (mode == INTERACTIVE) {
            savepic();
            framebox(10, 8, 10+thaistrlen(m1)+4, 11, REVERSEATTR);
            dispstrhgc(m1, 10+2, 9, REVERSEATTR);
            dispstrhgc(m2, 10+ (thaistrlen(m1)+4 - thaistrlen(m2)) / 2,
                       10, REVERSEATTR);
            while (ebioskey(0) != ESCKEY) ;
            retpic();
            writemenu(0, 1, 0); /* show new userline per page */
        }
    }
    lineperpage = find_line_perpage();
    if (mode == INTERACTIVE) {
        savepic();        /*"                             "*/
        dispprintf(19,12,0,
                   "จำนวนบรรทัดต่อกระดาษ 1 นิ้ว คือ : %s บรรทัด/นิ้ว",
                   find_lineperinch());
        ebioskey(0);
        retpic();
    }
}

/* feed n/216 inche or n/180 inch depend on printer, n can be any integer */
void line_feed(unsigned n)
{
    register int i, j;

    j = (printer24pin) ? 180 : 216;
    i = n / j;

    /* optimize by feed 1 inch */
    for (; i>0 ; i--)
        PrinterLineFeed(j);
    PrinterLineFeed(n % j);
}

/* skip n lines */
void skip_line(unsigned n)
{
    for (; n>0; n--) {
        if (graphicprint)
            line_feed((printer24pin) ? IPL_GRPH_24+linespace
                                    : IPL_GRPH_9+linespace);
        else
            line_feed((printer24pin) ? IPL_TEXT_24+linespace
                                    : IPL_TEXT_9+linespace);
    }
}

char *find_lineperinch(void)
{
    static char x[7];
    register i;

    sprintf(x, "%4d", (unsigned) lineperpage * 1000 / pagelength);
    for (i = 0; i < 4; i++) {
        if (x[i] != '0') {
            movmem(&x[i+1], &x[i+2], 4-i);
            x[i+1] = '.';
            break;
        }
    }
    return x;
}
