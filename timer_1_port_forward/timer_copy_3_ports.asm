.device ATMega328p
.org 0x0000
RJMP INIT
.org 0x001A
rjmp BASIC


.equ INPORT0 = 2
.equ INPORT1 = 3
.equ INPORT2 = 4


.def CANDIDATE = R20

.MACRO CFG_INPUT_PINS
       LDI R22,0x00
       OUT DDRD,R16
.ENDMACRO

.MACRO CFG_OUTPUT_PINS
       LDI R21,(1<<PC0)|(1<<PC1)|(1<<PC2)
       OUT DDRC,R21
       LDI R21,0x00
       OUT PORTC,R21
.ENDMACRO

.MACRO CFG_LED
     LDI R17,(1<<PB5)
     OUT DDRB,R17
     OUT PORTB,R17
.ENDMACRO

.MACRO CFG_TIMER
; habilita timer 1
    LDI R16,(1<<CS12)
    STS TCCR1B,R16
; configura modo normal
    LDI R16,0x00
    STS TCCR1A,R16
; configura interrupt (overflow)
    LDI R16,(1<<TOIE1)
    STS TIMSK1,R16
.ENDMACRO

INIT: CFG_INPUT_PINS
      CFG_OUTPUT_PINS
      CFG_LED
      CFG_TIMER
      LDI CANDIDATE,0x00
      OUT PORTC,CANDIDATE
      SEI

LOOP: RJMP LOOP


BASIC:

; NO NANO -> PA1 corresponde ao segundo bit (iniciando em 0) de PORTC
      IN R16,PIND
      LDI R17,0x00
      SBRC R16,INPORT2 ; Pula se PIND[INPORT2] estiver OFF
      ORI R17,(1<<PC2)  ; Seta PC2 ON
      SBRC R16,INPORT1 ; Pula se PIND[INPORT1] estiver OFF
      ORI R17,(1<<PC1)  ; Seta PC1 ON
      SBRC R16,INPORT0 ; Pula se PIND[INPORT0] estiver OFF
      ORI R17,(1<<PC0)  ; Seta PC0 ON


      OUT PORTC,R17
      RETI

TURN_ON:
        LDI R17,(1<<PB5)
        OUT PORTB,R17
        RET

TURN_OFF:
        LDI R17,0x00
        OUT PORTB,R17
        RET


