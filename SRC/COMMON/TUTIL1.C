
/*****************************************************************************/
/* find length of thai string                                                */
/* input                                                                     */
/*      thaist : thai string                                                 */
/* return : length of thai string                                            */
/* Updated: Suttipong Kanakakorn, use pointer instead of array
            Sun  08-06-1989  16:32:15                                        */
/*****************************************************************************/
int thaistrlen(char *thaist)
{
  register int count = 0;

  while (*thaist) {
        if (whatlevel(*thaist) == MIDDLE)
           count++;
        thaist++;
  }
  return(count);
}
