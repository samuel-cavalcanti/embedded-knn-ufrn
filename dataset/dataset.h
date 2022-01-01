#ifndef DATASET_H
#define DATASET_H

#define DATASET_SIZE 100
#define SAMPLE_SIZE 9

#include <stdint.h>

typedef struct sample
{
    float features[SAMPLE_SIZE];
    uint8_t label;
} sample;

typedef struct dataset
{

    sample samples[DATASET_SIZE];

} dataset;

#endif