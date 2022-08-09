.DEF LIST_POS = R17
.DEF POS_A = R18
.DEF POS_B = R19


.MACRO CFG_SRAM
       LDI XL,0x00
       LDI XH,0x01
.ENDMACRO

.MACRO CFG_STACK
     LDI R16,HIGH(RAMEND)
     OUT SPH,R16
     LDI R16,LOW(RAMEND)
     OUT SPL,R16
.ENDMACRO

.MACRO FILL_RAM
      LDI R16,0x00

LOOP: ST X+,R16
      INC R16,
      CPI R16,0x08
      BRNE LOOP
.ENDMACRO


.device ATMega328p
.org 0x00
rjmp INIT

INIT: CFG_SRAM
      CFG_STACK
      FILL_RAM
      LDI POS_A,0x00
      LDI POS_B,0x03


CHANGE_POSITION:
      MOV XL,POS_A ; Carrega POS_A
      LD R16,X ; Carrega em R16 conteúdo de POS_A
      PUSH R16 ; Bota conteúdo de POS_A no stack
      MOV XL,POS_B ; Carrega POS_B
      LD R16,X ; Carrega em R16 conteúdo de POS_B
      MOV XL,POS_A ; Carrega POS_A
      ST X,R16 ; Carrega conteúdo de R16 em POS_A
      POP R16 ; Tira conteúdo (original) de POS_A do stack
      MOV XL,POS_B ; Carrega POS_B
      ST X,R16 ; Carrega conteúdo de R16 em POS_B


TRUE_LOOP: RJMP TRUE_LOOP
