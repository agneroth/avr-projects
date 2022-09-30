/********************************************************************
 * Projeto de medição de temperatura com DS18B20
 * Desenvolvido pela Curto Circuito com base no exemplo Simple
 */
/********************************************************************/

/*inclusão das bibliotecas necessárias*/
#include <OneWire.h>  
#include <DallasTemperature.h>
/********************************************************************/

#define dados 2 /*o pino de dados do sensor está ligado na porta 2 do Arduino*/

OneWire oneWire(dados);  /*Protocolo OneWire*/
/********************************************************************/
DallasTemperature sensors(&oneWire); /*encaminha referências OneWire para o sensor*/

volatile uint8_t id = 0;
/********************************************************************/ 
void setup() /*laço de configuração*/
{ 
 Serial.begin(9600); /*definição de Baudrate de 9600*/
 sensors.begin(); /*inicia biblioteca*/
 sensors.getAddress(id,id);
 //sensors.getTempC()
} 
void loop(){ 
 sensors.requestTemperatures();
 Serial.println(id); /* Endereço do sensor */
}