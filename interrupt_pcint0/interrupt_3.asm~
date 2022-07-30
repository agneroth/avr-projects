.MACRO CFG_STACK
     LDI R16,HIGH(RAMEND) // R16 = RAMEND_HIGH
     OUT SPH,R16 // SPH = R16
     LDI R16,LOW(RAMEND) // ...
     OUT SPL,R16
.ENDMACRO

; configura portas digitais 8,9,13 como input pull-up
.MACRO CFG_PORT
     LDI R16,(1<<PCINT1)|(1<<PCINT0)|(1<<PB5)
     OUT DDRB,R16
     LDI R17,(1<<PCINT1)|(1<<PCINT0)|(1<<PB5)
     OUT PORTB,R17
.ENDMACRO

; configura exception nas portas digitais 0-7 para low-level
.MACRO CFG_HANDLER
     LDS R16,PCICR
     ORI R16,(1<<PCIE0)
     STS PCICR,R16
     ; Determina qual pino estará escutando
     LDS R17,PCMSK0
     ORI R17,(1<<PCINT1)|(1<<PCINT0)
     STS PCMSK0,R17
.ENDMACRO

; inicializa o led ligado
.MACRO CFG_LED
     LDI R16,(1<<PB5)
     OUT DDRB,R16
     LDI R18,(1<<PB5)
     OUT PORTB,R18
.ENDMACRO

// diz o microcontrolador especifico p/ o compilador
.device ATMega328p
// inicio da memoria de código
.org 0x0000
// pula pro inicio do código
rjmp   INIT
.org 0x0006
rjmp TURN_OFF



HANDLER:
     LDS R16,PCMSK0
     SBRS R16,0
     RJMP TURN_OFF
     LDS R16,PCMSK0
     SBRS R16,1
     RJMP TURN_ON
     RJMP HANDLER_END

HANDLER_MAIN:
     CPI R18,(1<<PB5)
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

INIT:
      CFG_LED
      CFG_STACK
      CFG_PORT
      CFG_HANDLER
      SEI

LOOP: RJMP LOOP

