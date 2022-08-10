.device ATMega328p

.dseg ; Data segment
.org 0x100

.DEF LIST_END = R17
.DEF POS_A = R18
.DEF POS_B = R19
.DEF CANDIDATE = R20


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

; Carrega SRAM com ARRAY (code mem)
.MACRO FILL_FROM_CMEM
         LDI R30,LOW(ARRAY<<1)
         LDI R31,HIGH(ARRAY<<1)
         LDI R16,0x00
         LPM R1,Z+
         ST X+,R1
         INC R16
LOAD_LOOP:
          LPM R1,Z+
          ST X+,R1
          INC LIST_END
          INC R16
          CPI R16,INITIAL_ARRAY_SIZE
          BRNE LOAD_LOOP
.ENDMACRO

.cseg
.org 0x00
rjmp INIT

INIT: CFG_SRAM
      CFG_STACK
      FILL_FROM_CMEM
      LDI CANDIDATE,0x06
      CALL ORDERLY_INSERT ; Carrega valor no registrador candidato
      LDI CANDIDATE,0x03 ;
      CALL ORDERLY_INSERT
      RJMP LOOP


; Uses R16, POS_A, POS_B, CANDIDATE and calls CHANGE_POSITION
ORDERLY_INSERT:
      INC LIST_END
      MOV XL,LIST_END ; Carrega o final da lista
      MOV POS_A,LIST_END ; Salva a posição do valor inserido
      ST X,CANDIDATE ; Insere o valor no final da lista
SWITCH_POS_LOOP:
      LD R16,-X ; Carrega o último valor da lista
      MOV POS_B,XL ; Salva a posição atual
      CP R16,CANDIDATE ; Compara se o último valor é igual ou maior
      BRSH HANDLER_END ; Se for, termina
      CALL CHANGE_POSITION ; Senão, troca posição
      MOV POS_A,POS_B ; Troca a posicao
      CPI POS_A,0x00 ; Se a posição atual for 0, acaba.
      BREQ HANDLER_END
      RJMP SWITCH_POS_LOOP ; Senão, reinicia


HANDLER_END:
      RET
; Uses POS_A, POS_B and R16
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
      RET


LOOP: RJMP LOOP

.equ INITIAL_ARRAY_SIZE = 3
ARRAY: .db 4,1,0
