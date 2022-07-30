.MACRO CFG_STACK
     LDI R16,HIGH(RAMEND) // R16 = RAMEND_HIGH
     OUT SPH,R16 // SPH = R16
     LDI R16,LOW(RAMEND) // ...
     OUT SPL,R16
.ENDMACRO


.MACRO CFG_PORT
       ; Configura porta D como leitura
     LDI R16,0xFF
     OUT DDRD,R16
       ; Configura porta digital 3 como pullUp
     LDI R17,(1<<PD3)
     OUT PORTD,R17
        ; Configura LED como write, restante como read.
     LDI R16,(1<<PB5)
     OUT DDRB,R16
         ; Configura portal digital 8 e LED como pullUp e
     LDI R17,(1<<PB5)
     OUT PORTB,R17
.ENDMACRO

.MACRO CFG_HANDLER
       ; Configura INT1/Pino digital 3 como interrupt
     LDI R16,(1<<ISC10)
     sts EICRA,R16
     sbi EIMSK,INT1
       ; Configura PCINT0 como interrupt
     LDS R16,PCICR
     ORI R16,(1<<PCIE0)
     STS PCICR,R16
     ; Configura pino digital 8 como interrupt
     LDS R17,PCMSK0
     ORI R17,(1<<PCINT0)
     STS PCMSK0,R17
.ENDMACRO

// diz o microcontrolador especifico p/ o compilador
.device ATMega328p
// inicio da memoria de código
.org 0x0000
// pula pro inicio do código
rjmp   INIT
// INT0
.org 0x0004
rjmp TURN_ON
// PCINT0
.org 0x0006
rjmp TURN_OFF


TURN_OFF:                  ; desliga led
     LDI R18, 0x00
     OUT PORTB,R18
     RETI

TURN_ON:
     LDI R18,(1<<PB5)        ; liga led
     OUT PORTB,R18
     RETI


INIT:
      CFG_STACK
      CFG_PORT
      CFG_HANDLER
      SEI

LOOP: RJMP LOOP

