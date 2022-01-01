#ifndef KNN_H
#define KNN_H

#include "../dataset/dataset.h"
#include <stdint.h>

void find_k_nearest(dataset *set, float *input_signal, sample *nearest_samples, uint8_t k_nearest);

#endif // KNN_H