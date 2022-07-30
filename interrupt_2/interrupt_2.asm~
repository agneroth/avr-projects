.MACRO CFG_STACK
     LDI R16,HIGH(RAMEND) // R16 = RAMEND_HIGH
     OUT SPH,R16 // SPH = R16
     LDI R16,LOW(RAMEND) // ...
     OUT SPL,R16
.ENDMACRO

; configura porta digital 3 como input pull-up
.MACRO CFG_PORT
     LDI R16,(1<<PD3)
     OUT DDRD,R16
     LDI R17,(1<<PD3)
     OUT PORTD,R17
.ENDMACRO

; configura exception na porta digital 3 para low-level
.MACRO CFG_HANDLER
     LDI R16,0x00
     sts EICRA,R16
     LDI R17,(1<<INT1)
     out EIMSK,R17
.ENDMACRO

; inicializa o led ligado
.MACRO CFG_LED
     LDI R16,(1<<PB5)
     OUT DDRB,R16
     LDI R18,(1<<PB5)
     OUT PORTB,R18
.ENDMACRO

; alterna o estado do led
.MACRO HANDLER_MAIN
      CPI R18,(1<<PB5)
      BREQ TURN_OFF   ; ta ligado vai pra desligar
      RJMP TURN_OFF    ; ta desligado vai pra ligar
.ENDMACRO

// diz o microcontrolador especifico p/ o compilador
.device ATMega328p
// inicio da memoria de código
.org 0x0000
// pula pro inicio do código
rjmp   INIT
.org 0x0004
rjmp HANDLER



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

INIT:
      CFG_LED
      CFG_STACK
      CFG_PORT
      CFG_HANDLER
      SEI

LOOP: RJMP LOOP

