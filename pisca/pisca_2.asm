.device ATMega328p
.org 0x00

rjmp INIT

INIT:

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


