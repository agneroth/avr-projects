
int ADC_O_1;                         // ADC Output First 8 bits 
int ADC_O_2;                         // ADC Output Next 2 bits   
#define 2_SEC = 15625;

void setup() {
  Serial.begin(9600); // abre a porta serial a 9600 bps:
  ADMUX |= (1<<REFS0);
  ADCSRA |= (1<<ADEN)|(1<<ADSC)|(1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0); // 0xC7 = 0b11000111 = 
  //(1<<ADEN)|(1<<ADSC)|(1<<ADATE)|(1<<ADPS2)|(1<<ADPS0);
  ADCSRB = 0x00;
  

}

void loop() {
  // put your main code here, to run repeatedly:
  if ((ADCSRA & 0x40) == 0) {
   //ADC_O_1 = ADCL;
   //ADC_O_2 = ADCH;
   Serial.println(100*float(ADC)/1024);
   ADCSRA |= 0x40;
   delay(1000);
  }
}
