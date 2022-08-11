.device ATMega328p

.equ INPORT0 = 2
.equ INPORT1 = 3
.equ INPORT2 = 4

.equ OC0A_LOW = 0x36
.equ OC0A_HIGH = 0x35

.equ CARTAO = 0b00000110

.DEF LIST_END = R17
.DEF POS_A = R18
.DEF POS_B = R19
.DEF CANDIDATE = R20
.DEF LED_STATUS = R21

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

.MACRO CFG_INPUT_PINS
       LDI R16,(1<<PD6)
       OUT DDRD,R16
.ENDMACRO

.MACRO CFG_OUTPUT_PINS
       LDI R16,(1<<PC0)|(1<<PC1)|(1<<PC2)
       OUT DDRC,R16
       LDI R16,0x00
       OUT PORTC,R21
.ENDMACRO

.MACRO CFG_LED
     LDI LED_STATUS,(1<<PB5)
     OUT DDRB,LED_STATUS
     OUT PORTB,LED_STATUS
.ENDMACRO

.MACRO CFG_TIMER
    CLR R16
    STS TCNT1L,R16
    STS TCNT1H,R16
    LDI R16,(1<<CS12)|(1<<CS10)|(1<<WGM12)
    STS TCCR1B,R16
    LDI R16,(1<<COM1A0)
    STS TCCR1A,R16
    LDI R16,OC0A_HIGH
    STS OCR1AH,R16
    LDI R16,OC0A_LOW
    STS OCR1AL,R16
    LDI R16,(1<<OCIE1A)
    STS TIMSK1,R16
.ENDMACRO


.MACRO CFG_TIMER_OCR
       CLR R16
       OUT TCNT0,R16
       OUT TCCR0A,R16
       OUT TCCR0B,R16
       LDI R16,(1<<WGM01)|(0<<WGM00)
       OUT TCCR0A,R16
       LDI R16,(1<<CS02)|(1<<CS00)|(0<<WGM02)
       OUT TCCR0B,R16
       LDI R16,0xFF
       OUT OCR0A,R16
.ENDMACRO

.dseg ; Data segment
.org 0x100
.cseg
.org 0x0000
RJMP INIT
.org 0x0016
rjmp BASIC

INIT: CFG_SRAM
      CFG_STACK
      CALL FILL_FROM_CMEM
      CFG_INPUT_PINS
      CFG_OUTPUT_PINS
      CFG_LED
      CFG_TIMER
      CFG_TIMER_OCR
      CALL UPDATE_OUTPUT
      SEI

LOOP: RJMP LOOP

FILL_FROM_CMEM:
         LDI LIST_END,0x00
         LDI R30,LOW(ARRAY<<1)
         LDI R31,HIGH(ARRAY<<1)
         LDI R16,0x00
         LPM R1,Z+
         ST X+,R1
         INC R16
         CPI R16,INITIAL_ARRAY_SIZE
         BREQ LOAD_END
LOAD_LOOP:
          LPM R1,Z+
          ST X+,R1
          INC LIST_END
          INC R16
          CPI R16,INITIAL_ARRAY_SIZE
          BRNE LOAD_LOOP
LOAD_END: RET

BASIC:
      CALL INSERT_INPUT
      CALL UPDATE_OUTPUT
      ;CALL TOGGLE_LED
      RETI

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

INSERT_INPUT:
      IN R16,PIND
      LDI CANDIDATE,0x00
      SBRC R16,INPORT2 ; Pula se PIND[INPORT2] estiver OFF
      ORI CANDIDATE,0b00000100  ; Seta PC2 ON
      SBRC R16,INPORT1 ; Pula se PIND[INPORT1] estiver OFF
      ORI CANDIDATE,0b00000010  ; Seta PC1 ON
      SBRC R16,INPORT0 ; Pula se PIND[INPORT0] estiver OFF
      ORI CANDIDATE,0b00000001  ; Seta PC0 ON
      CALL ORDERLY_INSERT

UPDATE_OUTPUT:
      LDI XL,0x00
      LD R16,X
      LDI R17,0x00
      SBRC R16,2 ; Pula se MAIOR[2] estiver OFF
      ORI R17,(1<<PC2)  ; Seta PC2 ON
      SBRC R16,1; Repete
      ORI R17,(1<<PC1)
      SBRC R16,0
      ORI R17,(1<<PC0)
      OUT PORTC,R17
      CPI R16,CARTAO
      BREQ TURN_ON
      BRNE TURN_OFF
      RET

TOGGLE_LED:
     CPI LED_STATUS,(1<<PD5)
     BREQ TURN_OFF
     RJMP TURN_ON

TURN_ON:
        LDI LED_STATUS,(1<<PB5)
        OUT PORTB,LED_STATUS
        LDI R16,(0<<COM0A1)|(1<<COM0A0)|(1<<WGM01)|(0<<WGM00)
        OUT TCCR0A,R16
        RET

TURN_OFF:
        LDI LED_STATUS,0x00
        OUT PORTB,LED_STATUS
        LDI R16,(1<<WGM01)|(0<<WGM00)
        OUT TCCR0A,R16
        RET

.equ INITIAL_ARRAY_SIZE = 1
ARRAY: .db 3



