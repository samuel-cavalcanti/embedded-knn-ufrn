#include "knn/knn.h"
#include "dataset/dataset.h"
#include "hardware_abstraction_layer/hardware_abstraction_layer.h"

typedef enum Label
{
    AMUSING = 1,
    BORING,
    RELAXING,
    SCARY,
    UNDEFINED

} Label;

typedef enum Spys
{
    RUSSIANS,
    CHINESE,
    AMERICANS,
    GERMANS

} Spys;

void send_data(float *input, Label user_status, Spys spy_countries);

int main(void)
{

    dataset set;
    float input[9];
    uint8_t k_nearest = 1;
    sample samples[k_nearest];

    hadware_setup();

    for (;;)
    {
        input[0] = read_heart_beat_in_millivolts();
        input[1] = read_blood_volume_pulse_in_percentage();
        input[2] = read_galvanic_skin_response_in_microsecond();
        input[3] = read_respiration_sensor_in_percentage();
        input[4] = read_skin_temperature_in_celsius();
        input[5] = read_muscle_sensor_in_microcoulomb(ZYGOMATICUS);
        input[6] = read_muscle_sensor_in_microcoulomb(SUPERCILII);
        input[7] = read_muscle_sensor_in_microcoulomb(TRAPEZIUS);
        input[8] = read_joytick();
        find_k_nearest(&set, input, samples, k_nearest);

        switch (samples[0].label)
        {
        case AMUSING:
            send_data(input, samples[0].label, RUSSIANS);
            break;

        case BORING:
            send_data(input, samples[0].label, AMERICANS);
            break;

        case RELAXING:
            send_data(input, samples[0].label, CHINESE);
            break;

        case SCARY:
            send_data(input, samples[0].label, GERMANS); //7x1

            break;

        case UNDEFINED:
            break;
        }
    }

    return 0;
}

void send_data(float *input, Label user_status, Spys spy_countries)
{
}
