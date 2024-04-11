#ifndef FILTERS_H
#define FILTERS_H

#include "types.h"
#include <math.h>

#define shb(x, n) ((x) << (8*(n)))

#define alpha(x) (shb((x), 3) & 0xFF)
#define blue(x) (shb((x), 2) & 0xFF)
#define green(x) (shb((x), 1) & 0xFF)
#define red(x) ((x) & 0xFF)

typedef struct {
    f32 min;
    f32 max; 
} Extrema;

u32 bytes_to_rgba(u8 r, u8 g, u8 b, u8 a);
u32 bytes_to_grayscale(u8 val,u8 a);
f32 rgb_to_lum(u32 rgb);

u32 minimum(u32* vals, u32 length);
u32 maximum(u32* vals, u32 length);
u32 argmin(u32* vals, u32 length);
u32 argmax(u32* vals, u32 length);
u32* multiple_argmin(u32* vals, u32 length, u32 out_length);
u32* multiple_argmax(u32* vals, u32 length, u32 out_length);
Extrema extremas(f32* arr, u32 length);

f32* convolute(u32* mask, f32* arr, u32 width, u32 height);
f32* padded_arr(f32* arr, u32 width, u32 length);
u32* sobel_filter(u32* arr, u32 width, u32 height);

#endif // FILTERS_H