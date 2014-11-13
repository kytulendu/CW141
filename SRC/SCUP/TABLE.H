#define	CP_MIDMIDDLE	0x8F
#define	CP_BOTTOMMIDDLE	0x90
#define	CP_TOPMIDDLE	0x91
#define	CP_MIDRIGHT	0x92
#define	CP_MIDLEFT	0x93
#define	CP_LEFTUP	0x94
#define	CP_HORIZONTAL	0x95
#define	CP_VERTICAL	0x96
#define	CP_RIGHTUP	0x97
#define	CP_TOPLEFT	0x98
#define	CP_TOPRIGHT	0x99
#define	CP_BOTTOMLEFT	0x9A
#define	CP_BOTTOMRIGHT	0x9B
#define	CP_LEFTBOTTOM	0x9C
#define	CP_RIGHTBOTTOM	0x9D

#define	cp_istablechar(ch)  (((ch) >= CP_MIDMIDDLE)&&((ch) <= CP_RIGHTBOTTOM))
