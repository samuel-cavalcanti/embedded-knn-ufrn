#ifndef CHOSEN_H

#define CHOSEN_H


#include <stdint.h>
#include <stdbool.h>
#define MAX_CHOSEN_SIZE 255

typedef struct chosen
{
    int16_t array[MAX_CHOSEN_SIZE];
    uint8_t current_index;
    uint8_t size;
} chosen;

chosen new_chosen(uint8_t size);

bool is_in_chosen(chosen* self, uint16_t index);
void add_in_chosen(chosen* self,uint16_t index);


#endif // CHOSEN_H