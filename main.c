#include "knn/knn.h"
#include "dataset/dataset.h"
#include "hardware/hardware_abstraction_layer.h"

typedef enum Label
{
    AMUSING = 0,
    BORING = 1,
    RELAXING = 2,
    SCARY = 3,

} Label;

int main(void)
{

    dataset set;
    float input[4];
    uint8_t K_NEAREST = 5;
    sample samples[K_NEAREST];

    hadware_setup();

    for (;;)
    {
        /*
    	Apesar dos sensores emitirem diferentes valores,
        nessa simulação são 5 potenciômetros que variam de 0 até 5V.
        Portanto para normalizar é só dividir por 5.0
    */
        input[0] = read_heart_beat_in_millivolts() / 5.0;
        input[1] = read_blood_volume_pulse_in_percentage() / 5.0;
        input[2] = read_galvanic_skin_response_in_microsecond() / 5.0;
        input[3] = read_respiration_sensor_in_percentage() / 5.0;


        find_k_nearest(&set, input, samples, K_NEAREST);

        uint8_t label_frequencies[4] = {0, 0, 0, 0};

        for (uint8_t i = 0; i < K_NEAREST; i++)
        {
            label_frequencies[(uint8_t)samples[i].label]++;
        }

        // pwm
        write_scary_led((float)label_frequencies[SCARY] / K_NEAREST);
        write_amusing_led((float)label_frequencies[AMUSING] / K_NEAREST);
        write_boring_led((float)label_frequencies[BORING] / K_NEAREST);
        write_relaxing_led((float)label_frequencies[RELAXING] / K_NEAREST);
    }

    return 0;
}
