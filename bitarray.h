#ifndef BIT_ARRAY_H
#define BIT_ARRAY_H

#include "types.h"
#include <malloc.h>
#include <stdio.h>
#include <stdlib.h>

void set_bit(u8 *array, u32 bit_num);
void clear_bit(u8 *array, u32 bit_num);
u8 get_bit(u8 *array, u32 bit_num);
u8* get_bitarray(u32 size);
#endif // BIT_ARRAY_H