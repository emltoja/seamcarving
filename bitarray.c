#include "bitarray.h"

// Path: bitarray.h

u8* get_bitarray(u32 size) {
    u8* arr = (u8 *)malloc(size + sizeof(char));
    if (arr == NULL) {
        printf("Failed to allocate memory for bitarray\n");
        exit(1);
    }
    return arr;
}

void set_bit(u8* array, u32 index) {
    array[index/sizeof(char)] |= 1 << (index % sizeof(char));
}

void clear_bit(u8* array, u32 index) {
    array[index/sizeof(char)] &= ~(1 << (index % sizeof(char)));
}

char get_bit(u8* array, u32 index) {
    return (array[index/sizeof(char)] >> (index % sizeof(char))) & 1;
}