;                   Font macro definition module for TURBO C
;                             EGA/VGA  Adaptor
;
;                           Kraisorn Liansee
;               Date    :     12/01/88
;               Updated :     12/01/88


    shift_down            macro reg
                          add &reg&,80
                          endm

    shift_up              macro reg
                          sub &reg&,80
                          endm

