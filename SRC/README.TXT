CU-Writer 1.41 by Chulalongkorn University
==========================================

Buliding source code
--------------------

1. To build this source code you need

   - Turbo C 2.01
   - Turbo Assembler 2.0 or newer

2. Set PATH to Turbo C and Turbo Assembler

3. Run MAKE.BAT

4. Binary will go to C:\CW\


Directory
---------

C:\
|
|---CW         => Binary
|    |
|    \---Src   => Source directory
|
|---TASM       => Turbo Assembler directory
|
\---TC         => Turbo C 2.01 binary directory

Note
----

This is an original CU-Writer 1.41 that I found on a ftp side.

ftp://ftp.cs.washington.edu/pub/thaisys/software/dos/cu-writer/

cusrc-1.zip 	41 KB 	12/5/2536 	0:00:00
cusrc-2.zip 	37 KB 	12/5/2536 	0:00:00
cusrc-3.zip 	62 KB 	12/5/2536 	0:00:00
cusrc-4.zip 	91 KB 	12/5/2536 	0:00:00

No modification in code except change line function in \COMMON\CSCR.C
to _line, MAKEFILE and TURBOC.CFG.

Use Thai Industrial Standard 620-2533 (TIS-620) for character encoding
