#include <stdlib.h>
#include <string.h>

#include "inc.h"

/* -------------------------------------------------------------
   Routine to set actual character width (character per inch).
   characters per inch (cpi) is stored in global int 'cpi'.
   the parameter is the buffer store dot data size[1920,3264].
   -------------------------------------------------------------
     ch/inch       dot/char        increasingdot    process(after/before)
        1            240                216          10/1
        2            120                 96           5/1
        3             80                 56          10/3
        4             60                 36           5/2
        5             48                 24           2/1
        6             40                 16           5/3
        7             34                 10          17/12
        8             30                  6           5/4
        9             27                  3           9/8
       ---------------------------------------------------
       10             24                  0          NONE
       ---------------------------------------------------
       11             22                - 2          11/12
       12             20                - 4           5/6
       13             18                - 6           3/4
       14             17                - 7          17/24
       15             16                - 8           2/3
       16             15                - 9          15/24
       17             14                -10           7/12
       18             13                -11          13/24
       19             12                -12           1/2
       20             12                -12           1/2
    ------------------------------------------------------------ */
/*
void waittest(void)
{
 while (ebioskey(1)==0);
}
*/

void charwidth(char buf1[])
{
     extern int maxdot; /* already defined in prt.c and pmenu.c */
     extern int maxbuffer; /* already defined in pmenu.c */
     extern int cpi   ; /* already defined in prt.c and pmenu.c */

     char *buf2;
     char cf[21];
     register int i,j,k,m;
     buf2 =(char *) calloc(maxdot+100,sizeof(char));
     i=0;
     j=0;
     switch (cpi){

     case  0 : break;

     case  1 :     /* term '10/cpi' generate number */
     case  2 :     /* of times the same dot appears */
     case  5 : while((i<maxbuffer) && (j<maxdot)){
                    k=j + 10/cpi;
                    for(;j<k;j++) buf2[j] = buf1[i];
                    i++;
               }
               memmove(buf1,buf2,maxdot);
               break;

     case  3 : while((i<maxbuffer) && (j<maxdot)){
                    k=j + 3;  /* 3 times repeat */
                    for(;j<k;j++) buf2[j] = buf1[i];
                    i++;
                    k=j + 3;  /* 3 times repeat */
                    for(;j<k;j++) buf2[j] = buf1[i];
                    i++;
                    k=j + 4;  /* 4 times repeat */
                    for(;j<k;j++) buf2[j] = buf1[i];
                    i++;
               }
               memmove(buf1,buf2,maxdot);
               break;

     case  4 : while((i<maxbuffer) && (j<maxdot)){
                    k=j + 2;  /* 2 times repeat */
                    for(;j<k;j++) buf2[j] = buf1[i];
                    i++;
                    k=j + 3;  /* 3 times repeat */
                    for(;j<k;j++) buf2[j] = buf1[i];
                    i++;
               }
               memmove(buf1,buf2,maxdot);
               break;

     case  6 : while((i<maxbuffer) && (j<maxdot)){
                    k=j + 2;  /* 2 times repeat */
                    for(;j<k;j++) buf2[j] = buf1[i];
                    i++;
                    k=j + 2;  /* 2 times repeat */
                    for(;j<k;j++) buf2[j] = buf1[i];
                    i++;
                    k=j + 1;  /* 1 times repeat */
                    for(;j<k;j++) buf2[j] = buf1[i];
                    i++;
               }
               memmove(buf1,buf2,maxdot);
               break;

     case  7 : while((i<maxbuffer) && (j<maxdot)){
                    for(m=0;m<5;m++){
                       k=i + 1;  /* 1 times iteration */
                       for(;i<k;) buf2[j++] = buf1[i++];
                       buf2[j++] = buf1[i];
                       buf2[j++] = buf1[i++]; /* 1 dot repeat */
                    }
                    buf2[j++] = buf1[i++];
                    buf2[j++] = buf1[i++];
               }
               memmove(buf1,buf2,maxdot);
               break;

     case  8 : /* term '3+4*(cpi-8)' refer to 3,7 accordingly */
     case  9 : while((i<maxbuffer) && (j<maxdot)){
                    k=i + 3+4*(cpi-8);  /* 3,7 times iteration */
                    for(;i<k;) buf2[j++] = buf1[i++];
                    buf2[j++] = buf1[i];
                    buf2[j++] = buf1[i++]; /* 1 dot repeat */
               }
               memmove(buf1,buf2,maxdot);
               break;

     case 10 : break;

     case 11 : cf[11]=12;  /* terms (12/-576)(cpi-12)(cpi-13)(...)...(...)... */
     case 12 : cf[12]=6;  /* +     ( 6/ 168)(cpi-11)(cpi-13)(...)...(...)... */
     case 13 : cf[13]=4;  /* +     ...                                       */
     case 15 : cf[15]=3;  /* +     ( 2/1520)(cpi-11)(cpi-12)..(..)..(cpi-19) */
     case 19 : cf[19]=2;  /* refer to 12,6,4,3,2,2 accordingly               */
     case 20 : cf[20]=2;
/*
               while((i<maxbuffer) && (j<maxdot)){
                    k=i-1+cf[cpi];
                    for(;i<k;) buf2[j++] = buf1[i++];
                    j--;
                    buf2[j++]|=buf1[i++]; /* 1 dot deplete */
               }
*/
               while((i<maxbuffer) && (j<maxdot)){
                    k=i-1+cf[cpi];
                    for(;i<k;) buf2[j++]|= buf1[i++];
                    j--;
                    buf2[j++]|=buf1[i];
                    buf2[j]|=buf1[i++]; /* 1 dot deplete */
               }
               memmove(buf1,buf2,maxdot);
               break;
     case 14 : while((i<maxbuffer) && (j<maxdot)){
                    for(m=0;m<7;m++){
                       k = i+2;
                       for(;i<k;) buf2[j++] = buf1[i++];
                       i++;  /* ignore 1 dot of 3 */
                    }
                    k = i+3;
                    for(;i<k;) buf2[j++] = buf1[i++];
               }
               memmove(buf1,buf2,maxdot);
               break;
     case 16 : while((i<maxbuffer) && (j<maxdot)){
                    for(m=0;m<7;m++){
                       k = i+2;
                       for(;i<k;) buf2[j++] = buf1[i++];
                       i++;  /* ignore 1 dot of 3 */
                    }
                    i++; i++;
                    buf2[j++] = buf1[i++];
               }
               memmove(buf1,buf2,maxdot);
               break;
     case 17 : while((i<maxbuffer) && (j<maxdot)){
                    for(m=0;m<5;m++){
                       k = i+1;	
                       for(;i<k;) buf2[j++] = buf1[i++];
                       i++;  /* ignore 1 dot of 2 */
                    }
                    k = i+2;
                    for(;i<k;) buf2[j++] = buf1[i++];
               }
               memmove(buf1,buf2,maxdot);
               break;
     case 18 : while((i<maxbuffer) && (j<maxdot)){
                    for(m=0;m<11;m++){
                       k = i+1;
                       for(;i<k;) buf2[j++] = buf1[i++];
                       i++;  /* ignore 1 dot of 2 */
                    }
                    k = i+2;
                    for(;i<k;) buf2[j++] = buf1[i++];
               }
               memmove(buf1,buf2,maxdot);
               break;
     default : break;
     }
  free(buf2);
}
