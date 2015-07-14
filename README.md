CU Writer 1.41 Source Code
==========================

Descriptions
------------
[CU Writer](http://th.wikipedia.org/wiki/%E0%B8%8B%E0%B8%B5%E0%B8%A2%E0%B8%B9%E0%B9%84%E0%B8%A3%E0%B9%80%E0%B8%95%E0%B8%AD%E0%B8%A3%E0%B9%8C) is a Thai [WordStar](https://en.wikipedia.org/wiki/WordStar) clone that run on [DOS](https://en.wikipedia.org/wiki/DOS).
Develop by Computer Service Centre and Faculty of Engineering, Chulalongkorn University.
First public release in april 1989 in [public domain](http://unlicense.org/),
through BBTV Channel 7.

This is the same original source code that found on a ftp server.
ftp://ftp.cs.washington.edu/pub/thaisys/software/dos/cu-writer/

    cusrc-1.zip 	41 KB 	12/5/2536 	0:00:00
    cusrc-2.zip 	37 KB 	12/5/2536 	0:00:00
    cusrc-3.zip 	62 KB 	12/5/2536 	0:00:00
    cusrc-4.zip 	91 KB 	12/5/2536 	0:00:00

no modification in the source code, except change a function name
in \COMMON\CSCR.C from line to _line to make it not conflict with
Borland Graphics Interface (BGI) when linking CUFONT,
some edit in MAKEFILE and TURBOC.CFG.

I also add the original document which included in CU Writer 1.41 release,
TIS-620 converted document and binary font files to the repo.

The source code use Thai Industrial Standard 620-2533 (TIS-620)
for character encoding.

System Requirement
------------------

  * IBM PC or Compatible, 8086 or 8088 CPU, 256 KB RAM or better
  * DOS 3.3 or better
  * Hercules, EGA, VGA, MCGA or AT&T 400 (Graphic card in AT&T PC 6300/Olivetti M24)
  * Two 5.25" 360Kb floppy disk drive
  * 9 pin or 24 pin Dot-Matrix printer

Compiling the source code
-------------------------

1. To build this source code you need

   - Borland Turbo C 2.01
   - Borland Turbo Assembler 2.0 or newer
   
2. Put this reposity content into C:\CW
3. Set PATH to Turbo C and Turbo Assembler
4. Change directory to C:\CW\SRC
5. Run MAKE.BAT
6. Binary will go to C:\CW\

CU-Writer developers
--------------------
    สถาบันบริการคอมพิวเตอร์ จุฬาลงกรณ์มหาวิทยาลัย และ ภาควิชาวิศวกรรมคอมพิวเตอร์ คณะวิศวกรรมศาสตร์ จุฬาลงกรณ์มหาวิทยาลัย
    Computer Service Centre and Faculty of Engineering, Chulalongkorn University

    สำนวน หิรัญวงษ์	
    Sumnuan Hirunwong
    
    ไกรศร เลียนษี
	Kraisorn  Liansee
	
    ดำเกิง ธรรมเถกิงกิจ
    
    ทศพล อภิกุลวณิช
	Thotsapol Aphikulvanich
	
    ธณัฎฐ์ จิระกาล
	Tanat Chirakarn
	
    ประสาท โตปิตุมาตุคุณ
	Prasart Topitumatukhun
	
    ลักษณ์ชัย ลือชัยชนะกุล
    
    วิทยา วงศ์อนวัช
	Wittaya Wonganawat
	
    สุทธิพงษ์ คนาคกร
	Suttipong Kanakakorn
	
    สุบรรณ เย็นใจชน
	Subun Yenjaichon
	
    สุวพจน์ จันทโรจนวงศ์

If you know any one who is in CU-Writer project that isn't in this list,
please in inform me. ;)
