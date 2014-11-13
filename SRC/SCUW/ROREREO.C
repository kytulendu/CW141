#include "Thai.h"
#include "TCtype.h"
#include "Global.h"

unsigned char *RoreReoRtn(TempIndx)
    register unsigned char *TempIndx;
{
    if (*(TempIndx - 1) != RoreReo)
        return(FAIL);
    if (istrvwl(*(TempIndx - 2)))
        return(TempIndx - 1);
    return(FAIL);
}