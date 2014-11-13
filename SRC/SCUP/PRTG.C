/* Updated: Suttipong Kanakakorn Tue  08-08-1989  23:50:08 */
/* Group constants in a file, add function prototype */

#include <stdlib.h>
#include "inc.h"
#include "global.ext"

/* function prototype */
void print_picture(char *point);
void PrintBuffer9pin(unsigned char c[]);
void PrintThree9pin(void);
int ltrim(unsigned char line[], int left, int right);
int rtrim(unsigned char line[], int left, int right);

/*  constant   area   */
#define  WIDTH       24
#define  ADJUST(x) (x>=0) ? x : 256+x
#define  ASCII_NO   256

/*
void print_picture(char *point)
{
 extern int  grp_ready;
 extern char *print_buffer_pointer;
 grp_ready=NO;
 print_buffer_pointer=point;
 read_picture_file();
}
*/

/* ---------------------------------------------
   function PrintBuffer9pin().print  data  in buffer.
   include left-right trim  functions to  fasten
   graphic print. In text mode the printer print
   nothing when it reaches blank (' ').
   -------------------------------------------- */
void PrintBuffer9pin(unsigned char c[])
{
      extern int maxdot ;
      register int i;
      int left,right,length;
      charwidth(c)      ; /* alternate character width in cpi.obj */
      left = ltrim(c,0,maxdot);
      right = rtrim(c,0,maxdot);
      for(i=0; i<left; i++)
         putp(' ');       /* text mode */
      length = right - (left * 24)  ;
      if(length != 0){
         PrinterSetGraphicMode9pinQuadrupleDensity(length);
         for(i=0;i< length ; i++) {
            putp(c[(left * 24) + i]);   /* graphic (bit image) print */
         }
      }
}

/* modify by Suttipong Kanakakorn Tue  08-29-1989  13:08:36 */
void PrintThree9pin(void)
{
    extern  int  linespace ;  /*  FX    : 24     */
                              /*  LX,LQ : 20     */
    extern  int  nlqmode   ;
    extern  int  pic_print ;
    extern  int  grp_ready ;
    extern  int  dot_per_line;
    extern  int  maxdot;
    extern  char *bupper1;
    extern  char *bupper2;
    extern  char *bmiddle1;
    extern  char *bmiddle2;
    extern  char *bbelow1;
    extern  char *bbelow2;
    extern  char *print_buffer_pointer;
    char    *print_pointer[6];
    char    *temp_buffer;
    int     i,j,spleft;
    print_pointer[0]=bupper1;
    print_pointer[1]=bupper2;
    print_pointer[2]=bmiddle1;
    print_pointer[3]=bmiddle2;
    print_pointer[4]=bbelow1;
    print_pointer[5]=bbelow2;
    if (nlqmode == 1) {
       for (i=0;i<6;i++) {
           if (pic_print==NO) {
              PrintBuffer9pin(print_pointer[i]);
              if ((i%2)==0)
                PrinterLineFeed216inch(1);
              else {
                if (i==5) {                       /* After finish line */
                  if (extbarprinting) {
                    spleft=linespace;
                    while(spleft > 24) {
                      PrinterLineFeed216inch(23);
                      PrintBuffer9pin(extbar1);     /* More table line */
                      PrinterLineFeed216inch(1);
                      PrintBuffer9pin(extbar2);
                      spleft-=24;
                    }
                    PrinterLineFeed216inch(spleft);
                  } else
                    PrinterLineFeed216inch(linespace);
                } else
                  PrinterLineFeed216inch(23);
              }
           } /* picprint == yes */ /*
           else {
              switch (dot_per_line) {
                 case 1:
                      if (i%2==0) {
                         PrintBuffer9pin(print_pointer[i]);
                         PrinterLineFeed216inch(1);
                      }
                      else {
                         /* print_picture(print_pointer[i]); */
                         PrintBuffer9pin(print_pointer[i]);
                         PrinterLineFeed216inch(23);
                      }
                      break;
                 case 2:
                      print_picture(print_pointer[i]);
                      PrintBuffer9pin(print_pointer[i]);
                      if ((i%2)==0)
                         PrinterLineFeed216inch(1);
                      else
                         PrinterLineFeed216inch(23);
                      break;
                 case 3:
                      print_picture(print_pointer[i]);
                      PrintBuffer9pin(print_pointer[i]);
                      switch (i%2) {
                         case 0:
                              PrinterLineFeed216inch(1);
                              break;
                         case 1:
                              PrinterLineFeed216inch(1);
                              temp_buffer=(char *)calloc(6600,sizeof(char));
                              print_picture(temp_buffer);
                              PrintBuffer9pin(temp_buffer);
                              PrinterLineFeed216inch(22);
                              free(temp_buffer);
                              break;
                      }
                      break;
              }
           } */
       }
    }
    else {
       for (i=1;i<6;i+=2) {
           for (j=0;j<maxdot;j++)
               *(print_pointer[i]+j)|=*(print_pointer[i-1]+j);
           /* if ((pic_print==YES) && (dot_per_line==1))
              print_picture(print_pointer[i]); */
           PrintBuffer9pin(print_pointer[i]);
           if (i == 5)
              PrinterLineFeed216inch(linespace);
           else
              PrinterLineFeed216inch(24);
       }
    }
}

int ltrim(unsigned char line[], int left, int right)
{
     register int i    ;
     i = left ;
     while( line[i] == 0){
          i++ ;
          if (i >= right-1)
             return(left);
     }
     return((int)(i / 24));
}

int rtrim(unsigned char line[], int left, int right)
{
     register int i      ;
     i = right-1;
     while( line[i] == 0){
          i-- ;
          if(i <= left)
          return(left) ;
     }
     return(i+1);
}
