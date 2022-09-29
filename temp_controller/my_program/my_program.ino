
volatile bool converted;
volatile bool newValue;
volatile int value;

volatile float p_value = 30;

void setup()
{
  Serial.begin(9600); // abre a porta serial a 9600 bps

  // Select A0
  ADMUX |= (1<<REFS0);
  // Enable ADC; Prescaler = 128; Enable Interrupt;
  ADCSRA |= (1<<ADEN)|(1<<ADSC)|(1<<ADIE)|(1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0); // 0xC7 = 0b11000111 
  // Set trigger on TIMER1 match B
  ADCSRB |= (1<<ADTS2)|(1<<ADTS0);


  // Set PB1 to be an output (Pin9 Arduino UNO)
  DDRB |= (1 << PB1);
  // Clear Timer/Counter Control Registers
  TCCR1A = 0;
  TCCR1B = 0;
  // Set inverting mode
  TCCR1A |= (1 << COM1A0)|(1 << COM1A1);

  // Set phase-correct PWM mode 
  TCCR1A |= (1 << WGM11);
  TCCR1B |= (1 << WGM13);
  // Set prescaler to 1024 and starts PWM
  TCCR1B |= (1 << CS12);
  TCCR1B |= (1 << CS10);
  // Set PWM frequency/top value
  ICR1 = 0xFFFF;
  // Set dutycicle 15625 -> 2 secs
  OCR1A = 15625;
  sei();

}

void loop() {
    if (newValue) {
    newValue = false;
    float temp = 100*float(value)/1024;
    uint16_t control = convert_desired_temp(temp);
    Serial.print("Temp: ");
    Serial.print(" Desire: ");
    Serial.print(temp);
    Serial.print(" Control: ");
    Serial.println(convert_control(control));
    OCR1A = control;
    delay(500);             // C5:: give you time to see the ouput
  }
  
  if (converted) {
    ADCSRA |= (1<<ADSC|1<<ADIE);   // C4:: start converting voltage on A0
    converted = false;
  }
}

ISR(ADC_vect){//timer1 interrupt 1Hz toggles pin 13 (LED)
  value = ADC;
  converted = true;
  newValue = true;
}

uint16_t convert_desired_temp(float delta_temp) {
  if (delta_temp < 5) {
    return 0x00000000;
  }
  else if (delta_temp > p_value) {
    return 0xFFFFFFFF;
  } else {
    return (0xFFFFFFFF-2*15625)*delta_temp/p_value + 15625;
  }
}

float convert_control(uint16_t control) {
    return 2*float(control)/15625;
}



