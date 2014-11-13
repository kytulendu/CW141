/* macro for control code of printer */
#define SET_1_8_LINE_SPACE   0x30      /* set 1/8 inch line spacing */
#define SET_1_6_LINE_SPACE   0x32      /* set 1/6 inch line spacing */
#define SET_N_180_LINE_SPACE 0x33      /* set n/180 inch line spacing */
#define SET_N_60_LINE_SPACE  0x41      /* set n/60 inch line spacing */
#define N_180_LINE_FEED      0x4a      /* perform n/180 inch line feed */
#define SET_PAGE_IN_LINE     0x43      /* set page length in lines */
#define SINGLE_DENSITY       0x4b      /* set single density bit image */
#define DOUBLE_DENSITY       0x4c      /* set double density bit image */
#define H_SP_DOUBLE_DENSITY  0x59      /* select high speed double density bit image*/
#define QUADRUPLE_DENSITY    0x5a      /* select quadruple_density bit image printing */
#define BIT_IMAGE_COMMAND    0x2a      /* general 8 pin bit image command */

/* macro for buffer */
#define MAX_BUFFER           1920
#define DOT_BIT_IMAGE_MODE_1 480
#define DOT_BIT_IMAGE_MODE_2 960
#define DOT_BIT_IMAGE_MODE_3 1440
#define DOT_BIT_IMAGE_MODE_4 1920

#define MINIMUM_LINE_SPACING 4         /* minimum line spacing not update base position */
#define MASK_BYTE            128

/* macro for font area */
#define MAX_FONT             10
#define FONT_AREA_SIZE       11264

/* macro for error message */
#define MEM_NOT_ENOUGH       0
#define NOT_OPEN_FILE        1

/* define printer type */
#define EPSON9PIN80COLTYPE1     0
#define EPSON9PIN136COLTYPE1    1
#define EPSON9PIN80COLTYPE2     2
#define EPSON24PIN80COL         3
#define EPSON24PIN136COL        4
#define NEC24PIN80COL           5
#define NEC24PIN136COL          6
#define FX                   0
#define LX                   1
#define LQ                   2
#define N5                   3
#define N7                   4

/* Add by Suttipong Kanakakorn Tue  08-29-1989  12:32:36 */
/* for calculating linespace */
/* IPL = inch per line / 180 for 24 pins printer */
/* IPL = inch per line / 216 for 9 pins printer */

#define IPL_TEXT_24     60
#define IPL_TEXT_9      67
#define IPL_GRPH_24     46
#define IPL_GRPH_9      48

/* ML = maxline per page */
#define ML_TEXT_24      (((pagelength)-1) * 180 / IPL_TEXT_24)
#define ML_TEXT_9       (((pagelength)-1) * 216 / IPL_TEXT_9)
#define ML_GRPH_24      (((pagelength)-1) * 180 / IPL_GRPH_24)
#define ML_GRPH_9       (((pagelength)-1) * 216 / IPL_GRPH_9)
