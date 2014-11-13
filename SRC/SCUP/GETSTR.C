/******  getstring *******/
/* extract from pmenu.c  similar to getstr.c in prjcuw\scuw */
/* Updated: Add function prototype
    By Suttipong Kanakakorn Mon  08-07-1989  00:26:14    */

#include <string.h>
#include "..\common\kbdcode.h"
#include "inc.h"
#include "global.ext"

/*****************************************************************************/
/* get string                                                                */
/* input                                                                     */
/*      textst : string want to get                                          */
/*      x      : vertical position ( 0 - 89 )                                */
/*      y      : horisontal position ( 0 - 16 )                              */
/*      maxlen : maximum length of string                                    */
/*      attr   : attribute of string for displaying                          */
/*****************************************************************************/
int getstring(char textst[], int x, int y, int maxlen, font_attr attr)
{
 int inkey,oldlen,temp;
 char keepchar;
 keepchar = '\0';
 oldlen = strlen(textst);
 dispblank(x,y,maxlen,attr);
 dispstrhgc(textst,x,y,attr);
 waitkbd(x+thaistrlen(textst),y);
 inkey = readkbd();
 switch (inkey){
 case CNTRL_H  :
 case BSKEY    :
 case CNTRL_S  :
 case LEKEY    :
 case CNTRL_M  :
 case RETKEY   : break;
 default       : textst[0] = '\0';
                 dispblank(x,y,maxlen,attr);
                 break;
 }
 do{
   switch(inkey){
   case CNTRL_M :
   case RETKEY  : return(YES);
   case CNTRL_U : return(NO);              /* Abort */
   case CNTRL_H :
   case BSKEY   :
   case LEKEY   :
   case CNTRL_S : temp = strlen(textst);
                   if (temp != 0){
                      if (temp < oldlen)
                         textst[temp] = keepchar;
                      keepchar = textst[temp-1];
                      textst[temp-1] = '\0';
                      dispblank(x,y,maxlen,attr);
                      dispstrhgc(textst,x,y,attr);
                   }
                   break;
   case ESCKEY  : return(ESCKEY);
   case RIKEY   :
   case CNTRL_D : temp = strlen(textst);
                  if ((temp < oldlen) && (temp < maxlen)){
                     textst[temp] = keepchar;
                     keepchar = textst[temp+1];
                     textst[temp+1] = '\0';
                     dispblank(x,y,maxlen,attr);
                     dispstrhgc(textst,x,y,attr);
                  }
                  break;
   case CNTRL_Y : if (strlen(textst) < oldlen)
                     textst[strlen(textst)] = keepchar;
                  keepchar = textst[0];
                  textst[0] = '\0';
                  dispblank(x,y,maxlen,attr);
                  break;
   case CNTRL_R : if (strlen(textst) < oldlen){
                     textst[strlen(textst)] = keepchar;
                     dispblank(x,y,maxlen,attr);
                     dispstrhgc(textst,x,y,attr);
                  }
                  break;
   case F10KEY  : if ( thaimode == YES )
                     thaimode = NO;
                  else
                     thaimode = YES;
                  break;
   default      : inkey = (inkey & 0xff);
                  if (inkey >= 32){
                     if (strlen(textst) < maxlen){
                        if (whatlevel(inkey) == MIDDLE){
                           textst[strlen(textst)+1] = '\0';
                           textst[strlen(textst)] = inkey;
                           oldlen = strlen(textst);
                           dispblank(x,y,maxlen,attr);
                           dispstrhgc(textst,x,y,attr);
                        }
                        else{
                           if ((strlen(textst) != 0) &&
                               (whatlevel(inkey) >
                                whatlevel(textst[strlen(textst)-1]))){
                              textst[strlen(textst)+1] = '\0';
                              textst[strlen(textst)] = inkey;
                              oldlen = strlen(textst);
                              dispblank(x,y,maxlen,attr);
                              dispstrhgc(textst,x,y,attr);
                           }
                           else
                              errorsound();
                        }
                     }
                  }
                  break;
   }
   waitkbd(x+thaistrlen(textst),y);
   inkey = readkbd();
 } while(1);
}

int getname(char textst[], int x, int y, int maxlen, font_attr attr)
{
     int inkey,oldlen,temp;
     char keepchar;
     keepchar = '\0';
     oldlen = strlen(textst);
     dispblank(x,y,maxlen,attr);
     dispstrhgc(textst,x,y,attr);
     waitkbd(x+thaistrlen(textst),y);
     inkey = ebioskey(0);
     switch (inkey){
            case BSKEY    :
            case CNTRL_H  :
            case CNTRL_S  :
            case LEKEY    :
            case CNTRL_M  :
            case RETKEY   : break;
            default       : textst[0] = '\0';
                            dispblank(x,y,maxlen,attr);
                            break;
     }
     do
     {
       switch(inkey){
       case RETKEY  :
       case CNTRL_M : return(YES);
       case CNTRL_U : return(NO);                 /* Abort */
       case BSKEY   :
       case LEKEY   :
       case CNTRL_H :
       case CNTRL_S : temp = strlen(textst);
                      if (temp != 0){
                         if (temp < oldlen)
                            textst[temp] = keepchar;
                         keepchar = textst[temp-1];
                         textst[temp-1] = '\0';
                         dispblank(x,y,maxlen,attr);
                         dispstrhgc(textst,x,y,attr);
                      }
                      break;
       case ESCKEY  : return(ESCKEY);
       case RIKEY   :
       case CNTRL_D : temp = strlen(textst);
                      if ((temp < oldlen) && (temp < maxlen)){
                         textst[temp] = keepchar;
                         keepchar = textst[temp+1];
                         textst[temp+1] = '\0';
                         dispblank(x,y,maxlen,attr);
                         dispstrhgc(textst,x,y,attr);
                      }
                      break;
       case CNTRL_Y : if (strlen(textst) < oldlen)
                         textst[strlen(textst)] = keepchar;
                      keepchar = textst[0];
                      textst[0] = '\0';
                      dispblank(x,y,maxlen,attr);
                      break;
       case CNTRL_R : if (strlen(textst) < oldlen){
                         textst[strlen(textst)] = keepchar;
                         dispblank(x,y,maxlen,attr);
                         dispstrhgc(textst,x,y,attr);
                      }
                      break;
       default      : inkey = (inkey & 0xff);
                      if (inkey >= 32){
                         if ((inkey >= 'a') && (inkey <= 'z'))
                            inkey = inkey - ('a' - 'A');
                         if ((strlen(textst) < maxlen) && (inkey != ' ')){
                            textst[strlen(textst)+1] = '\0';
                            textst[strlen(textst)] = inkey;
                            oldlen = strlen(textst);
                            dispblank(x,y,maxlen,attr);
                            dispstrhgc(textst,x,y,attr);
                         }
                      }
                      break;
       }
       waitkbd(x+thaistrlen(textst),y);
       inkey = ebioskey(0);
     } while(1);
}

int getnumber(char textst[], int x, int y, int maxlen, font_attr attr)
{
     int inkey,oldlen,temp;
     char keepchar;
     keepchar = '\0';
     oldlen = strlen(textst);
     dispblank(x,y,maxlen,attr);
     dispstrhgc(textst,x,y,attr);
     waitkbd(x+thaistrlen(textst),y);
     inkey = ebioskey(0);
     switch (inkey){
            case BSKEY    :
            case CNTRL_H  :
            case CNTRL_S  :
            case CNTRL_M  :
            case RETKEY   :
            case LEKEY    :  break;
            default       :  textst[0] = '\0';
                             dispblank(x,y,maxlen,attr);
                             break;
     }
     do
     {
       switch(inkey){
       case RETKEY  :
       case CNTRL_M : return(YES);
       case CNTRL_U : return(NO);
       case BSKEY   :
       case LEKEY   :
       case CNTRL_H :
       case CNTRL_S : temp = strlen(textst);
                      if (temp != 0){
                         if (temp < oldlen)
                            textst[temp] = keepchar;
                         keepchar = textst[temp-1];
                         textst[temp-1] = '\0';
                         dispblank(x,y,maxlen,attr);
                         dispstrhgc(textst,x,y,attr);
                      }
                      break;
       case ESCKEY  : return(ESCKEY);
       case RIKEY   :
       case CNTRL_D : temp = strlen(textst);
                      if ((temp < oldlen) && (temp < maxlen)){
                         textst[temp] = keepchar;
                         keepchar = textst[temp+1];
                         textst[temp+1] = '\0';
                         dispblank(x,y,maxlen,attr);
                         dispstrhgc(textst,x,y,attr);
                      }
                      break;
       case CNTRL_Y : if (strlen(textst) < oldlen)
                         textst[strlen(textst)] = keepchar;
                      keepchar = textst[0];
                      textst[0] = '\0';
                      dispblank(x,y,maxlen,attr);
                      break;
       case CNTRL_R : if (strlen(textst) < oldlen){
                         textst[strlen(textst)] = keepchar;
                         dispblank(x,y,maxlen,attr);
                         dispstrhgc(textst,x,y,attr);
                      }
                      break;
       default      : inkey = (inkey & 0xff);
                      if ((inkey >= '0') && (inkey <= '9')){
                         if (strlen(textst) < maxlen){
                            textst[strlen(textst)+1] = '\0';
                            textst[strlen(textst)] = inkey;
                            oldlen = strlen(textst);
                            dispblank(x,y,maxlen,attr);
                            dispstrhgc(textst,x,y,attr);
                         }
                      }
                      break;
       }
       waitkbd(x+thaistrlen(textst),y);
       inkey = ebioskey(0);
     } while(1);
}
