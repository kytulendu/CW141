/*	-------------------------------------------------------
	name	 : nstrcmp (new string compare)
	function : compare two strings until the end of source string
	return value	: 0 -> when same.
					: positive value -> when not same
						and source greater than dest.
					: negative value _> when not same
						and source less that dest.
	date written	: Friday 13, May 1988
	last updated	: Saturday, 14 May, 1988 10:03:04 AM
	-------------------------------------------------------	*/

int nstrcmp(register char *source, register char *destination)
{
	if (source != destination)
		while(*source)
			{
				if (*source++ != *destination++)
					return(*--source - *--destination);
						/* return negative if source less than dest.
						   else positive will be returned */
			}
	return(0); /* when same return zero */
}
