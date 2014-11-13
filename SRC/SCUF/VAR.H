/****************************************************************************/
/*  VAR.C 15 JAN 89                                                         */
/*  var.c combine with var.h By Noom Suttipong Sun  07-30-1989  15:37:35    */
/*  Updated: Suttipong Kanakakorn                                           */
/*         : Mon  07-31-1989  20:13:05                                      */
/****************************************************************************/

#ifndef __VAR_H_
#define __VAR_H_

#ifdef  DEFINE_VAR
#define EXTERN
#else
#define EXTERN extern
#endif

/* =================================================================== */
/* First part can define and declare with the same attribute           */


EXTERN char dfont[20 * 256];
EXTERN char ditalicfont[1];

EXTERN char *progname;   /* point to argv[0] */
EXTERN char cuf_dir[MAXDRIVE+MAXDIR];  /* which directory is cw.exe in */

/* End first part                                                       */
/* ==================================================================== */
/* Second part                                                          */
/* We can't init extern variables , just declare it with extern         */
/* We'll define it in one place only                                    */

#ifndef DEFINE_VAR

/* Just declare these variables only */

EXTERN char protect1;
EXTERN char protect2;
EXTERN boolean thaimode;    /*  current mode is thai or eng */

#else
/* EXTERN will be declared as null already */

EXTERN char protect1 = 0x00;
EXTERN char protect2 = 0x00;
EXTERN boolean thaimode = YES;    /*  current mode is thai or eng */

#endif

#undef EXTERN /* restore it */

#endif /* __VAR_H */
