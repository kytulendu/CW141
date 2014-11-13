/*-------------------------------------------------------------------------*/
/*           Graphics hardware detector header for TURBO C 2.00            */
/*                                                                         */
/*                         Kraisorn Liansee                                */
/*           Date    :         4/27/89                                     */
/*           Updated :         4/27/89                                     */
/*                      Suttipong Kanakakorn                               */
/*           Updated : Sun  08-27-1989 20:25:09                            */
/*-------------------------------------------------------------------------*/

typedef enum Graphics_hardware
{
    DETECT, /* request autodetection */
    CGA, MCGA, EGA, EGA64, EGAMONO, IBM8514,         /* 1 - 6 */
    HERCMONO, ATT400, VGA, PC3270,                   /* 7 - 10 */
    MDA = 128,
    UNKNOWN_HW = -1
 } graphics_hardware;

void graph_detecthardware (graphics_hardware *mode_ptr);
void egavga_class (graphics_hardware *mode_ptr);

int is_egavga (void);
int is_mcga (void);
int check_vretrace (void);
