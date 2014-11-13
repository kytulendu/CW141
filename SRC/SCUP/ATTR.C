#include	<string.h>
#include	"24pins.h"

/* -------------------------------------------------------------------- */
/*	Function	:	cp_lqscr_blank				*/
/*	Description	:	clear buffer				*/
/*	Parameters	:	buf     - font buffer			*/
/*	Return value	:	pointer to the font buffer		*/
/*	Remark		:	This function used buf as buffer	*/
/* -------------------------------------------------------------------- */
FONT	*cp_lqscr_blank( FONT *buf)
{
	memset ( buf, 0, CP_LQ_FONTSIZE);
	return ( buf);
}
/* -------------------------------------------------------------------- */
/*	Function	:	cp_sub_middle_lower			*/
/*	Description	:	write subscript character middle and	*/
/*				lower to font character buffer		*/
/*	Parameters	:	buf     - font buffer			*/
/*				middle  - middle subscript buffer	*/
/*				lower	- lower subscript buffer	*/
/*	Return value	:	pointer to the font buffer		*/
/*	Remark		:	This function used buf as buffer	*/
/* -------------------------------------------------------------------- */
FONT	*cp_sub_middle_lower ( FONT *buf, FONTSCR *middle, FONTSCR * lower)
{
	int	i;

	memset (buf , 0, CP_LQ_FONTSIZE);
	for ( i = 0; i < CP_LQCOL; i++) {
		buf -> font[i][0] = middle-> font[i][1];
		buf -> font[i][1] = ( (lower -> font[i][0]) << 4) |
				    ( (lower -> font[i][1]) >> 4);
		buf -> font[i][2] = ( (lower -> font[i][1]) << 4);
	};
	return ( buf);
}
/* -------------------------------------------------------------------- */
/*	Function	:	cp_sub_upper_middle			*/
/*	Description	:	write subscript character upper and	*/
/*				middle to font character buffer		*/
/*	Parameters	:	buf     - font buffer			*/
/*				middle  - middle subscript buffer	*/
/*				upper	- upper subscript buffer	*/
/*	Return value	:	pointer to the font buffer		*/
/*	Remark		:	This function used buf as buffer	*/
/* -------------------------------------------------------------------- */
FONT	*cp_sub_upper_middle( FONT *buf, FONTSCR *upper, FONTSCR *middle)
{
	int	i;

	memset ( buf, 0, CP_LQ_FONTSIZE);
        for ( i = 0; i < CP_LQCOL; i++) {
		buf -> font[i][1] = ( (upper -> font[i][0]) << 4 ) |
				    ( (upper -> font[i][1]) >> 4 );
		buf -> font[i][2] = ( (upper -> font[i][1]) << 4 ) |
				       middle-> font[i][0];
	};
	return ( buf);
}

/* -------------------------------------------------------------------- */
/*	Function	:	cp_super_upper_middle			*/
/*	Description	:	write superscript character upper and	*/
/*				middle to font character buffer		*/
/*	Parameters	:	buf     - font buffer			*/
/*				middle  - middle subscript pointer	*/
/*				upper	- upper subscript pointer	*/
/*	Return value	:	pointer to the font buffer		*/
/*	Remark		:	This function used buf as buffer	*/
/* -------------------------------------------------------------------- */
FONT	*cp_super_upper_middle ( FONT *buf , FONTSCR *upper, FONTSCR *middle)
{
	int	i;

	memset ( buf , 0, CP_LQ_FONTSIZE);
	for ( i = 0; i < CP_LQCOL; i++) {
		buf -> font[i][0] = ( (upper -> font[i][0]) << 4) |
				    ( (upper -> font[i][1]) >> 4);
		buf -> font[i][1] = ( (upper -> font[i][1]) << 4);
		buf -> font[i][2] = ( (middle -> font[i][0]) << 4) |
				    ( (middle -> font[i][1]) >> 4);
	};
	return ( buf);
}
/* -------------------------------------------------------------------- */
/*	Function	:	cp_super_middle_lower			*/
/*	Description	:	write superscript character middle and 	*/
/*				lower to font character buffer		*/
/*	Parameters	:	buf     - font buffer			*/
/*				lower	- lower superscript pointer	*/
/*	Return value	:	pointer to the font buffer		*/
/*	Remark		:	This function used buf as buffer	*/
/* -------------------------------------------------------------------- */
FONT	*cp_super_middle_lower ( FONT *buf , FONTSCR *middle, FONTSCR *lower)
{
	int	i;

	memset ( buf , 0, CP_LQ_FONTSIZE);
	for ( i = 0; i < CP_LQCOL; i++) {
		buf -> font[i][0] = ( (middle -> font[i][1]) << 4);
		buf -> font[i][1] = ( (lower  -> font[i][0]) << 4) |
				    ( (lower  -> font[i][1]) >> 4);
		buf -> font[i][2] = ( (lower  -> font[i][1]) << 4);
	};
	return ( buf);
}
/* -------------------------------------------------------------------- */
/*	Function	:	cp_underline				*/
/*	Description	:	underline character in buffer		*/
/*	Parameters	:	newchar   - new character buffer	*/
/*				oldchar   - old character pointer	*/
/*	Return value	:	pointer to newchar buffer		*/
/*	Remark		:	This function used newchar as buffer	*/
/* -------------------------------------------------------------------- */
FONT	*cp_underline ( FONT *newchar, FONT *oldchar)
{
	int i;

	memcpy ( newchar, oldchar, CP_LQ_FONTSIZE);
	for (i = 0; i < CP_LQCOL; i++)
		newchar->font[i][0] |= CP_UNDERLINE_MSK;
	return ( newchar );
}
/* -------------------------------------------------------------------- */
/*	Function	:	cp_doubleline				*/
/*	Description	:	doubleline character in buffer		*/
/*	Parameters	:	newchar   - new character buffer	*/
/*				oldchar   - old character pointer	*/
/*	Return value	:	pointer to newchar buffer		*/
/*	Remark		:	This function used newchar as buffer	*/
/* -------------------------------------------------------------------- */
FONT	*cp_doubleline ( FONT *newchar, FONT *oldchar)
{
	int i;

	memcpy ( newchar, oldchar, CP_LQ_FONTSIZE);
	for (i = 0; i < CP_LQCOL; i++)
		newchar->font[i][0] |= CP_DOUBLELINE_MSK;
	return ( newchar );
}
/* --------------------------------------------------------------------	*/
/*	Function	:	cp_enlarge				*/
/*	Description	:	enlarge character in graphic buffer	*/
/*	Parameters	:	buf    - graphic buffer			*/
/*				index  - index to text position in	*/
/*					 graphic buffer			*/
/* 	Return value	:	index to next position in graph buffer	*/
/* -------------------------------------------------------------------- */
int	cp_enlarge( struct cp_grlqbuf buf, int index)
{
	int	i, j;
	FONT	upper[2], middle[2], lower[2];

	index--;	/* Pointer to that character */
	for ( i = 0; i < 17; i ++)
	    for (j = 0; j < CP_LQROW; j++) {
		upper[0].font[2*i][j] = upper[0].font[2*i+1][j]
				      = (buf.upper + index)->font[i][j];
		middle[0].font[2*i][j] = middle[0].font[2*i+1][j]
				      = (buf.middle + index)->font[i][j];
		lower[0].font[2*i][j] = lower[0].font[2*i+1][j]
				      = (buf.lower + index)->font[i][j];
	    };

	for ( j = 0; j < CP_LQROW; j++) {
		upper[0].font[2*i][j] = upper[0].font[2*i+1][j]
				      = (buf.upper + index)->font[i][j];
		middle[0].font[2*i][j] = middle[0].font[2*i+1][j]
				      = (buf.middle + index)->font[i][j];
		lower[0].font[2*i][j] = lower[0].font[2*i+1][j]
				      = (buf.lower + index)->font[i][j];
	};

	memcpy (buf.upper + index, upper, 2 * CP_LQ_FONTSIZE );
	memcpy (buf.middle + index, middle, 2 * CP_LQ_FONTSIZE );
	memcpy (buf.lower + index, lower, 2 * CP_LQ_FONTSIZE );
	return (index + 2);
}

/* --------------------------------------------------------------------	*/
/*	Function	:	cp_bold					*/
/*	Description	:	change any character to boldface	*/
/*	Parameters	:	buf    - graphic buffer			*/
/*				index  - index to text position in	*/
/*					 graphic buffer			*/
/* 	Return value	:	NONE					*/
/* -------------------------------------------------------------------- */
void	cp_bold ( struct cp_grlqbuf buf, int index)
{
	int	i, j;
	FONT	upper, middle, lower;

	index--;	/* Pointer to that character */
	for ( i = 0; i < 17; i ++)
		for (j = 0; j < CP_LQROW; j++) {
			upper.font[i][j]  = (buf.upper + index)->font[i][j];
			upper.font[i][j]  |= (buf.upper + index)->font[i+1][j];

			middle.font[i][j] = (buf.middle + index)->font[i][j];
			middle.font[i][j] |= (buf.middle + index)->font[i+1][j];

			lower.font[i][j]  = (buf.lower + index)->font[i][j];
			lower.font[i][j]  |= (buf.lower + index)->font[i+1][j];
		};

	for ( j = 0; j < CP_LQROW; j++) {
		upper.font[i][j]  = (buf.upper + index)->font[i][j];
		middle.font[i][j] = (buf.middle + index)->font[i][j];
		lower.font[i][j]  = (buf.lower + index)->font[i][j];
	};

	memcpy (buf.upper + index, &upper, CP_LQ_FONTSIZE );
	memcpy (buf.middle + index, &middle, CP_LQ_FONTSIZE );
	memcpy (buf.lower + index, &lower, CP_LQ_FONTSIZE );
}
