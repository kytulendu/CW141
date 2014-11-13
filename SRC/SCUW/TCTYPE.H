/* -------------------------------------------------------------
	This is the new Thai character type checking Method.
	Copyright by Subun & Tanat Advanced Computer Inc. 1988
	Used with permission, Sahaviriya System.

	By using short int (16 bits).

	- High order byte is reserved for future use.
		1. bit 0  for back punctuation.
		2. bit 1  for front punctuation.

	- Lower order byte is devided into 6 parts.
 		1. bit 7  for consonants.
		   bit 6  for always leading consonants.
		2. bit 5  for leading vowels.
		   bit 4  for following vowels (rear).
		3. bit 3  for tonal marks.
		4. bit 2  for garant marks.
		5. bit 1  for special marks.
		6. bit 0  for thai numbers.

	- The tctype's functions are istcon(c),istlcon(c),isvowel(c),istlvwl(c)
		istrvwl(c),isttnl(c),istgrnt(c),istspmk(c),istdigit(c).

	- istcon(c)		: check thai consonant.
	- istlcon(c)	: check thai leading consonant (╘,╪,╫,н).
	- istvowel(c)	: check thai vowel.
	- istlvwl(c)	: check thai leading vowel (Ю,А,Д,Ц,Б).
	- istrvwl(c)	: check thai rear vowel (п р  т   оео).
	- isttnl(c)		: check thai tonal mark.
	- istgrant(c)	: check thai garant mark.
	- istspmk(c)	: check thai special mark.
	- istdigit(c)	: check thai digit.
	- isfpunc(c)	: check front punctuation.({,(,[,)
	- isbpunc(c)	: check back punctuation. ( },),],!,?,:,;,%,ъ)
	- ispunc(c)		: check punctuation.

	----------------------------------------------------------- */

extern short int _tctype[];

#ifndef __TCTYPE__
#define __TCTYPE__

#define	_CSN_	0x80
#define _LCSN_	0x40
#define _VWL_	0x30
#define _LVWL_	0x20
#define _RVWL_	0x10
#define _TNL_	0x08
#define _GRNT_	0x04
#define _SPMK_	0x02
#define _TDGT_	0x01
#define _BPUNC_ 0x0100	/* Back Punctuation */
#define _FPUNC_ 0x0200	/* Front Punctuatuon */

#ifndef tlint
#define istcon(c)		((_tctype)[c]&(_CSN_))
#define istlcon(c)		((_tctype)[c]&(_LCSN_))
#define isvowel(c)		((_tctype)[c]&(_VWL_))
#define istlvwl(c)		((_tctype)[c]&(_LVWL_))
#define istrvwl(c)		((_tctype)[c]&(_RVWL_))
#define isttnl(c)		((_tctype)[c]&(_TNL_))
#define	istgrnt(c)		((_tctype)[c]&(_GRNT_))
#define istspmk(c)		((_tctype)[c]&(_SPMK_))
#define istdigit(c)		((_tctype)[c]&(_TDGT_))
#define isthai(c)		(istcon(c) ||isvowel(c) || isttnl(c) || istgrnt(c) || istdigit(c))
#define isfpunc(c)		((_tctype)[c]&(_FPUNC_))
#define isbpunc(c)		((_tctype)[c]&(_BPUNC_))
#define ispunc(c)		(isbpunc(c) || isfpunc(c))
#endif

#endif __TCTYPE__
