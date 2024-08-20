#include "bitarray.h"

// Path: bitarray.h

// Check wether index is out of bounds
void check_bounds(size_t size, u32 index) {
    if (index >= size) {
        fprintf(stderr, "Bit number %d out of bounds\n", index);
        exit(1);
    }
}

// Create a new BitArray with the given size
BitArray* create_bitarray(size_t num_bits) {
    BitArray* bitarray = (BitArray*)malloc(sizeof(BitArray));
    if (!bitarray) {
        perror("Failed to allocate memory for BitArray");
        return NULL;
    }

    size_t num_bytes = (num_bits + 7) / 8;
    bitarray->array = (u8*)malloc(num_bytes);
    if (!bitarray->array) {
        perror("Failed to allocate memory for BitArray array");
        free(bitarray);
        return NULL;
    }

    memset(bitarray->array, 0, num_bytes);
    bitarray->size = num_bits;

    return bitarray;
}


// Set bit to 1 
void set_bit(BitArray* bitarray, u32 bit_num) {
    check_bounds(bitarray->size, bit_num);
    bitarray->array[bit_num / 8] |= (1 << (bit_num % 8));
}

// Set bit to 0 (clear bit)
void clear_bit(BitArray* bitarray, u32 bit_num) {
    check_bounds(bitarray->size, bit_num);
    bitarray->array[bit_num / 8] &= ~(1 << (bit_num % 8));
}

// Flip bit
void flip_bit(BitArray* bitarray, u32 bit_num) {
    check_bounds(bitarray->size, bit_num);
    bitarray->array[bit_num / 8] ^= (1 << (bit_num % 8));
}

// Get bit value
u8 get_bit(BitArray* bitarray, u32 bit_num) {
    check_bounds(bitarray->size, bit_num);
    return (bitarray->array[bit_num / 8] >> (bit_num % 8)) & 1;
}

// Free BitArray
void free_bitarray(BitArray* bitarray) {
    if (!bitarray) {
        return;
    }
    free(bitarray->array);
    free(bitarray);
}
