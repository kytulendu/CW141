/*-----------------------------------------------------------------------*/
/*            Graphics module header file for CW 1.20                    */
/*                                                                       */
/*                     Suttipong Kanakakorn                              */
/*        Date    :        Sun  07-30-1989  16:01:13                     */
/*        Updated :                                                      */
/*-----------------------------------------------------------------------*/

#ifndef __CWGRPHC_H_
#define __CWGRPHC_H_

#ifdef  DEFINE_VAR
#define EXTERN
#else
#define EXTERN extern
#endif

#ifndef DEFINE_VAR
EXTERN int scrmode;
#else
EXTERN int scrmode=DETECT; /* not initialize (text) mode */
#endif

EXTERN unsigned int graph_seg;
EXTERN unsigned char *screen_buffptr;

/* ===================================================================== */

typedef void(Prchar_func)(unsigned c, unsigned attr, unsigned x, unsigned y);
typedef Prchar_func *Prchar_ptr;

EXTERN Prchar_ptr  prchar_ptr;
EXTERN Prchar_func hprchar, eprchar, aprchar;
/* same as
void(hprchar)(unsigned c, unsigned attr, unsigned x, unsigned y);
void(hprchar)(unsigned c, unsigned attr, unsigned x, unsigned y);
but more easier to manage and can get rid of compiler warning
*/

/* --------------------------------------------------------------------- */

typedef void (Prblank_func)(unsigned x, unsigned y);
typedef Prblank_func *Prblank_ptr;

EXTERN Prblank_func hprblank, eprblank, aprblank;
EXTERN Prblank_ptr  prblank_ptr;

/* --------------------------------------------------------------------- */

typedef void (Setcurpos_func)(unsigned x, unsigned y, int thaimode);
typedef Setcurpos_func *Setcurpos_ptr;

EXTERN Setcurpos_func hsetcurpos, esetcurpos, asetcurpos;
EXTERN Setcurpos_ptr  setcurpos_ptr;

/* --------------------------------------------------------------------- */

typedef void (Plot_func)(unsigned x, unsigned y);
typedef Plot_func *Plot_ptr;

EXTERN Plot_func hplot, eplot, aplot;
EXTERN Plot_ptr  plot_ptr;

/* --------------------------------------------------------------------- */

typedef void (Voidvoid_func)(void);
typedef Voidvoid_func *Voidvoid_ptr;

EXTERN Voidvoid_func hsetgraph, esetgraph, asetgraph,
                     hsettext, esettext, asettext;
EXTERN Voidvoid_func hsavepic, esavepic, asavepic,
                     hretpic, eretpic, aretpic;
EXTERN Voidvoid_func hclsall, eclsall, aclsall,
                     hprakeaw, eprakeaw, aprakeaw;

EXTERN Voidvoid_ptr  setgraph_ptr, settext_ptr, savepic_ptr, retpic_ptr;
EXTERN Voidvoid_ptr  clsall_ptr, prakeaw_ptr;

/* --------------------------------------------------------------------- */

typedef void (Clsgraph_func)(unsigned x1, unsigned x2,
                             unsigned y1, unsigned y2);
typedef Clsgraph_func *Clsgraph_ptr;
                               
EXTERN Clsgraph_func hclsgraph, eclsgraph, aclsgraph;
EXTERN Clsgraph_ptr  clsgraph_ptr;

/* --------------------------------------------------------------------- */

typedef void (Clrline_func)(unsigned x1, unsigned y1, unsigned x2);
typedef Clrline_func *Clrline_ptr;

EXTERN Clrline_func hclrline, eclrline, aclrline;
EXTERN Clrline_ptr  clrline_ptr;

/* --------------------------------------------------------------------- */

typedef void (RWchardot_func)(unsigned int x, unsigned int y);
typedef RWchardot_func *RWchardot_ptr;

EXTERN RWchardot_func hrdchardot, erdchardot, ardchardot,
                      hwrchardot, ewrchardot, awrchardot;
EXTERN RWchardot_ptr  rdchardot_ptr, wrchardot_ptr;

/* --------------------------------------------------------------------- */

typedef void (PGwind_func)(unsigned x1, unsigned x2,
                           unsigned linecount, unsigned bytecount,
                           char *buffer);
typedef PGwind_func *PGwind_ptr;

EXTERN PGwind_func hputwind, eputwind, aputwind, hgetwind, egetwind, agetwind;
EXTERN PGwind_ptr  putwind_ptr, getwind_ptr;

/* ===================================================================== */

#define prchar(c, attr, x, y)       (*prchar_ptr)(c, attr, x, y)

#define prblank(x, y)               (*prblank_ptr)(x, y)

#define setcurpos(x, y, thaimode)   (*setcurpos_ptr)(x, y, thaimode)

#define plot(x, y)                  (*plot_ptr)(x, y)

#define settext()                   (*settext_ptr)()

#define savepic()                   (*savepic_ptr)()

#define retpic()                    (*retpic_ptr)()

#define clsall()                    (*clsall_ptr)()

#define clsgraph(x1, y1, x2, y2)    (*clsgraph_ptr)(x1, y1, x2, y2);

#define clrline(x1, y1, x2)         (*clrline_ptr)(x1, y1, x2)

#define prakeaw()                   (*prakeaw_ptr)()

#define rdchardot (x, y)            (*rdchardot_ptr)(x,y)

#define wrchardot (x, y)            (*wrchardot_ptr)(x,y)

#endif /* __CWGRPHC_H_ */
