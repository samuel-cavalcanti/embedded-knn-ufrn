//#define __AVR_ATmega328P__      // para visualizar melhor na IDE
#if defined(__AVR_ATmega328P__) // microcontrolador do arduino Uno

#include "hardware_abstraction_layer.h"

#include <avr/io.h>
#include "stdint.h"
#include <stdbool.h>

float read_adc(uint8_t admux);

void wait_to_analogic_digital_conversion_finish();

void hadware_setup()
{

    /*
        Habilitando todas as portas C do atmega 328p que equivale as portas
        A0,A1,A2,A3,A4,A5 do arduino uno respectivamente.
        As portas A0,A1,A2 serão as entradas dos sensores de temperatura e sensor de umidade.
        As portas A0, A1,A2  também se equivale as portas ADC[0], ADC[1] e ADC[2], respectivamente. 
    */
    DDRC = 0b00000000;

    /*
      ADCSRA -- significa  ADC Control and Status Register A

      no bit 7 habilita ou não o ADC
      no bit 6 se 1 então começa a conversão se estiver em modo simples
      no bit 5 se 1 então habilita o auto gatilho 
      no bit 4 representa a flag de interrupção, quando a interrupção ocorrer esse bit vai para 1 
      no bit 3 habilita a interrupção

      e os 3 bits menos significativos sever para escolher o Prescaler do ADC
      na prática utilizando o atmega a 16MHz a única opção é o 111

      7 - estou habilitando o ADC
      6 - estou informação que não quero começar a conversão
      5 - não quero habilitar a conversão automática ou seja eu que vou sinalizar quando começar a conversão
      4 - não controlo  
      3 - não quero interromper nenhuma conversão a final nem comecei uma.   
      2:0 - sou obrigado a usar o o Prescaler 111 por está trabalhando a 16MHz 
    */
    //         76543210
    ADCSRA = 0b10000111;

    /* 
        ADCSRB -- significa ADC Control and Status Register B
        bit 7 - nenhuma informação encontrada, pode existir só não encontrei / não foi necessário
        bit 6 - nenhuma informação encontrada, pode existir só não encontrei / não foi necessário
        bit 5 - nenhuma informação encontrada, pode existir só não encontrei / não foi necessário
        bit 4 - nenhuma informação encontrada, pode existir só não encontrei / não foi necessário
        bit 3 - nenhuma informação encontrada, pode existir só não encontrei / não foi necessário


        os 3 últimos bits [2:0]  sevem para definir a fonte do auto gatilho

        000 -- significa que a conversão será contínua, onde ao final de uma conversão outra começa imediatamente
        001 -- significa que a conversão se dará a partir de um comparador analógico, ou seja o 6 bit do ADCSRA == 1
        010 -- significa que a conversão se dará a partir de uma interrupção externa
        011 -- significa que a conversão se dará a partir da comparação de A do TC0
        100 -- significa que a conversão se dará a partir do estouro de contagem do TC0
        101 -- significa que a conversão se dará a partir da comparação de B do TC1
        110 -- significa que a conversão se dará a partir do estouro de contagem do TC1
        111 -- evento de captura do TC1


        nesse caso habilitei  a fonte do gatilho a partir da comparação do comparador analógico
        dessa forma eu informo quando quero fazer a conversão para digital   
    */

    //         76543210
    ADCSRB = 0b00000001;

    /*
        PORTD 

        são as portas digitais do arduino de  D0 até D7
        quero os pinos do PWM logo, os Pinos 5 e 6
        ou seja 
        os bits 5 e 6 deve ser marcados como saída
    */
    //       76543210
    DDRD = 0b01101000;

    /*
        PORTB

        Utilizado portas D9,D10,D11 como saída pwm para os leds
        PINB pinos de entrada na da porta PORTB
        D8, D9 e D10, D11 representa os bits 0,1,2,3 do registrador DDRB
        Quero so registrados D9,D10 como saída.


     */
    //       76543210
    DDRB = 0b00001110;

    //         76543210
    TCCR0A = 0b10100011;
    TCCR1A = 0b10100011;
    TCCR2A = 0b10100011;

    //         76543210
    TCCR0B = 0b00000100;
    TCCR1B = 0b00000100;
    TCCR2B = 0b00000100;
}

float read_heart_beat_in_millivolts()
{
    /*
        ADMUX -- ADC Multiplexer Selection Register
        
        Segundo a referência os bits  6 e 7 representam o tipo de tensão de referência
        no caso 00, significa que a tensão de referência será proveniente de uma tensão
        externa, que será ligada ao pino AREF
        
        no caso 11, significa que ele usará a tensão interna de 1.1V como referência
        no caso 01, significa que ele usará a tensão do Vcc (5V) como referência 

        o bit da posição 5 representa se o resultado do da conversão estará ajustado a direita ou a esquerda
        isso porque o resultado da conversão são 10bis e irem precisar de 2 registradores para armazena-lo
        ajustando a esquerda, podemos ler o resultado do primeiro registrador e ignorar os 2 bis menos significativos
        tendo em visa que na leitura pode haver ruído,então ignorar os 2 bits pode ser bastante interessante.
        
        Os 4 bits menos significativos ou seja os bits da posição 0,1,2,3
        representam as entradas do conversor. No arduino uno elas represetam
        as entradas do sinal
        0000 - ADC[0]
        0001 - ADC[1]
        0010 - ADC[2]
        0011 - ADC[3]
        0100 - ADC[4]
        0101 - ADC[5]
        0110 - ADC[6]
        0111 - ADC[7]

        no caso a entrada escolhida foi a ADC[0]
        e a tensão de referência externa de  é 5V, uma vez que
        podemos representar a tensão da umidade de 0 2.5V e a tensão de 0 até 5V
        dos sensores de temperatura.
        Ou seja os valores de umidade varia de 0 até 512 (1024/2) 
        e os valores de temperatura variam de 0 até 1024
 

        traduzindo a minha escolha para os bits fica
        76
        01 - Vcc como referência 

        3210
        0000 -  ADC[0]

        5 
        0 - lendo os resistadores da direita para esquerda

    */

    float v_in = read_adc(0b01000000);

    return v_in; // (v_in - 2.8) / 50 * 1000;
}

float read_blood_volume_pulse_in_percentage()
{

    //Vcc como referencia(01) e ADC1 como entrada

    float v_in = read_adc(0b01000001);

    return v_in; // 58.962 * v_in - 115.09;
}

float read_galvanic_skin_response_in_microsecond()
{
    //Vcc como referencia(01) e ADC2 como entrada
    float v_in = read_adc(0b01000010);

    return v_in; // 24.0 * v_in - 49.2;
}

float read_respiration_sensor_in_percentage()
{
    //Vcc como referencia(01) e ADC3 como entrada

    float v_in = read_adc(0b01000011);

    return v_in; //58.923 * v_in - 115.01;
}

float read_adc(uint8_t admux)
{
    ADMUX = admux;

    wait_to_analogic_digital_conversion_finish();

    /* 
    Sabendo que a tensão de referência é 5V 
    e o valor máximo inteiro da leitura analógica é 1023
    então é convertido a leitura para voltagem 
     */

    return ((float)ADC) * (5.0 / 1023.0);
}

void wait_to_analogic_digital_conversion_finish()
{
    // habilitando o bit 6 do ADCSRA inicializo o processo de conversão
    //          76543210
    ADCSRA |= 0b01000000;

    /*
        no bit 4 representa a flag de interrupção, quando a interrupção ocorrer esse bit vai para 1 
        ou seja quando derminar o processo de de conversão então sai do while
    */

    //                  76543210
    while (!(ADCSRA & 0b00010000))
    {
    }
}

/*

Timer output	Arduino output	Chip pin	Pin name
    OC2B	           3           5	      PD3
    OC0B	           5	       11	      PD5
    OC0A	           6	       12	      PD6
    OC1A	           9	       15	      PB1
    OC1B	           10	       16	      PB2
    OC2A	           11          17      	  PB3

*/

void write_amusing_led(float duty_cycle)
{

    /*
    OC2B - OCR2B representa a saída do pino  D3 do arduino uno
    */

    OCR2B = (uint8_t)(duty_cycle * 255.0);
}

void write_boring_led(float duty_cycle)
{
    /*
    OC0B - OCR0B representa a saída D5 do arduino uno
    */
    OCR0B = (uint8_t)(duty_cycle * 255.0);
}

void write_relaxing_led(float duty_cycle)
{
    /*
    OC0A - OCR0A representa a saída do pino D6 do arduino uno
    */
    OCR0A = (uint8_t)(duty_cycle * 255.0);
}

void write_scary_led(float duty_cycle)
{

    /*
      OC2A - OCR2A representa a saída do pino D11
      do arduino uno 
    */
    OCR2A = (uint8_t)(duty_cycle * 255.0);
}

#endif // arduino Uno