;                   Font macro definition module for TURBO C
;                             AT&T Adaptor
;
;                           Kraisorn Liansee
;               Date    :     12/01/88
;               Updated :     12/01/88



    shift_down        macro  reg
                      local br1
                      add &reg&,2000h
                      jns br1
                      add &reg&,8050h
    br1:
                      endm

    shift_up          macro  reg
                      local br1
                      sub &reg&,2000h
                      jns br1
                      add &reg&,8000h - 050h
    br1:
                      endm
