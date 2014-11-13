/* ------ Define constant ---- */
#define	CP_UPPER	0
#define	CP_MIDDLE	1
#define	CP_LOWER	2
#define	CP_UPPEST	3
#define CP_INVALID  4

/* ------ Define thai table ----- */
extern  char	cp_smo_table[];
extern  char	cp_ku_table[];
extern  char	cp_thaitable[];

/* ------ Define Macro ------- */
#define	cp_ismidlev(ch)		( (cp_thaitable[ch] == CP_MIDDLE) )
#define	cp_isuplev(ch)		( (cp_thaitable[ch] == CP_UPPER ) )
#define	cp_islowlev(ch)		( (cp_thaitable[ch] == CP_LOWER ) )
#define	cp_isuppest(ch)		( (cp_thaitable[ch] == CP_UPPEST) )
