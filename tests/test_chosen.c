#include <stdio.h>
#include <stdbool.h>
#include "../knn/chosen/chosen.h"

void test_chosen_array()
{

    int size = 5;

    chosen array = new_chosen(size);
    add_in_chosen(&array, 0);
    add_in_chosen(&array, 1);
    add_in_chosen(&array, 2);
    add_in_chosen(&array, 3);
    add_in_chosen(&array, 4);

    for (int i = 0; i < size; i++)
    {
        add_in_chosen(&array, i);
    }

    bool expected[7] = {
        false,
        false,

        true,
        true,
        true,

        true,
        true,
    };

    uint16_t test_value[7] = {6, 5, 4, 3, 2, 1, 0};

    for (int i = 0; i < 7; i++)
        if (is_in_chosen(&array, test_value[i]) != expected[i])
        {
            printf("teste falhou !! com o index: %i valor: %i \n", i, test_value[i]);
            return;
        }

    printf("teste chosen passou!\n");
}

int main(int argc, char const *argv[])
{
    test_chosen_array();
    return 0;
}
