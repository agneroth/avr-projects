.MACRO CFG_STACK
LDI R16,HIGH(RAMEND) // R16 = RAMEND_HIGH
OUT SPH,R16 // SPH = R16
LDI R16,LOW(RAMEND) // ...
OUT SPL,R16
.ENDMACRO

; Configura porta 3 para leitura
.MACRO CFG_PORT
LDI R16,0x08
OUT DDRD,R16
.ENDMACRO

; Configura porta 3 para external interrupt (qualquer sinal logico)
.MACRO CFG_HANDLER
LDI R16,(1<<ISC10)
sts EICRA,R16
LDI R17,(1<<INT1)
out EIMSK,R17
.ENDMACRO

; Configura e liga a led
.MACRO CFG_LED
LDI R16,0x20
OUT DDRB,R16
LDI R18,0x20
OUT PORTB,R18
.ENDMACRO


.MACRO HANDLER_MAIN
 CPI R18,0x20
 BREQ TURN_OFF ; ta ligado vai pra desligar
 RJMP TURN_ON ; ta desligado vai pra ligar
.ENDMACRO


// diz o microcontrolador especifico p/ o compilador
.device ATMega328p
// inicio da memoria de código
.org 0x0000
// pula pro inicio do código
rjmp INIT
.org 0x0004
rjmp HANDLER


INIT:
CFG_STACK
CFG_PORT
CFG_HANDLER
CFG_LED
SEI

LOOP: RJMP LOOP

HANDLER:
RJMP TURN_OFF

HANDLER_END:
RETI

TURN_ON:
        LDI R18, 0x20 ; liga led
        OUT PORTB,R18
        rjmp HANDLER_END

TURN_OFF: ; desliga led
          LDI R18, 0x00
          OUT PORTB,R18
          rjmp HANDLER_END
