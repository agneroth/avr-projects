.MACRO CFG_STACK
     LDI R16,HIGH(RAMEND) // R16 = RAMEND_HIGH
     OUT SPH,R16 // SPH = R16
     LDI R16,LOW(RAMEND) // ...
     OUT SPL,R16
.ENDMACRO

.MACRO CFG_PORT
     LDI R16,(1<<PD2)
     OUT DDRD,R16
     LDI R17,(1<<PD2)
     OUT PORTD,R17
.ENDMACRO

.MACRO CFG_HANDLER
LDI R16,0x00
;     LDI R16,(1<<ISC01)
     sts EICRA,R16
     LDI R17,(1<<INT0)
     out EIMSK,R17
.ENDMACRO

.MACRO CFG_LED
     LDI R16,(1<<PB5)
     OUT DDRB,R16
     LDI R18,(1<<PB5)
     OUT PORTB,R18
.ENDMACRO

.MACRO HANDLER_MAIN
      CPI R18,0x20
      BREQ TURN_OFF   ; ta ligado vai pra desligar
      RJMP TURN_ON    ; ta desligado vai pra ligar
.ENDMACRO

// diz o microcontrolador especifico p/ o compilador
.device ATMega328p
// inicio da memoria de código
.org 0x0000
// pula pro inicio do código
rjmp   INIT
.org 0x0002
rjmp HANDLER

ALTERNA_INIT:

     LDI R16,0x20
     LDI R17,0x20
     OUT DDRB,R16
     rjmp ALTERNA

SUBTRAI:
        DEC R18            ;subração
        CPI R18,0x00       ; comparar
        BREQ ALTERNA       ; se ta zero vai pro alterna
        LDI R19,0xFF       ;reinicializa o registrador r19
        RJMP SUBTRAI_2     ;se nao ta zerado vai pro subtrai 2

SUBTRAI_2:
         DEC R19
         CPI R19,0x00
         BREQ SUBTRAI
         LDI R20,0xFF
         RJMP SUBTRAI_3


SUBTRAI_3:
         DEC R20
         CPI R20,0x00
         BREQ SUBTRAI_2
         RJMP SUBTRAI_3

TURN_ON:
     LDI R17, 0x20        ; liga led
     OUT PORTB,R17
     rjmp SUBTRAI


TURN_OFF:                  ; desliga led
     LDI R17, 0x00
     OUT PORTB,R17
     rjmp SUBTRAI


ALTERNA:
        LDI R18,0xFF    ;inicializando o registrador
        LDI R19,0xFF    ;inicializando o registrador
        LDI R20,0xFF    ;inicializando o registrador
        CPI R17,0x20
        BREQ TURN_OFF   ; ta ligado vai pra desligar
        RJMP TURN_ON    ; ta desligado vai pra ligar

HANDLER: RJMP HANDLER_END


HANDLER_END:
     RETI


INIT:

    CFG_STACK
      CFG_PORT
  CFG_LED
 CFG_HANDLER
      SEI
      RJMP LOOP

LOOP: RJMP LOOP








