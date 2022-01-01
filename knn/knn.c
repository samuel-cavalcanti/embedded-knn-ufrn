#include "knn.h"
#include <math.h>
#include <stdbool.h>
#include "chosen/chosen.h"

typedef struct knn
{
    dataset *set;
    chosen chosen_array;
    sample *best_sample;
    uint16_t current_index;

} knn;

sample find_nearest(knn *self, float *input_signal);
bool is_best_sample(knn *self, uint16_t current_sample_index, float *input_signal);
float euclidean_distance(float *signal_a, float *signal_b);

void find_k_nearest(dataset *set, float *input_signal, sample *nearest_samples, uint8_t k_nearest)
{
    knn model;

    model.set = set;
    model.chosen_array = new_chosen(k_nearest);

    for (uint8_t i = 0; i < k_nearest; i++)
        nearest_samples[i] = find_nearest(&model, input_signal);
}

sample find_nearest(knn *self, float *input_signal)
{
    uint16_t best_index = 0;
    self->best_sample = &self->set->samples[best_index];

    for (uint16_t sample_index = 0; sample_index < DATASET_SIZE; sample_index++)
        if (is_best_sample(self, sample_index, input_signal))
        {
            self->best_sample = &self->set->samples[sample_index];
            best_index = sample_index;
        }

    add_in_chosen(&self->chosen_array, best_index);

    return *self->best_sample;
}

bool is_best_sample(knn *self, uint16_t current_sample_index, float *input_signal)
{
    if (is_in_chosen(&self->chosen_array, current_sample_index))
        return false;

    float dataset_distance_to_signal = euclidean_distance(self->set->samples[current_sample_index].features, input_signal);
    float current_best_signal_distance = euclidean_distance(self->best_sample->features, input_signal);

    if (dataset_distance_to_signal < current_best_signal_distance)
        return true;

    return false;
}

float euclidean_distance(float *signal_a, float *signal_b)
{

    float distance = 0;

    for (int i = 0; i < SAMPLE_SIZE; i++)
        distance += sqrt(pow(signal_a[i] - signal_b[i], 2));

    return distance;
}
