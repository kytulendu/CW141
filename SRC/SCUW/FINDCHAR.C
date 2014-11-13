/* This function is written for find the position of character in string
	that is ascending-sorted, if not found then return NULL.
	And when character is greater than an element in string, return NULL.

	Sample calling statement : findchar ('x', "abcdefg");
	return value : NULL(or 0) ,if not found
					Else return string's position + 1
					(real position must minus 1)
	Copyright by Subun & Tanat Advanced Computer Inc. 1988
	Date Written : Tuesday, 10 May, 1988 11:25:20 AM

	Last Updated : Tuesday, 10 May, 1988 11:22:59 AM

*/

#include <stdio.h>

short int findchar(unsigned char ch, unsigned char *strbuf)
{
	while (*strbuf != NULL && ch >= *strbuf)
		if (ch == *(strbuf++))
			return(1);
	return(NULL);
}
