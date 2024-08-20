#ifndef BIT_ARRAY_H
#define BIT_ARRAY_H

#include "types.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    u8 *array;
    size_t size;
} BitArray;


BitArray* create_bitarray(size_t size);
void set_bit(BitArray *array, u32 bit_num);
void clear_bit(BitArray *array, u32 bit_num);
void flip_bit(BitArray *array, u32 bit_num);
u8 get_bit(BitArray *array, u32 bit_num);
void free_bitarray(BitArray *array);

#endif // BIT_ARRAY_H