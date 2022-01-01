#include "../dataset/dataset.h"
#include "../knn/knn.h"

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>

dataset empty_dataset();

bool test_find_nearest_1d();
bool test_find_nearest_2d();
void test_knn();
int main(int argc, char const *argv[])
{
    test_knn();

    return 0;
}

void test_knn()
{

    if (test_find_nearest_1d() && test_find_nearest_2d())
    {
        printf("test knn passou!\n");
        return;
    }

    printf("knn falhou!\n");
}

/*
 teste baseado no artigo:
 https://towardsdatascience.com/machine-learning-basics-with-the-k-nearest-neighbors-algorithm-6a6e71d01761
*/

bool test_find_nearest_1d()
{

    dataset set = empty_dataset();

    set.samples[0].features[0] = 22;
    set.samples[0].label = 1;

    set.samples[1].features[0] = 23;
    set.samples[1].label = 1;

    set.samples[2].features[0] = 24;
    set.samples[2].label = 1;

    set.samples[3].features[0] = 18;
    set.samples[3].label = 1;

    set.samples[4].features[0] = 19;
    set.samples[4].label = 1;

    set.samples[5].features[0] = 25;
    set.samples[5].label = 0;

    set.samples[6].features[0] = 27;
    set.samples[6].label = 0;

    set.samples[7].features[0] = 29;
    set.samples[7].label = 0;

    set.samples[8].features[0] = 31;
    set.samples[8].label = 0;

    set.samples[9].features[0] = 45;
    set.samples[9].label = 0;
    //                          0   1  2  3  4  5  6  7  8
    float query[SAMPLE_SIZE] = {0, 0, 0, 0, 0, 0, 0, 0, 0};

    uint8_t k_nearest = 2;

    sample samples[k_nearest];

    float input_queries[5] = {29, 45, 31, 18, 19};
    float second_nearest_expected[5] = {27, 31, 29, 19, 18};

    for (int i = 0; i < 5; i++)
    {
        query[0] = input_queries[i];
        find_k_nearest(&set, query, samples, k_nearest);
        if (samples[0].features[0] != input_queries[i] || samples[1].features[0] != second_nearest_expected[i])
        {
            printf("teste  falhou !\n");
            printf("i = %i, input_queries[i] %f output: %f \n", i, input_queries[i]);
            printf("samples[0].features[0]: %f  samples[1].features[0] %f\n", samples[0].features[0], samples[1].features[0]);
            return false;
        }
    }

    return true;
}

dataset empty_dataset()
{
    dataset set;
    uint8_t not_valid_label = 2;

    for (int i = 0; i < DATASET_SIZE; i++)
    {
        for (int j = 0; j < SAMPLE_SIZE; j++)
            set.samples[i].features[j] = 0.0;

        set.samples[i].label = not_valid_label;
    }

    return set;
}

float deg2rad(float degree)
{

    return M_PI / 180 * degree;
}
float rad2deg(float radians)
{
    return 180 / M_PI * radians;
}

bool is_desired_vector_and_have_proximity_angles(sample *samples, float desired_x, float desired_y)
{

    if (samples[0].features[0] != desired_x || samples[0].features[1] != desired_y)
        return false;

    float theta = roundf(rad2deg(atan2(samples[0].features[1], samples[0].features[0])));

    float phi = roundf(rad2deg(atan2(samples[1].features[1], samples[1].features[0])));

    float gamma = roundf(rad2deg(atan2(samples[2].features[1], samples[2].features[0])));

    int diference_phi = abs((int)theta - (int)phi);

    int difefence_gamma = abs((int)theta - (int)gamma);

    if (diference_phi != 10 || difefence_gamma != 10)
        return false;

    return true;
}

bool test_find_nearest_2d()
{
    dataset set = empty_dataset();

    for (int i = 0; i < 9; i++)
    {
        int angle_in_degree = (i + 1) * 10;

        set.samples[i].features[0] = cos(deg2rad(angle_in_degree));
        set.samples[i].features[1] = sin(deg2rad(angle_in_degree));
    }

   
    float input_query_x[3] = {cos(deg2rad(20)), cos(deg2rad(50)), cos(deg2rad(80))};
    float input_query_y[3] = {sin(deg2rad(20)), sin(deg2rad(50)), sin(deg2rad(80))};

    uint8_t k_nearest = 3;
    sample samples[k_nearest];
    //                          0   1  2  3  4  5  6  7  8
    float query[SAMPLE_SIZE] = {0, 0, 0, 0, 0, 0, 0, 0, 0};

    for (int i = 0; i < 3; i++)
    {
        query[0] = input_query_x[i];
        query[1] = input_query_y[i];
        find_k_nearest(&set, query, samples, k_nearest);
        if (is_desired_vector_and_have_proximity_angles(samples, input_query_x[i], input_query_y[i]) == false)
        {
            float output_angle = rad2deg(atan(samples[0].features[1] / samples[0].features[0]));
            float intput_angle = rad2deg(atan(query[1] / query[0]));
            printf("teste falhou !\n");
            printf("i = %i, input angle %f output angle  %f \n", i, intput_angle, output_angle);

            return false;
        }
    }

    return true;
}
