/*inclusão das bibliotecas necessárias*/
#include <OneWire.h>  
#include <DallasTemperature.h>

volatile bool converted;
volatile bool newValue;
volatile uint16_t value;
volatile float p_value = 10;
volatile float temp;
volatile uint16_t control;

#define temp_wire 2 

OneWire oneWire(temp_wire);
DallasTemperature sensors(&oneWire);

void setup()
{
  Serial.begin(9600); // abre a porta serial a 9600 bps
  sensors.begin(); /*inicia biblioteca*/

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
  TCCR1A |= (1 << COM1A1);
  //TCCR1A |= (1 << COM1A0);

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
  
  sensors.requestTemperaturesByIndex(0);
  sensors.setResolution(9);
  sei();
}

void convert_desired_temp(float delta_temp) {
    if (delta_temp < 5.0L) {
      TCCR1B &= ~(1<<CS12);
      TCCR1B &= ~(1<<CS10);
      TCCR1A &= ~(1<<COM1A1);
      PORTB &= ~(1<<PB1);
      control = 0x00000000;
    } else if (delta_temp > p_value) {
      TCCR1B &= ~(1<<CS12);
      TCCR1B &= ~(1<<CS10);
      TCCR1A &= ~(1<<COM1A1);
      PORTB |= (1<<PB1);
      control = 0xFFFFFFFF;
    } else {
      TCCR1B |= (1<<CS12)|(1<<CS10);
      TCCR1A |= (1<<COM1A1);
      control = uint16_t((pow(2,16)-2L*15625L - 1L)*delta_temp/p_value + 15625L);
      OCR1A = control;
    }
}

float convert_control(uint16_t control) {
    return 2*float(control)/15625L;
}

void loop() {
    if (newValue) {
    newValue = false;
    float desired_temp = float(value) * 100L / 1023L;
    float delta_temp = desired_temp - temp;
    convert_desired_temp(delta_temp);

    Serial.print(temp);
    Serial.print(" ");
    Serial.print(desired_temp);
    Serial.print(" ");
    Serial.println(convert_control(control));
  }
  
  if (converted) {
    ADCSRA |= (1<<ADSC)|(1<<ADIE);   // C4:: start converting voltage on A0
    converted = false;
  }

  if (sensors.getWaitForConversion()) {
    temp = sensors.getTempCByIndex(0);  
    sensors.requestTemperaturesByIndex(0);
}
  delay(1000);
}

ISR(ADC_vect){
  value = ADC;
  converted = true;
  newValue = true;
}






