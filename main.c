#include "knn/knn.h"
#include "dataset/dataset.h"
#include "hardware/hardware_abstraction_layer.h"

typedef enum Label
{
    AMUSING = 0,
    BORING =1,
    RELAXING =2,
    SCARY = 3,
    UNDEFINED=4,

} Label;


int main(void)
{

    dataset set;
    float input[4];
    uint8_t K_NEAREST = 1;
    sample samples[K_NEAREST];

    hadware_setup();

    for (;;)
    {
        /*
    	Apesar dos sensores emitirem diferentes valores,
        nessa simulação são 5 potenciômetros que variam de 0 até 5V.
        Portanto para normalizar é só dividir por 5.0
    */
	input[0] = read_heart_beat_in_millivolts()/5.0;
    input[1] = read_blood_volume_pulse_in_percentage()/5.0;
    input[2] = read_galvanic_skin_response_in_microsecond()/5.0;
    input[3] = read_respiration_sensor_in_percentage()/5.0;
  	
  	// Serial.println("Input");
	// print_signal(input);

    float mean_values[K_NEAREST];
  	
  	find_k_nearest(&set, input, samples, K_NEAREST);
    for (uint8_t i =0; i < K_NEAREST; i++ )
    {
      mean_values[samples[i].label] +=1.0/K_NEAREST;
    }


    // pwm
    write_amusing_led(mean_values[AMUSING]);
    write_boring_led(mean_values[BORING]);
    write_relaxing_led(mean_values[RELAXING]);
    write_scary_led(mean_values[SCARY]);
    write_undefined_led(mean_values[UNDEFINED]);

       
    }

    return 0;
}

