/* Written: Suttipong Kanakakorn */
/* Updated: Sun  08-27-1989  23:04:09 */

#include <dir.h>
#include <stdio.h>

#include "..\common\cwtype.h"

search_file_mode find_file(char *filename, char *search_path)
{
    struct ffblk ffblk;
    char p[MAXPATH];

    if (findfirst(filename, &ffblk, 0) == 0)
        return CUR_DIR;
    sprintf(p, "%s\\%s", search_path, filename);
    if (findfirst(p, &ffblk, 0) == 0)
        return CW_DIR;
    return NOT_FOUND;
}
