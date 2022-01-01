#include "chosen.h"



chosen new_chosen(uint8_t size)
{
    chosen chosen_array;

    for (int i = 0; i < MAX_CHOSEN_SIZE; i++)
        chosen_array.array[i] = -1;

    chosen_array.size = size;
    chosen_array.current_index = 0;

    return chosen_array;
}

bool is_in_chosen(chosen *self, uint16_t index)
{

    for (int i = 0; i < self->size; i++)
        if (self->array[i] == index)
            return true;

    return false;
}

void add_in_chosen(chosen *self, uint16_t value)
{
    self->array[self->current_index] = value;

    self->current_index++;
}