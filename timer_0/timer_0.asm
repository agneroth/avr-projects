.include "m328Pdef.inc"

.MACRO CFG_TIMER
       ; Timer 0 - 8 bits - TCNT0
       ; Internal clock/1024 -> CS02:0 = 101 (register TCCR0B)
       ; CTC -> WGM02:0 = 010 (registers TCCR0A and TCCR0B)
       ; Count to 0xFF -> OCR0A = 0xFF
       ; On compare match OCR0A toggle pin6 -> COM0A1:0 = 01 (register TCCR0A)
       CLR R16
       OUT TCNT0,R16
       OUT TCCR0A,R16
       OUT TCCR0B,R16
       LDI R16,(1<<COM0A0)|(0<<WGM00)
       OUT TCCR0A,R16
       LDI R16,(1<<CS02)|(1<<CS00)
       OUT TCCR0B,R16
       LDI R16,0xFF
       OUT OCR0A,R16
       LDI R16,(1<<OCIE0A)
       STS TIMSK0,R16
       LDI R16,0xFF
       OUT DDRD,R16
.ENDMACRO

.MACRO CFG_LED
     LDI R18,(1<<PB5)
     OUT DDRB,R18
     OUT PORTB,R18
     LDI R16,0xFF
     OUT DDRD,R16
.ENDMACRO


.org 0x0000
RJMP INIT

INIT:
CFG_LED
CFG_TIMER
SEI


LOOP: RJMP LOOP

.org 0x001C
RJMP HANDLER



HANDLER:
     CPI R18,0x20
     BREQ TURN_OFF
     RJMP TURN_ON

HANDLER_END:
     RETI

TURN_ON:
     LDI R18,(1<<PB5)        ; liga led
     OUT PORTB,R18
     rjmp HANDLER_END


TURN_OFF:                  ; desliga led
     LDI R18, 0x00
     OUT PORTB,R18
     rjmp HANDLER_END

