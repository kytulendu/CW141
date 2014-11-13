;****************************************************************************;
; DISP.ASM  3 JAN 89      /**** MODIFIED ****/                               ;
; FOR AT&T                                                                   ;
;****************************************************************************;
PUBLIC    _aprchar
PUBLIC    _aprblank
PUBLIC    _asetcurpos
PUBLIC    _aplot

PUBLIC    _asavepic
PUBLIC    _aretpic

PUBLIC    _aclsall
PUBLIC    _aclsgraph
PUBLIC    _aclrline
PUBLIC    _aprakeaw
PUBLIC    _ardchardot
PUBLIC    _awrchardot
PUBLIC    _aputwind
PUBLIC    _agetwind

INCLUDE   cwfntam.asm

DGROUP           GROUP     _DATA,_BSS
_DATA            SEGMENT   WORD PUBLIC 'DATA'

extrn _dfont:byte,_ditalicfont:byte,_protect1:byte
extrn    curfont:byte, enlargefont:byte, offsetfont:word
extrn     chulalogo1:word, chulalogo2:word, _chardot:byte
extrn _graph_seg:word
extrn _screen_buffptr:dword

_DATA            ENDS
_BSS             SEGMENT  WORD PUBLIC 'BSS'
_BSS             ENDS

ADISP_TEXT  SEGMENT   BYTE PUBLIC 'CODE'
           ASSUME    CS:ADISP_TEXT,DS:DGROUP
           extrn     _acompute_off:far
;=============================================================================;
; prchar                                                                      ;
;     print character at specified cursor position                            ;
;          ( don't forward cursor position )                                  ;
;     USAGE : prchar(char c,char attr,unsigned x,unsigned y)                  ;
;             Attribute : bit 0 -> single underline                           ;
;                         bit 1 -> reverse                                    ;
;                         bit 2 -> bold                                       ;
;                         bit 3 -> superscript                                ;
;                         bit 4 -> subscript                                  ;
;                         bit 5 -> italic                                     ;
;                         bit 6 -> enlarge                                    ;
;                         bit 7 -> double underline                           ;
;=============================================================================;
_aprchar proc     far
        push     bp
        push     si
        push     di
        push     es
        mov      bp,sp
        mov      ax,_graph_seg   ; display at page 0
        mov      es,ax
; ascii value * 20 is offset in font
; we will set DS:[BX+SI] point at font
        mov      si,[bp+12]   ; get character
        and      si,00ffh    ; clear high byte
        add      si,si       ; add for word offset
        add      si,offset DGROUP:offsetfont
        mov      si,ds:[si]  ; get offset from table
        mov      al,[bp+14]   ; get attribute
        or       al,al       ; Has it attribute ?
        jnz      haveattribute
        mov      bx,offset DGROUP:_dfont
        jmp      nochangefont
; if it's attribute -> change font
haveattribute:
        test     al,020h     ; italic ?
        jz       normalfont
        mov      bx,offset DGROUP:_ditalicfont
        jmp      findnextattr
normalfont:
        mov      bx,offset DGROUP:_dfont
findnextattr:
        mov      di,offset DGROUP:curfont
        test     al,008h     ; superscript ?
        jz       testsub
        mov      word ptr ds:[di+16],0  ; clear old font
        mov      word ptr ds:[di+18],0
        sub      di,3
testsub:
        test     al,010h     ; subscript ?
        jz       transferfont
        mov      word ptr ds:[di],0     ; clear old font
        mov      word ptr ds:[di+2],0
        add      di,3
transferfont:
        mov      ax,ds:[bx+si]          ; transfer font
        mov      ds:[di],ax             ; Looping not required,
        mov      ax,ds:[bx+si+2]        ; because of speed.
        mov      ds:[di+2],ax
        mov      ax,ds:[bx+si+4]
        mov      ds:[di+4],ax
        mov      ax,ds:[bx+si+6]
        mov      ds:[di+6],ax
        mov      ax,ds:[bx+si+8]
        mov      ds:[di+8],ax
        mov      ax,ds:[bx+si+10]
        mov      ds:[di+10],ax
        mov      ax,ds:[bx+si+12]
        mov      ds:[di+12],ax
        mov      ax,ds:[bx+si+14]
        mov      ds:[di+14],ax
        mov      ax,ds:[bx+si+16]
        mov      ds:[di+16],ax
        mov      ax,ds:[bx+si+18]
        mov      ds:[di+18],ax
        mov      bx,offset DGROUP:curfont ; base table is in BX
        xor      si,si         ; ds:[bx+si] now point at font
        mov      al,[bp+14]     ; get attribute
        test     al,004h       ; bold ?
        jz       chkunderline
        mov      cx,20
loopbold:
        mov      dl,ds:[bx+si]
        shr      dl,1
        or       ds:[bx+si],dl
        inc      si
        loop     loopbold
        xor      si,si
chkunderline:
        test     al,001h     ; single underline ?
        jz       chkdoubleunderline
        mov      byte ptr ds:[bx+17],0ffh
chkdoubleunderline:
        test     al,080h     ; double underline ?
        jz       chkreverse
        mov      byte ptr ds:[bx+16],0ffh
        mov      byte ptr ds:[bx+18],0ffh
chkreverse:
        test     al,002h     ; reverse ?
        jz       chkenlarge
        mov      al,[bp+12]   ; get char
        cmp      al,0d0h
        jbe      changerev
        cmp      al,0d1h
        jbe      chkenlarge
        cmp      al,0d3h
        jbe      changerev
        cmp      al,0dah
        jbe      chkenlarge
        cmp      al,0e7h
        jb       changerev
        cmp      al,0eeh
        jbe      chkenlarge
changerev:
        mov      dx,0ffffh
        xor      ds:[bx],dx
        xor      ds:[bx+2],dx
        xor      ds:[bx+4],dx
        xor      ds:[bx+6],dx
        xor      ds:[bx+8],dx
        xor      ds:[bx+10],dx
        xor      ds:[bx+12],dx
        xor      ds:[bx+14],dx
        xor      ds:[bx+16],dx
        xor      ds:[bx+18],dx
chkenlarge:
        test     byte ptr [bp+14],040h  ;enlarge ?
        jz       nochangefont
        mov      di,offset enlargefont
        mov      cx,20
twentybyte:
        mov      dx,0
        test     byte ptr ds:[bx+si],080h
        jz       testbit6
        or       dx,0000000011000000b
testbit6:
        test     byte ptr ds:[bx+si],040h
        jz       testbit5
        or       dx,0000000000110000b
testbit5:
        test     byte ptr ds:[bx+si],020h
        jz       testbit4
        or       dx,0000000000001100b
testbit4:
        test     byte ptr ds:[bx+si],010h
        jz       testbit3
        or       dx,0000000000000011b
testbit3:
        test     byte ptr ds:[bx+si],008h
        jz       testbit2
        or       dx,1100000000000000b
testbit2:
        test     byte ptr ds:[bx+si],004h
        jz       testbit1
        or       dx,0011000000000000b
testbit1:
        test     byte ptr ds:[bx+si],002h
        jz       testbit0
        or       dx,0000110000000000b
testbit0:
        test     byte ptr ds:[bx+si],001h
        jz       nomoretest
        or       dx,0000001100000000b
nomoretest:
        mov      ds:[di],dx
        inc      si
        add      di,2
        loop     twentybyte
        mov      bx,offset DGROUP:enlargefont
        xor      si,si
nochangefont:
; we will set ES:[DI] point at screen

;********  MODIFIED **********

; we will write character now ! (using standard code)
         mov      al,[bp+12]   ; get character
         cmp      al,0d0h
         ja       cont1
         jmp      middle_level
cont1:   cmp      al,0d1h
         jbe      abvlevel
         cmp      al,0d3h
         ja       cont2
         jmp      middle_level
cont2:   cmp      al,0d7h
         jbe      abvlevel
         cmp      al,0dah
         jbe      below_level
         cmp      al,0e6h
         ja       cont3
         jmp      middle_level
cont3:   cmp      al,0eeh
         jbe      abvlevel
         jmp      middle_level
abvlevel:
         jmp      above_level
; write below level character
; we will write last-four font only
below_level:
         mov      ax,[bp+18]
         mov      cl,20       ; 20 lines per row
         mul      cl
         add      ax,16+4        ; down 16 lines
         push     ax
         xor      ax,ax
         push     ax
         mov      di,bx       ;save in register variable
         call     _acompute_off  ; get offset in ax
         mov      sp,bp
         mov      bx,di
         mov      di,ax
         add      di,[bp+16]  ; add column
         mov      es,_graph_seg

         test     byte ptr [bp+14],040h    ;enlarge ?
         jz       noenlargebelow

         mov      ax,ds:[bx+si+32]
         xor      es:[di],ax
         shift_down di
         mov      ax,ds:[bx+si+34]
         xor      es:[di],ax
         mov      ax,ds:[bx+si+36]
         xor      es:[di],ax
         shift_down di
         mov      ax,ds:[bx+si+38]
         xor      es:[di],ax

         jmp      out_write
noenlargebelow:
         mov      al,ds:[bx+si+16]
         xor      es:[di],al
         shift_down di
         mov      al,ds:[bx+si+17]
         xor      es:[di],al
         shift_down di
         mov      al,ds:[bx+si+18]
         xor      es:[di],al
         shift_down di
         mov      al,ds:[bx+si+19]
         xor      es:[di],al
         jmp      out_write
; write middle level character
; we will write 12 byte only by ignore first-four & last-four font
noenlarge1:
         jmp      noenlarge
middle_level:
         mov      ax,[bp+18]
         mov      cl,20       ; 20 lines per row
         mul      cl
         add      ax,4
         push     ax
         xor      ax,ax
         push     ax
         mov      di,bx       ;save in register variable
         call     _acompute_off  ; get offset in ax
         mov      sp,bp
         mov      bx,di
         mov      di,ax
         add      di,[bp+16]  ; add column
         mov      es,_graph_seg

         test     byte ptr [bp+14],040h    ;enlarge ?
         jz       noenlarge1

         mov      ax,ds:[bx+si]
         mov      es:[di],ax
         shift_down di
         mov      ax,ds:[bx+si+2]
         mov      es:[di],ax
         shift_down di
         mov      ax,ds:[bx+si+4]
         mov      es:[di],ax
         shift_down di
         mov      ax,ds:[bx+si+6]
         mov      es:[di],ax
         shift_down di
         mov      ax,ds:[bx+si+8]
         mov      es:[di],ax
         shift_down di
         mov      ax,ds:[bx+si+10]
         mov      es:[di],ax
         shift_down di
         mov      ax,ds:[bx+si+12]
         mov      es:[di],ax
         shift_down di
         mov      ax,ds:[bx+si+14]
         mov      es:[di],ax
         shift_down di
         mov      ax,ds:[bx+si+16]
         mov      es:[di],ax
         shift_down di
         mov      ax,ds:[bx+si+18]
         mov      es:[di],ax
         shift_down di
         mov      ax,ds:[bx+si+20]
         mov      es:[di],ax
         shift_down di
         mov      ax,ds:[bx+si+22]
         mov      es:[di],ax
         shift_down di
         mov      ax,ds:[bx+si+24]
         mov      es:[di],ax
         shift_down di
         mov      ax,ds:[bx+si+26]
         mov      es:[di],ax
         shift_down di
         mov      ax,ds:[bx+si+28]
         mov      es:[di],ax
         shift_down di
         mov      ax,ds:[bx+si+30]
         mov      es:[di],ax
         shift_down di
         mov      ax,ds:[bx+si+32]
         mov      es:[di],ax
         shift_down di
         mov      ax,ds:[bx+si+34]
         mov      es:[di],ax
         shift_down di
         mov      ax,ds:[bx+si+36]
         mov      es:[di],ax
         shift_down di
         mov      ax,ds:[bx+si+38]
         mov      es:[di],ax

         jmp      out_write
noenlarge:
         mov      al,ds:[bx+si]
         mov      es:[di],al
         shift_down di
         mov      al,ds:[bx+si+1]
         mov      es:[di],al
         shift_down di
         mov      al,ds:[bx+si+2]
         mov      es:[di],al
         shift_down di
         mov      al,ds:[bx+si+3]
         mov      es:[di],al
         shift_down di
         mov      al,ds:[bx+si+4]
         mov      es:[di],al
         shift_down di
         mov      al,ds:[bx+si+5]
         mov      es:[di],al
         shift_down di
         mov      al,ds:[bx+si+6]
         mov      es:[di],al
         shift_down di
         mov      al,ds:[bx+si+7]
         mov      es:[di],al
         shift_down di
         mov      al,ds:[bx+si+8]
         mov      es:[di],al
         shift_down di
         mov      al,ds:[bx+si+9]
         mov      es:[di],al
         shift_down di
         mov      al,ds:[bx+si+10]
         mov      es:[di],al
         shift_down di
         mov      al,ds:[bx+si+11]
         mov      es:[di],al
         shift_down di
         mov      al,ds:[bx+si+12]
         mov      es:[di],al
         shift_down di
         mov      al,ds:[bx+si+13]
         mov      es:[di],al
         shift_down di
         mov      al,ds:[bx+si+14]
         mov      es:[di],al
         shift_down di
         mov      al,ds:[bx+si+15]
         mov      es:[di],al
         shift_down di
         mov      al,ds:[bx+si+16]
         mov      es:[di],al
         shift_down di
         mov      al,ds:[bx+si+17]
         mov      es:[di],al
         shift_down di
         mov      al,ds:[bx+si+18]
         mov      es:[di],al
         shift_down di
         mov      al,ds:[bx+si+19]
         mov      es:[di],al

         jmp      out_write
; above level has 2 level ,above-middle & topest
; above-middle level has ascii code < 0e4h
; topest level has ascii code >= 0e4h
above_level:

         cmp      al,0e6h
         jae      cont4
         jmp      above_middle
cont4:   cmp      al,0edh
         jb       cont5
         jmp      above_middle

cont5:   mov      ax,[bp+18]
         mov      cl,20       ; 20 lines per row
         mul      cl
         add      ax,4
         push     ax
         xor      ax,ax
         push     ax
         mov      di,bx       ;save in register variable
         call     _acompute_off  ; get offset in ax
         mov      sp,bp
         mov      bx,di
         mov      di,ax
         add      di,[bp+16]  ; add column
         mov      es,_graph_seg

         test     byte ptr [bp+14],040h    ;enlarge ?
         jz       noenlargetop

         mov      ax,ds:[bx+si]
         xor      es:[di],ax
         shift_down di
         mov      ax,ds:[bx+si+2]
         xor      es:[di],ax
         shift_down di
         mov      ax,ds:[bx+si+4]
         xor      es:[di],ax
         shift_down di
         mov      ax,ds:[bx+si+6]
         xor      es:[di],ax

         jmp      out_write
noenlargetop:
         mov      al,ds:[bx+si]
         xor      es:[di],al
         shift_down di
         mov      al,ds:[bx+si+1]
         xor      es:[di],al
         shift_down di
         mov      al,ds:[bx+si+2]
         xor      es:[di],al
         shift_down di
         mov      al,ds:[bx+si+3]
         xor      es:[di],al

         jmp      out_write

above_middle:

         mov      ax,[bp+18]
         mov      cl,20       ; 20 lines per row
         mul      cl
         add      ax,4+4      ; down 4 lines
         push     ax
         xor      ax,ax
         push     ax
         mov      di,bx       ;save in register variable
         call     _acompute_off  ; get offset in ax
         mov      sp,bp
         mov      bx,di
         mov      di,ax
         add      di,[bp+16]  ; add column
         mov      es,_graph_seg

         test     byte ptr [bp+14],040h    ;enlarge ?
         jz       noenlargeabove

         mov      ax,ds:[bx+si+8]
         xor      es:[di],ax
         shift_down di
         mov      ax,ds:[bx+si+10]
         xor      es:[di],ax
         shift_down di
         mov      ax,ds:[bx+si+12]
         xor      es:[di],ax
         shift_down di
         mov      ax,ds:[bx+si+14]
         xor      es:[di],ax
         jmp      out_write

noenlargeabove:
         mov      al,ds:[bx+si+4]
         xor      es:[di],al
         shift_down di
         mov      al,ds:[bx+si+5]
         xor      es:[di],al
         shift_down di
         mov      al,ds:[bx+si+6]
         xor      es:[di],al
         shift_down di
         mov      al,ds:[bx+si+7]
         xor      es:[di],al
out_write:
         pop      es
         pop      di
         pop      si
         pop      bp
         ret
_aprchar  endp
;=============================================================================;
; prblank                                                                     ;
;     print blank character at specified cursor position                      ;
;          ( don't forward cursor position )                                  ;
;     USAGE : prblank(unsigned x,unsigned y)                                  ;
;=============================================================================;
_aprblank proc     far
         push     bp
         push     di
         push     es
         mov      bp,sp

; we will set ES:[DI] point at screen

         mov      ax,[bp+12]
         mov      cl,20       ; 20 lines per row
         mul      cl
         add      ax,4
         push     ax
         xor      ax,ax
         push     ax
         call     _acompute_off  ; get offset in ax
         mov      sp,bp
         mov      di,ax
         add      di,[bp+10]  ; add column
         mov      es,_graph_seg

         xor      al,al
         mov      es:[di],al
         shift_down di
         mov      es:[di],al
         shift_down di
         mov      es:[di],al
         shift_down di
         mov      es:[di],al
         shift_down di
         mov      es:[di],al
         shift_down di
         mov      es:[di],al
         shift_down di
         mov      es:[di],al
         shift_down di
         mov      es:[di],al
         shift_down di
         mov      es:[di],al
         shift_down di
         mov      es:[di],al
         shift_down di
         mov      es:[di],al
         shift_down di
         mov      es:[di],al
         shift_down di
         mov      es:[di],al
         shift_down di
         mov      es:[di],al
         shift_down di
         mov      es:[di],al
         shift_down di
         mov      es:[di],al
         shift_down di
         mov      es:[di],al
         shift_down di
         mov      es:[di],al
         shift_down di
         mov      es:[di],al
         shift_down di
         mov      es:[di],al

         pop      es
         pop      di
         pop      bp
         ret
_aprblank endp
;=============================================================================;
; setcurpos                                                                   ;
;    xor cursor at specified cursor position                                  ;
;    USAGE : setcurpos(unsigned x,unsigned y,int thaimode)                    ;
;=============================================================================;
_asetcurpos proc     far
           push     bp
           push     di
           push     es
           mov      bp,sp


; we will set ES:[DI] point at screen

; xor 12 byte only by ignore first-four & last-four byte
           cmp      byte ptr [bp+14],1      ; thai mode ?
           je      thaimode
           jmp     englishmode
thaimode:
         mov      ax,[bp+12]
         mov      cl,20       ; 20 lines per row
         mul      cl
         add      ax,4+4        ; down 4 lines
         push     ax
         xor      ax,ax
         push     ax
         call     _acompute_off  ; get offset in ax
         mov      sp,bp
         mov      di,ax
         add      di,[bp+10]  ; add column
         mov      es,_graph_seg

           xor      byte ptr es:[di],0ffh
           shift_down di
           xor      byte ptr es:[di],0ffh
           shift_down di
           xor      byte ptr es:[di],0ffh
           shift_down di
           xor      byte ptr es:[di],0ffh
           shift_down di
           xor      byte ptr es:[di],0ffh
           shift_down di
           xor      byte ptr es:[di],0ffh
           shift_down di
           xor      byte ptr es:[di],0ffh
           shift_down di
           xor      byte ptr es:[di],0ffh
           shift_down di
           xor      byte ptr es:[di],0ffh
           shift_down di
           xor      byte ptr es:[di],0ffh
           shift_down di
           xor      byte ptr es:[di],0ffh
           shift_down di
           xor      byte ptr es:[di],0ffh
           jmp      outsetcurpos
englishmode:

         mov      ax,[bp+12]
         mov      cl,20       ; 20 lines per row
         mul      cl
         add      ax,16+4     ; down 16 lines + 4 align for factor
         push     ax
         xor      ax,ax
         push     ax
         call     _acompute_off  ; get offset in ax
         mov      sp,bp
         mov      di,ax
         add      di,[bp+10]  ; add column
         mov      es,_graph_seg

           xor      byte ptr es:[di],0ffh
           shift_down di
           xor      byte ptr es:[di],0ffh
outsetcurpos:
           pop      es
           pop      di
           pop      bp
           ret
_asetcurpos endp
;=============================================================================;
; plot                                                                        ;
;     plot dot by find position as follow                                     ;
;     find byte position from formula                                         ;
;          2000h*(Y mod 4) + 90*integer(Y/4) + integer(X/8)                   ;
;     find bit position in byte from formula                                  ;
;          7 - (x mod 8)                                                      ;
;     USAGE : plot(unsigned x,unsigned y)                                     ;
;=============================================================================;
_aplot proc     far
      push     bp
      push     di
      push     es
      mov      bp,sp
;
      push     [bp+12]
      mov      di, [bp+10]
      push     di
      call     _acompute_off
      mov      sp,bp
      mov      cx,di
      mov      di,ax
      mov      es,_graph_seg

      and      cl,7
      mov      al,80H
      shr      al,cl
      or       byte ptr es:[di],al
;
      pop      es
      pop      di
      pop      bp
      ret
_aplot endp

;=============================================================================;
; savepic                                                                     ;
;     save picture from screen memory page 1 to page 0                        ;
;=============================================================================;
_asavepic   proc far
           push si
           push di
           push ds
           push es

           les  di,_screen_buffptr

           mov  ds,_graph_seg
           xor  si,si

           mov  dx,348
           cld
loop1:     mov  bx,si
           mov  cx,40
           rep  movsw
           mov  si,bx
           shift_down si
           dec  dx
           jnz  loop1

           pop  es
           pop  ds
           pop  di
           pop  si
           ret
_asavepic   endp
;=============================================================================;
; retpic                                                                      ;
;     return picture from screen memory page 0 to page 1                      ;
;=============================================================================;
_aretpic proc far
        push si
        push di
        push ds
        push es

        mov  es,_graph_seg
        xor  di,di

        lds  si,_screen_buffptr

           mov  dx,348
           cld
loop2:     mov  bx,di
           mov  cx,40
           rep  movsw
           mov  di,bx
           shift_down di
           dec  dx
           jnz  loop2

        pop  es
        pop  ds
        pop  di
        pop  si
        ret
_aretpic endp
;=============================================================================;
; clsall                                                                      ;
;       clear screen memory page 1 to blank                                   ;
;=============================================================================;
_aclsall proc  far
        push  di
        push  es

        mov   ax,_graph_seg
        mov   es,ax
        xor   di,di
        mov   cx,4000h
        mov   ax,00
        cld
        rep   stosw

        pop   es
        pop   di
        ret
_aclsall endp
;=============================================================================;
; clsgraph                                                                    ;
;     clear graphic screen for window specified                               ;
; USAGE : clsgraph(int x1,int y1,int x2,int y2)                               ;
;=============================================================================;
_aclsgraph  proc  far
           push  bp
           push  di
           push  es
           mov   bp,sp

; we will set ES:[DI] point at screen
         mov      ax,[bp+12]
         mov      cl,20       ; 20 lines per row
         mul      cl
         add      ax,4
         push     ax
         xor      ax,ax
         push     ax
         call     _acompute_off  ; get offset in ax
         mov      sp,bp
         mov      di,ax
         mov      es,_graph_seg


           add   di,[bp+10]  ; add x1
           mov   cx,[bp+16] ; get y2
           sub   cx,[bp+12]  ; sub with y1
           inc   cx         ; CX = no. of row to clear
           mov   ax,cx
           mov   cl,20
           mul   cl
           mov   cx,ax
           xor   ax,ax

           mov   bx,[bp+14] ; get x2
           sub   bx,[bp+10]  ; sub with x1
           inc   bx         ; BX = no. of column to clear
           mov   dx,cx      ; DX = no of lines
           cld

clearrow:  dec   dx
           js   endclsgraph

           push  di
           mov   cx,bx
           rep   stosb
           pop   di
           shift_down di
           jmp short clearrow;

endclsgraph:     pop  es
                 pop  di
                 pop  bp
                 ret
_aclsgraph  endp
;=============================================================================;
; clrline                                                                     ;
;     clear line y1 from x1 to x2                                             ;
; USAGE : clrline(int x1,int y1,int x2)                                       ;
;=============================================================================;
_aclrline   proc  far
           push  bp
           push  di
           push  es
           mov   bp,sp
; we will set ES:[DI] point at screen
         mov      ax,[bp+12]
         mov      cl,20       ; 20 lines per row
         mul      cl
         add      ax,4
         push     ax
         xor      ax,ax
         push     ax
         call     _acompute_off  ; get offset in ax
         mov      sp,bp
         mov      di,ax
         mov      es,_graph_seg

           add   di,[bp+10]  ; add x1
           mov   ax,0
           mov   bx,[bp+14]  ; get x2
           sub   bx,[bp+10]   ; sub with x1
           inc   bx          ;BX = number of bytes

           mov   dx,20
           cld
clearline: dec   dx
           js   endclearline

           push  di
           mov   cx,bx
           rep   stosb
           pop   di
           shift_down di
           jmp short clearline
endclearline:
           pop   es
           pop   di
           pop   bp
           ret
_aclrline   endp

;=============================================================================;
; prakeaw                                                                     ;
;       write parkeaw at upper left cornor                                    ;
;=============================================================================;
_aprakeaw proc  far
         push  si
         push  di
         push  es
         mov   es,_graph_seg
         mov   si,offset DS:chulalogo1
         mov   di,1
         shift_down di
         shift_down di
         mov   ax,ds:[si]
         mov   es:[di],ax
         shift_down di
         mov   ax,ds:[si+2]
         mov   es:[di],ax
         shift_down di
;
         mov   ax,ds:[si+4]
         mov   es:[di],ax
         shift_down di
         mov   ax,ds:[si+6]
         mov   es:[di],ax
         shift_down di
         mov   ax,ds:[si+8]
         mov   es:[di],ax
         shift_down di
         mov   ax,ds:[si+10]
         mov   es:[di],ax
         shift_down di
;
         mov   ax,ds:[si+12]
         mov   es:[di],ax
         shift_down di
         mov   ax,ds:[si+14]
         mov   es:[di],ax
         shift_down di
         mov   ax,ds:[si+16]
         mov   es:[di],ax
         shift_down di
         mov   ax,ds:[si+18]
         mov   es:[di],ax
         shift_down di
;
         mov   ax,ds:[si+20]
         mov   es:[di],ax
         shift_down di
         mov   ax,ds:[si+22]
         mov   es:[di],ax
         shift_down di
         mov   ax,ds:[si+24]
         mov   es:[di],ax
         shift_down di
         mov   ax,ds:[si+26]
         mov   es:[di],ax
         shift_down di
         mov   al,1
         mov   ds:_protect1[0],al

;
         mov   ax,ds:[si+28]
         mov   es:[di],ax
         shift_down di
         mov   ax,ds:[si+30]
         mov   es:[di],ax
         shift_down di
         mov   ax,ds:[si+32]
         mov   es:[di],ax
         shift_down di
         mov   ax,ds:[si+34]
         mov   es:[di],ax
         shift_down di
;
         mov   ax,ds:[si+36]
         mov   es:[di],ax
         shift_down di
         mov   ax,ds:[si+38]
         mov   es:[di],ax
;
         mov   si,offset DS:chulalogo2
         mov   di,3
         shift_down di
         shift_down di
         mov   ax,ds:[si]
         mov   es:[di],ax
         shift_down di
         mov   ax,ds:[si+2]
         mov   es:[di],ax
         shift_down di
;
         mov   ax,ds:[si+4]
         mov   es:[di],ax
         shift_down di
         mov   ax,ds:[si+6]
         mov   es:[di],ax
         shift_down di
         mov   ax,ds:[si+8]
         mov   es:[di],ax
         shift_down di
         mov   ax,ds:[si+10]
         mov   es:[di],ax
         shift_down di
;
         mov   ax,ds:[si+12]
         mov   es:[di],ax
         shift_down di
         mov   ax,ds:[si+14]
         mov   es:[di],ax
         shift_down di
         mov   ax,ds:[si+16]
         mov   es:[di],ax
         shift_down di
         mov   ax,ds:[si+18]
         mov   es:[di],ax
         shift_down di
;
         mov   ax,ds:[si+20]
         mov   es:[di],ax
         shift_down di
         mov   ax,ds:[si+22]
         mov   es:[di],ax
         shift_down di
         mov   ax,ds:[si+24]
         mov   es:[di],ax
         shift_down di
         mov   ax,ds:[si+26]
         mov   es:[di],ax
         shift_down di
;
         mov   ax,ds:[si+28]
         mov   es:[di],ax
         shift_down di
         mov   ax,ds:[si+30]
         mov   es:[di],ax
         shift_down di
         mov   ax,ds:[si+32]
         mov   es:[di],ax
         shift_down di
         mov   ax,ds:[si+34]
         mov   es:[di],ax
         shift_down di
;
         mov   ax,ds:[si+36]
         mov   es:[di],ax
         shift_down di
         mov   ax,ds:[si+38]
         mov   es:[di],ax

         pop   es
         pop   di
         pop   si
         ret
_aprakeaw endp


_ardchardot proc     far
           push     bp
           push     si
           push     di
           push     es
           mov      bp,sp

         mov      ax,[bp+14]
         mov      cl,20       ; 20 lines per row
         mul      cl
         push     ax
         xor      ax,ax
         push     ax
         call     _acompute_off  ; get offset in ax
         mov      sp,bp
         add      ax,[bp+12]
         mov      di,ax
         mov      es,_graph_seg

           mov      si,offset DGROUP:_chardot
           mov      al,es:[di]
           mov      ds:[si],al
           shift_down di
           mov      al,es:[di]
           mov      ds:[si+1],al
           shift_down di
           mov      al,es:[di]
           mov      ds:[si+2],al
           shift_down di
           mov      al,es:[di]
           mov      ds:[si+3],al
           shift_down di
           mov      al,es:[di]
           mov      ds:[si+4],al
           shift_down di
           mov      al,es:[di]
           mov      ds:[si+5],al
           shift_down di
           mov      al,es:[di]
           mov      ds:[si+6],al
           shift_down di
           mov      al,es:[di]
           mov      ds:[si+7],al
           shift_down di
           mov      al,es:[di]
           mov      ds:[si+8],al
           shift_down di
           mov      al,es:[di]
           mov      ds:[si+9],al
           shift_down di
           mov      al,es:[di]
           mov      ds:[si+10],al
           shift_down di
           mov      al,es:[di]
           mov      ds:[si+11],al
           shift_down di
           mov      al,es:[di]
           mov      ds:[si+12],al
           shift_down di
           mov      al,es:[di]
           mov      ds:[si+13],al
           shift_down di
           mov      al,es:[di]
           mov      ds:[si+14],al
           shift_down di
           mov      al,es:[di]
           mov      ds:[si+15],al
           shift_down di
           mov      al,es:[di]
           mov      ds:[si+16],al
           shift_down di
           mov      al,es:[di]
           mov      ds:[si+17],al
           shift_down di
           mov      al,es:[di]
           mov      ds:[si+18],al
           shift_down di
           mov      al,es:[di]
           mov      ds:[si+19],al

           pop      es
           pop      di
           pop      si
           pop      bp
           ret
_ardchardot endp


_awrchardot proc     far
           push     bp
           push     si
           push     di
           push     es
           mov      bp,sp

         mov      ax,[bp+14]
         mov      cl,20       ; 20 lines per row
         mul      cl
         push     ax
         xor      ax,ax
         push     ax
         call     _acompute_off  ; get offset in ax
         mov      sp,bp
         add      ax,[bp+12]
         mov      di,ax
         mov      es,_graph_seg

           mov      si,offset DGROUP:_chardot
           mov      al,ds:[si]
           mov      es:[di],al
           shift_down di
           mov      al,ds:[si+1]
           mov      es:[di],al
           shift_down di
           mov      al,ds:[si+2]
           mov      es:[di],al
           shift_down di
           mov      al,ds:[si+3]
           mov      es:[di],al
           shift_down di
           mov      al,ds:[si+4]
           mov      es:[di],al
           shift_down di
           mov      al,ds:[si+5]
           mov      es:[di],al
           shift_down di
           mov      al,ds:[si+6]
           mov      es:[di],al
           shift_down di
           mov      al,ds:[si+7]
           mov      es:[di],al
           shift_down di
           mov      al,ds:[si+8]
           mov      es:[di],al
           shift_down di
           mov      al,ds:[si+9]
           mov      es:[di],al
           shift_down di
           mov      al,ds:[si+10]
           mov      es:[di],al
           shift_down di
           mov      al,ds:[si+11]
           mov      es:[di],al
           shift_down di
           mov      al,ds:[si+12]
           mov      es:[di],al
           shift_down di
           mov      al,ds:[si+13]
           mov      es:[di],al
           shift_down di
           mov      al,ds:[si+14]
           mov      es:[di],al
           shift_down di
           mov      al,ds:[si+15]
           mov      es:[di],al
           shift_down di
           mov      al,ds:[si+16]
           mov      es:[di],al
           shift_down di
           mov      al,ds:[si+17]
           mov      es:[di],al
           shift_down di
           mov      al,ds:[si+18]
           mov      es:[di],al
           shift_down di
           mov      al,ds:[si+19]
           mov      es:[di],al

           pop      es
           pop      di
           pop      si
           pop      bp
           ret
_awrchardot endp


;------------------------------------------------------
;  Fic_getwind routine
;
;      fic_getwind (x1,y1,linecount,bytecount,buffptr);
;
;
;        copy window area at x1,y1 (left-top)
;             size = linecount * bytecount
;        to buffer area
;------------------------------------------------------



     fic_getwind_stack    struc

                          dw    5 dup (?)    ;five regs saved
                          dd    ?            ;return address

     x1_9                 dw    ?
     y1_9                 dw    ?
     linecount_9          dw    ?
     bytecount_9          dw    ?
     buffptr_9            dd    ?

     fic_getwind_stack    ends


   _agetwind              proc far
                          push bp
                          push ds
                          push es
                          push si
                          push di
                          mov bp,sp

                          mov ax,[bp].y1_9       ;offset of source window
                          mov bl,20
                          mul bl
                          add ax,4
                          push ax
                          xor ax,ax
                          push ax
                          call _acompute_off
                          mov sp,bp
                          add ax,[bp].x1_9
                          mov bx,ax
                          mov ds,_graph_seg
                          les di,[bp].buffptr_9

                          cld
   scanline_loop9:        dec [bp].linecount_9
                          js end_getwind9
                          mov si,bx
                          mov cx,[bp].bytecount_9
                          rep movsb
			  shift_down bx
                          jmp short scanline_loop9
   end_getwind9:          pop di
                          pop si
                          pop es
                          pop ds
                          pop bp
                          ret
   _agetwind              endp


;------------------------------------------------------
;  Fic_putwind routine
;
;      fic_putwind (x1,y1,linecount,bytecount,buffptr);
;
;
;        copy window area at x1,y1 (left-top)
;             size = linecount * bytecount
;        from buffer area
;------------------------------------------------------



     fic_putwind_stack    struc

                          dw    5 dup (?)    ;five regs saved
                          dd    ?            ;return address

     x1_10                dw    ?
     y1_10                dw    ?
     linecount_10         dw    ?
     bytecount_10         dw    ?
     buffptr_10           dd    ?

     fic_putwind_stack    ends


   _aputwind              proc far
                          push bp
                          push ds
                          push es
                          push si
                          push di
                          mov bp,sp

                          mov ax,[bp].y1_10       ;offset of source window
                          mov bl,20
                          mul bl
                          add ax,4
                          push ax
                          xor ax,ax
                          push ax
                          call _acompute_off
                          mov sp,bp
                          add ax,[bp].x1_10
                          mov bx,ax
                          mov es,_graph_seg
                          lds si,[bp].buffptr_10

                          cld
   scanline_loop10:       dec [bp].linecount_10
                          js end_putwind10
                          mov di,bx
                          mov cx,[bp].bytecount_10
                          rep movsb
			  shift_down bx
                          jmp short scanline_loop10
   end_putwind10:         pop di
                          pop si
                          pop es
                          pop ds
                          pop bp
                          ret
   _aputwind              endp

ADISP_TEXT ENDS
          END

