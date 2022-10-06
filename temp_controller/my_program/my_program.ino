/*inclusão das bibliotecas necessárias*/
#include <OneWire.h>  
#include <DallasTemperature.h>

volatile bool converted;
volatile bool newValue;
volatile uint16_t value;
volatile float p_value = 15;
volatile float temp;
volatile uint16_t control;
volatile int currentTime;
volatile bool blocked = false;
volatile bool recently_blocked = false;



#define temp_wire 2 

OneWire oneWire(temp_wire);
DallasTemperature sensors(&oneWire);

void setup()
{
  Serial.begin(9600); // abre a porta serial a 9600 bps
  sensors.begin(); /*inicia biblioteca*/

  // Select A0
  ADMUX |= (1<<REFS0);
  // Enable ADC; Enable Interrupt;
  ADCSRA |= (1<<ADEN)|(1<<ADSC)|(1<<ADIE);
  // Set trigger on TIMER1 match B
  ADCSRB |= (1<<ADTS2)|(1<<ADTS0);


  // Set PB1 to be an output (Pin9 Arduino UNO)
  DDRB |= (1 << PB1);
  // Clear Timer/Counter Control Registers
  TCCR1A = 0;
  TCCR1B = 0;
  // Set inverting mode
  TCCR1A |= (1 << COM1A1);
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

void disable_pwm() {
    TCCR1A &= ~(1<<COM1A1);
}

void enable_pwm() {
    TCCR1A |= (1<<COM1A1);
}

void pwm_pin_power_off() {
    PORTB &= ~(1<<PB1);
}

void pwm_pin_power_on() {
    PORTB |= (1<<PB1);
}

void enable_interrupt_match_a() {
      TIMSK1 |= (1<<OCIE1A);
    
}

void convert_desired_temp(float delta_temp) {
    if (delta_temp < 1.0L) {
      disable_pwm();
      pwm_pin_power_off();
      control = 0x0000;
      OCR1A = 15625;
      blocked = true;
      TCNT1 = 0;
      enable_interrupt_match_a();
    } else if (delta_temp > p_value) {
      disable_pwm();
      pwm_pin_power_on();
      control = 0xFFFF;
      blocked = true;
      OCR1A = 15625;
      TCNT1 = 0;
      enable_interrupt_match_a();
    } else {
      enable_pwm();
      control = uint16_t((pow(2,16)-2L*15625L -1)*delta_temp/(p_value - 1.0L) + 15625L);
      OCR1A = control;
    }
}

float convert_control(uint16_t control) {
    return 2*float(control - 1)/15625L;
}

void loop() {
  if (recently_blocked) {
    blocked = false;
    recently_blocked = false;
    TIMSK1 &= ~(1<<OCIE1A);
  }

  if (newValue & !blocked) {
    newValue = false;
//    float desired_temp = float(value) * 100L / 1023L;
    float desired_temp = 85.0;
    float delta_temp = desired_temp - temp;
    convert_desired_temp(delta_temp);
    Serial.print(temp);
    Serial.print(" ");
    Serial.print(desired_temp);
    Serial.print(" ");
    Serial.println(convert_control(control));
  }

  if (sensors.getWaitForConversion()) {
    temp = sensors.getTempCByIndex(0);  
    sensors.requestTemperaturesByIndex(0);
  }
  
  if (converted) {
    ADCSRA |= (1<<ADSC)|(1<<ADIE);   // C4:: start converting voltage on A0
    converted = false;
  }
}

ISR(ADC_vect){
  value = ADC;
  converted = true;
  newValue = true;
}

ISR(TIMER1_COMPA_vect) {  
  recently_blocked = true;
}



