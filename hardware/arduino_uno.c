#define __AVR_ATmega328P__      // para visualizar melhor na IDE
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
    TCCRnA -- Timer /Counter Control n Register A

    bit  7 -  COM0A1
    bit  6 -  COM0A0
    bit  5 -  COM0B1
    bit  4 -  COM0B0
    bit  3 -  ...
    bit  2 -  ...
    bit  1 -  WGM01
    bit  0 -  WGM00

    se COM0A1,COM0A0 = 00 então é a operação normal do pino, OC0A desconectado.
    se COM0A1,COM0A0 = 01 operação normal do pino, OC0A desconectado.
    se COM0A1,COM0A0 = 10 OC0A é limpo na igualdade de comparação. (modo não invertido) 
    se COM0A1,COM0A0 = 11 OC0A é ativo na igualdade de comparação e limpo no valor de TC mínimo. (modo invertido) 

    TCCRnB -- Timer /Counter Control n Register B

    bit  7 -  FOC0A
    bit  6 -  FOC0B
    bit  5 -  ...
    bit  4 -  ...
    bit  3 -  WGM02
    bit  2 -  CS02
    bit  1 -  CS01
    bit  0 -  CS00

    FOC0A e FOC0B -  Force Output Compare A e B, quando modo não-PWM, força uma comparação no modulo gerador de onda

    CS02,CS01,CS00 seleção do clock. Onde:
    
    CS02,CS01,CS00 = 000 , então sem Fonte de Clock (TC0 parado)
    CS02,CS01,CS00 = 001 , prescaler = 1
    CS02,CS01,CS00 = 010 , prescaler = 8 
    CS02,CS01,CS00 = 011 , prescaler = 64 
    CS02,CS01,CS00 = 100 , prescaler = 256  
    CS02,CS01,CS00 = 101 , prescaler = 1024

    CS02,CS01,CS00 = 110, Clock externo no pino T0. Contagem na borda de descida
    CS02,CS01,CS00 = 111, Clock externo no pino T0. Contagem na borda de subida


    então para configurar o PWD se utiliza os seguintes registradores:  WGM02,WGM01,WGM00
    
    WGM02,WGM01,WGM00 = 000, modo de operação TC normal
    WGM02,WGM01,WGM00 = 001, modo de operação TC PWM fase corrigida
    WGM02,WGM01,WGM00 = 010, modo de operação TC CTC
    WGM02,WGM01,WGM00 = 011, modo de operação TC PWM rápido
    

    A equação da frequencia do PWM rápido é:
     
    f_pwm = 16MHz/(prescaler*256), usando o prescaler = 256, temos que:
    f_pwm = 234.9624060150376 Hz,um valor acima de 100Hz e abaixo de 100Khz
    
    Também vamos usar a configuração não invertida, dessa forma a tensão rms
    cresce com o valor do contatador

    vamos usar o Fast PWM logo, o 3-bit do TCCRnB (WGM02) deve ser 0
    e 1:0-bit do TCCRnB (WGM01,WGM00) = 11
    
    */

    //         76543210
    TCCR0A = 0b10100011;
    //         76543210
    TCCR0B = 0b00000100;

    

    /*
        PORTD 

        são as portas digitais do arduino de  D0 até D7
        quero os pinos do PWM logo, os Pinos 5 e 6
        ou seja 
        os bits 5 e 6 deve ser marcados como saída
    */
    //       76543210
    DDRD = 0b01100000;


    /*
        PORTB

        Utilizado portas D9,D10,D11 como saída pwm para os leds
        PINB pinos de entrada na da porta PORTB
        D8, D9 e D10, D11 representa os bits 0,1,2,3 do registrador DDRB


     */
    //       76543210
    DDRB = 0b00001110;   

}

float read_heart_beat_in_millivolts()
{
    /*
        ADMUX -- ADC Multiplexer Selection Register
        
        Segundo a referência os bits  6 e 7 representam o tipo de tensão de referência
        no caso 00, significa que a tensão de referência será proveniente de uma tensão
        externa, que será ligada ao pino AREF
        
        no caso 11, significa que ele usará a tensão interna de 1.1V como referência

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
        00 - referência externa

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

void write_amusing_led(float duty_cycle)
{

}

void write_boring_led(float duty_cycle)
{
}

void write_relaxing_led(float duty_cycle)
{
}

void write_scary_led(float duty_cycle)
{
}

void write_undefined_led(float duty_cycle)
{
}

#endif // arduino Uno