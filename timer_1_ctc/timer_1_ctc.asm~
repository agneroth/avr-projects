.device ATMega328p

.MACRO CFG_TIMER
; habilita timer 1, configura CTC OCR1A
    LDI R16,(1<<CS12)|(1<<CS10)|(1<<WGM12)
    STS TCCR1B,R16
    LDI R16,0x00
    STS TCCR1A,R16
; configura OCR
    LDI R16,0xFF
    STS OCR1AH,R16
    LDI R16,0xFF
    STS OCR1AL,R16
; configura interrupt (overflow)
    LDI R16,(1<<TOIE1)
    STS TIMSK1,R16
.ENDMACRO

.MACRO CFG_LED
     LDI R18,0x20
     OUT DDRB,R18
     OUT PORTB,R18
.ENDMACRO

// diz o microcontrolador especifico p/ o compilador

// inicio da memoria de código
.org 0x0000
// pula pro inicio do código
rjmp   INIT
.org 0x001A
rjmp HANDLER


INIT: CFG_TIMER
      CFG_LED
      SEI

RJMP LOOP

LOOP: RJMP LOOP

HANDLER:
        CPI R18,0x20
        BREQ TURN_OFF   ; ta ligado vai pra desligar
        RJMP TURN_ON    ; ta desligado vai pra ligar

HANDLER_END:
     RETI

TURN_ON:
     LDI R18, 0x20        ; liga led
     OUT PORTB,R18
     rjmp HANDLER_END


TURN_OFF:                  ; desliga led
     LDI R18, 0x00
     OUT PORTB,R18
     rjmp HANDLER_END




