/* Updated: Suttipong Kanakakorn Wed  08-09-1989  00:05:24 */
/* add function prototype */

#include <io.h>
#include <dir.h>
#include <dos.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "inc.h"

/* function prototype */
int is_command(char st[]);
void get_argument(char arg[][40],char st[],int *no_arg);
int getpr(void);
void get_graphic_data(void);
void process_command_0(int printer_data);
void process_command_1(int printer_data);
void process_command(int printer_data);
void analyze_picture_file(char *st);
void initialize_read(char *st);
void read_picture_file(void);

char *print_buffer_pointer;
int pic_print;
int pic_offset;
int prd_handle;
int grp_ready;
int p_fileready;
int analyze_mode;
int command_level;           /* determine which of process command to be called */
int bit_image_mode;          /* use to save current bit image mode */
int line_spacing;            /* determine base position or offset_1 to update */
int base_position;           /* use in convert graphic data to display data */
int offset_1;                /* use in convert graphic data to display data */
int past_printer_data;       /* use to save old printer data */
int dot_per_line;            /* use in update base positon */
int dot_per_mode;            /* save number of dot in bit image mode */

/*
int is_command(char st[])
{
 int                         i;
 i=0;
 if (st[i]==BLANK)
    {
     while ((st[i]==BLANK) && (st[i] != NULL))
           i++;
     if (st[i]=='.')
        return(YES);
     else
        return(NO);
    }
 else
    {
     if (st[i]=='.')
        return(YES);
     else
        return(NO);
    }
}

/*
void get_argument(char arg[][40],char st[],int *no_arg)
{
 int                         i,j,k,spflg;
 i=j=k=0;
 memset(arg,0,400);
 if (st[i]==BLANK)
    spflg=NO;
 else
    spflg=YES;
 while (st[i] != NULL) {
    if (spflg==YES) {
       k=0;
       while ((st[i] > BLANK) && (st[i] != 0xa0)) {
          arg[j][k]=st[i];
          i++;
          k++;
       }
       arg[j][k]=NULL;
       spflg=NO;
       j++;
    }
    else {
        while (((st[i] <= BLANK) || (st[i]==0xa0)) && (st[i] != NULL))
           i++;
        spflg=YES;
    }
 }
 *no_arg=j;
 /* for (i=0;i<20;i++)
     printf("%c%x ",arg[1][i],arg[1][i]); */
}
*/

int getpr(void)
{
 char  buffer;
 if (_read(prd_handle,&buffer,1)==0)
    p_fileready=YES;
 return(buffer);
}


void get_graphic_data(void)
{
 extern int                  pic_offset;
 char                        *buffer;
 int                         number_dot,number_dot_lo,number_dot_high;
 int                         i,j,base;
 number_dot_lo=getpr();
 number_dot_high=getpr();
 number_dot=number_dot_high*256+number_dot_lo;
 buffer=(char *)calloc(3264,sizeof(char));
 if (_read(prd_handle,buffer,number_dot)==0)
    p_fileready=YES;
 if (analyze_mode==NO)
    switch (dot_per_mode)
    {
      case 480:
           base=pic_offset*24;
           for (i=0;i<number_dot;i++)
               for (j=0;j<4;j++)
                   print_buffer_pointer[base+(i*4)+j]=buffer[i];
           break;
      case 960:
           base=pic_offset*24;
           for (i=0;i<number_dot;i++)
               for (j=0;j<2;j++)
                   print_buffer_pointer[base+(i*2)+j]=buffer[i];
           break;
      case 1920:
           base=pic_offset*24;
           for (i=0;i<number_dot;i++)
               print_buffer_pointer[base+i]=buffer[i];
           break;
    }
 free(buffer);
}

void process_command_0(int printer_data)
{
 switch (printer_data)
 {
  case ESC:
       command_level=1;
       break;
  case LF:
       grp_ready=YES;
       break;
 }
}

void process_command_1(int printer_data)
{
 extern int                  command_level;
 extern int                  bit_image_mode;
 extern int                  dot_per_mode;
 extern int                  line_spacing;
 switch (printer_data)
  {
   case BIT_IMAGE_COMMAND:
        switch (getpr())
        {
         case 0:
              bit_image_mode=1;
              dot_per_mode=DOT_BIT_IMAGE_MODE_1;
              get_graphic_data();
              command_level=0;
              break;
         case 1:
              bit_image_mode=2;
              dot_per_mode=DOT_BIT_IMAGE_MODE_2;
              get_graphic_data();
              command_level=0;
              break;
         case 2:
              bit_image_mode=2;
              dot_per_mode=DOT_BIT_IMAGE_MODE_2;
              get_graphic_data();
              command_level=0;
              break;
         case 3:
              bit_image_mode=4;
              dot_per_mode=DOT_BIT_IMAGE_MODE_4;
              get_graphic_data();
              command_level=0;
              break;
         case 32:
              bit_image_mode=1;
              dot_per_mode=DOT_BIT_IMAGE_MODE_1;
              get_graphic_data();
              command_level=0;
              break;
         case 33:
              bit_image_mode=2;
              dot_per_mode=DOT_BIT_IMAGE_MODE_2;
              get_graphic_data();
              command_level=0;
              break;
         case 39:
              bit_image_mode=3;
              dot_per_mode=DOT_BIT_IMAGE_MODE_3;
              get_graphic_data();
              command_level=0;
              break;
        }
        break;
   case SINGLE_DENSITY:
        bit_image_mode=1;
        dot_per_mode=DOT_BIT_IMAGE_MODE_1;
        get_graphic_data();
        command_level=0;
        break;
   case DOUBLE_DENSITY:
        bit_image_mode=2;
        dot_per_mode=DOT_BIT_IMAGE_MODE_2;
        get_graphic_data();
        command_level=0;
        break;
   case H_SP_DOUBLE_DENSITY:
        bit_image_mode=2;
        dot_per_mode=DOT_BIT_IMAGE_MODE_2;
        get_graphic_data();
        command_level=0;
        break;
   case QUADRUPLE_DENSITY:
        bit_image_mode=4;
        dot_per_mode=DOT_BIT_IMAGE_MODE_4;
        get_graphic_data();
        command_level=0;
        break;
   case SET_1_8_LINE_SPACE:
        line_spacing=getpr()*22;
        if (analyze_mode==YES)
           if (line_spacing<MINIMUM_LINE_SPACING)
              dot_per_line++;
           else
              p_fileready=YES;
        command_level=0;
        break;
   case SET_1_6_LINE_SPACE:
        line_spacing=getpr()*30;
        if (analyze_mode==YES)
           if (line_spacing<MINIMUM_LINE_SPACING)
              dot_per_line++;
           else
              p_fileready=YES;
        command_level=0;
        break;
   case SET_N_60_LINE_SPACE:
        line_spacing=getpr()*3;
        if (analyze_mode==YES)
           if (line_spacing<MINIMUM_LINE_SPACING)
              dot_per_line++;
           else
              p_fileready=YES;
        command_level=0;
        break;
   case SET_N_180_LINE_SPACE:
        line_spacing=getpr();
        if (analyze_mode==YES)
           if (line_spacing<MINIMUM_LINE_SPACING)
              dot_per_line++;
           else
              p_fileready=YES;
        command_level=0;
        break;
   case N_180_LINE_FEED:
        line_spacing=getpr();
        if (analyze_mode==YES)
           if (line_spacing<MINIMUM_LINE_SPACING)
              dot_per_line++;
           else
              p_fileready=YES;
        grp_ready=YES;
        command_level=0;
        break;
  }
}

void process_command(int printer_data)
{
 extern int                 past_printer_data;
 past_printer_data=printer_data;
 switch (command_level)
 {
  case 0:
       process_command_0(printer_data);
       break;
  case 1:
       process_command_1(printer_data);
       break;
 }
}

void analyze_picture_file(char *st)
{
 extern int                  dot_per_line;
 dot_per_line=1;
 command_level=0;
 p_fileready=NO;
 analyze_mode=YES;
 prd_handle=open(st,O_RDONLY | O_BINARY);
 while (p_fileready==NO)
       process_command(getpr());
 close(prd_handle);
}

void initialize_read(char *st)
{
 command_level=0;
 p_fileready=NO;
 analyze_mode=NO;
 prd_handle=open(st,O_RDONLY | O_BINARY);
}

void read_picture_file(void)
{
 while ((p_fileready==NO) && (grp_ready==NO))
       process_command(getpr());
 if (p_fileready==YES)
    {
     pic_print=NO;
     close(prd_handle);
    }
}
*/
