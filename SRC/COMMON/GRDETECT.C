/*-------------------------------------------------------------------------*/
/*           Graphics hardware detector module for TURBO C 2.00            */
/*                                                                         */
/*                         Kraisorn Liansee                                */
/*           Date    :         4/27/89                                     */
/*           Updated :         4/27/89                                     */
/*           Updated : Suttipong Kanakakorn Sun  08-20-1989  11:28:45      */
/*-------------------------------------------------------------------------*/

#include <dos.h>

#include "grdetect.h"

/*-----------------------------------------------------------------*/
/*  Graph_detecthardware function                                  */
/*                                                                 */
/*        check current video configuration                        */
/*-----------------------------------------------------------------*/

void graph_detecthardware (graphics_hardware *mode_ptr)
{
    union REGS reg_pack;
    unsigned char far *mem_ptr, byte_keep;

    *mode_ptr = UNKNOWN_HW;

    reg_pack.h.ah = 0x0F;              /* read current video state */
    int86 (0x10, &reg_pack, &reg_pack);

    if (reg_pack.h.al == 0x07) {  /* Monochrome mode          */
        /*----------------------------------------*/
        /* May be CGA/EGA/VGA/MDA/Hercules        */
        /*----------------------------------------*/

        if (is_egavga ())
        {
             /*-----------------*/
             /* EGA/VGA test    */
             /*-----------------*/
             egavga_class (mode_ptr);      /* Classify EGA/VGA           */
             return;
        }
        /*--------------------------*/
        /* CGA/MDA/Hercules test    */
        /*--------------------------*/

        if (check_vretrace() == 0)      /* No Vertical retrace change    */
        {
             mem_ptr = (unsigned char far *) 0xB8000L; /* point to screen  */
             byte_keep = *mem_ptr;
             (*mem_ptr) ^= 0xFF;
             byte_keep  ^= 0xFF;
             if (byte_keep != (*mem_ptr))   /* No memory in this area        */
                  *mode_ptr = MDA;          /* IBM Monochrome monitor        */
             else
                  *mode_ptr = CGA;
             return;                        /* CGA adaptor                   */
        }
        else
        {
             *mode_ptr = HERCMONO;
             return;
        }
    } else {                            /* Assuming ALL except Mono */
        if (is_egavga()) {
             /*-----------------*/
             /* EGA/VGA test    */
             /*-----------------*/
             egavga_class (mode_ptr);      /* Classify EGA/VGA           */
             return;
        }
        if (is_mcga ())
           *mode_ptr = MCGA;
        else
           *mode_ptr = CGA;
        return;
    }
}

/*----------------------------------------------------------------*/
/*  Is_egavga function                                            */
/*                                                                */
/*      check if the equipped adaptor is EGA or VGA               */
/*----------------------------------------------------------------*/

int is_egavga ()
{
   union REGS reg_pack;

   reg_pack.x.ax = 0x1200;          /* Read EGA/VGA configuration */
   reg_pack.h.bl = 0x10;
   reg_pack.h.bh = 0xFF;            /* video controller mode      */
   reg_pack.h.cl = 0x0F;            /* video config mode          */
   int86 (0x10, &reg_pack, &reg_pack);

   if ((reg_pack.h.cl > 0x0C)||     /* invalid configuration      */
       (reg_pack.h.bh > 0x01) ||    /* invalid controller mode    */
       (reg_pack.h.bl > 0x03))      /* invalid memory size        */
       return (0);
   else
       return (1);
}

/*----------------------------------------------------------------*/
/*  EgaVga_class function                                         */
/*                                                                */
/*       determines EGA/VGA adaptor configuration                 */
/*----------------------------------------------------------------*/
void egavga_class (graphics_hardware *mode_ptr)
{
   union REGS reg_pack;
   unsigned char far *mem_ptr;

   reg_pack.x.ax = 0x1200;          /* Read EGA/VGA configuration */
   reg_pack.h.bl = 0x10;
   reg_pack.h.bh = 0xFF;            /* video controller mode      */
   reg_pack.h.cl = 0x0F;            /* video config mode          */
   int86 (0x10, &reg_pack, &reg_pack);

    *mode_ptr = EGA64;              /* Assumes EGA with 64K first */
    if (reg_pack.h.bh == 1)         /* Config in Monochrome mode  */
    {
       *mode_ptr = EGAMONO;
       return;
    }
    if ((reg_pack.h.bl == 0) || (reg_pack.h.cl == 0) || (reg_pack.h.cl == 1)
        || (reg_pack.h.cl == 6) || (reg_pack.h.cl == 7))
        return;                     /* EGA 64K                    */
    *mode_ptr = EGA;
    mem_ptr = (unsigned char far *) 0xC0039L;/* Checking ROM      */
    if (is_mcga ())
       *mode_ptr = VGA;
    else
     if ((*mem_ptr==0x5A) && (*(mem_ptr+1)==0x34) && (*(mem_ptr+2)==0x34)
         && (*(mem_ptr+3)==0x39))
         *mode_ptr = VGA;
    return;
}

/*----------------------------------------------------------------*/
/*  Is_mcga function                                              */
/*                                                                */
/*      check if the equipped adaptor is MCGA                     */
/*----------------------------------------------------------------*/
int is_mcga ()
{
    union REGS reg_pack;

    reg_pack.x.ax = 0x1A00;        /* Read video information      */
    int86 (0x10, &reg_pack, &reg_pack);

    if (reg_pack.h.al != 0x1A)     /* Call is invalid             */
       return (0);
    if ((reg_pack.h.bl == 7)||(reg_pack.h.bl == 8)||
        (reg_pack.h.bl == 0x0B)||(reg_pack.h.bl == 0x0C))
       return (1);
    else
       return (0);
}


/*-----------------------------------------------------------------*/
/*  Check_vretrace function                                        */
/*                                                                 */
/*       check if there is any transition on 0x3BF port            */
/*-----------------------------------------------------------------*/

int check_vretrace ()
{
    register unsigned int change;
    register unsigned int old_value;
    unsigned int count;

    old_value = inportb (0x3BA) & 0x80;
    change = 0;
    for (count = 0; count <= 0x7FFF; count++)
    {
        if ((inportb (0x3BA) & 0x80) != old_value)
           if (++change > 10) break;    /* Vertical Retrace changes */
    }
    if (change <= 10) return (0);       /* Not hercules             */

    for (count = 0; count <= 0x7FFF; count++)
    {
        if ((inportb (0x3BA) & 0x30) != 0x10)
           return (1);
    }

    return (2);
}
