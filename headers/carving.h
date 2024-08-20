#ifndef CARVING_H
#define CARVING_H

#include "types.h"
#include "filters.h"
#include "bitarray.h"
#include "log.h"
#include <malloc.h>

u32* get_lowest_energy_seam(u8* energy_map, u32 width, u32 height);
u32** get_multiple_lowest_energy_seams(u8* energy_map, u32 width, u32 height, u32 num_seams);
u32* translate_seam(u32** previous_seams, u32* seam, u32 count, u32 height);

void transpose_image(u8* img, u32 width, u32 height);

void mark_seam_red_vertical(u8* img, u32* seam, u32 width, u32 height);
void mark_seam_red_horizontal(u8* img, u32* seam, u32 width);
void mark_multiple_vertical_seams(u8* img, i32 width, i32 height, u32 num_seams);

void remove_seam(u8* img, u32* seam, u32 width, u32 height, i32* new_width);
void remove_multiple_vertical_seams(u8* img, i32 width, i32 height, u32 num_seams, i32* new_width);

#endif // CARVING_H