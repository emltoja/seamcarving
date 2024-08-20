#include "filters.h"

u32 bytes_to_rgba(u8 r, u8 g, u8 b, u8 a) {
    return shb(a, 3) | shb(b, 2) | shb(g, 1) | r;
}

u8 bytes_to_grayscale(u8 val, u8 a) {
    return shb(a, 3) | shb(val, 2) | shb(val, 1) | val;
}


f32 rgb_to_lum(u32 rgb) {

    f32 norm_r = red(rgb)/255.0;
    f32 norm_g = green(rgb)/255.0;
    f32 norm_b = blue(rgb)/255.0;

    return 0.2126*norm_r + 0.7152*norm_g + 0.0722*norm_b;
}


u32 minimum(u32* vals, u32 length) {

    u32 min = vals[0];

    for (u32 i = 1; i < length; i++) {
        if (vals[i] < min) {
            min = vals[i];
        }
    }
    return min;
    
}


u32 maximum(u32* vals, u32 length) {
    
    u32 max = vals[0];

    for (u32 i = 1; i < length; i++) {
        if (vals[i] > max) {
            max = vals[i];
        }
    }
    return max;

}


u32 argmin(u32* vals, u32 length) {
    
    u32 min = vals[0];
    u32 argmin = 0;
    
    for (u32 i = 1; i < length; i++) {
        if (vals[i] < min) {
            min = vals[i];
            argmin = i;
        }
    }
    return argmin;

}


u32 argmax(u32* vals, u32 length) {
    
    u32 max = vals[0];
    u32 argmax = 0;
    
    for (u32 i = 1; i < length; i++) {
        if (vals[i] > max) {
            max = vals[i];
            argmax = i;
        }
    }
    return argmax;

}

u32* multiple_argmin(u32* vals, u32 length, u32 out_length) {
    
        u32* argmins = malloc(out_length * sizeof(u32));
        u32* copy = malloc(length * sizeof(u32));
    
        if (argmins == NULL) {
            log_error("Failed to allocate memory for argmins\n");
            return NULL;
        }
    
        if (copy == NULL) {
            log_error("Failed to allocate memory for copy\n");
            return NULL;
        }
    
        for (u32 i = 0; i < length; i++) {
            copy[i] = vals[i];
        }
    
        for (u32 i = 0; i < out_length; i++) {
            argmins[i] = argmin(copy, length);
            copy[argmins[i]] = UINT32_MAX;
        }
    
        free(copy);
    
        return argmins;
    
    
}


u32* multiple_argmax(u32* vals, u32 length, u32 count) {
    
        u32* argmaxs = malloc(count * sizeof(u32));
        u32* copy = malloc(length * sizeof(u32));
    
        if (argmaxs == NULL) {
            log_error("Failed to allocate memory for argmaxs\n");
            return NULL;
        }
    
        if (copy == NULL) {
            log_error("Failed to allocate memory for copy\n");
            return NULL;
        }
    
        for (u32 i = 0; i < length; i++) {
            copy[i] = vals[i];
        }
    
        for (u32 i = 0; i < count; i++) {
            argmaxs[i] = argmax(copy, length);
            copy[argmaxs[i]] = 0;
        }
    
        free(copy);
    
        return argmaxs;
    
    
}


f32* convolute(u32* mask, f32* arr, u32 width, u32 height) {

    // Works only for masks 3x3

    f32* result = malloc(width * height * sizeof(f32));
    if (result == NULL) {
        log_error("Failed to allocate memory for result\n");
        return NULL;
    }

    for (u32 i = 0; i < height; ++i) {
        for (u32 j = 0; j < width; ++j) {
            f32 sum = 0; 
            for (u32 k = 0; k < 3; ++k) {
                for (u32 l = 0; l < 3; ++l) {
                    sum += mask[3*k + l] * arr[(i + k)*(width + 2) + (j + l)];
                }
            }
            result[i*width + j] = sum;
        }
    }

    return result;
}

Extrema extremas(f32* arr, u32 length) {
    
    Extrema result;
    result.min = INFINITY;
    result.max = -INFINITY;

    for (u32 i = 0; i < length; i++) {
        if (arr[i] < result.min) {
            result.min = (double)arr[i];
        }
        if (arr[i] > result.max) {
            result.max = (double)arr[i];
        }
    }

    return result;
}


f32* padded_arr(f32* arr, u32 width, u32 height) {

    u32 new_width = width + 2;
    u32 new_height = height + 2;
    f32* padded = malloc(new_width * new_height * sizeof(f32));

    if (padded == NULL) {
        log_error("Failed to allocate memory padded\n");
        return NULL;
    }

    // Initialize the new array with 0s
    for (u32 i = 0; i < new_width * new_height; i++) {
        padded[i] = 0;
    }

    // Copy the original array u16o the new array, leaving a border of 0s
    for (u32 i = 0; i < height; i++) {
        for (u32 j = 0; j < width; j++) {
            padded[(i+1)*new_width + (j+1)] = arr[i*width + j];
        }
    }

    return padded;
}


u8* sobel_filter(u8* arr, u32 width, u32 height) {

    f32 *lum          = malloc(width * height * sizeof(f32));
    u8  *result       = malloc(width * height * sizeof(u8));
    f32 *sobel_result = malloc(width * height * sizeof(f32));
    f32 *padded;
    f32 *gx, *gy;
    Extrema ext;
    u8 color_byte;
    u32 gx_mask[9] = {-1, 0, 1, -2, 0, 2, -1, 0, 1};
    u32 gy_mask[9] = {-1, -2, -1, 0, 0, 0, 1, 2, 1};

    if (sobel_result == NULL) {
        log_error("Failed to allocate memory for sobel_result\n");
        return NULL;
    }

    if (lum == NULL) {
        log_error("Failed to allocate memory lum\n");
        return NULL;
    }

    if (result == NULL) {
        log_error("Failed to allocate memory for result\n");
        return NULL;
    }

    for (u32 i = 0; i < width * height; ++i) {
        lum[i] = rgb_to_lum(arr[i]);
    }

    padded = padded_arr(lum, width, height);

    gx = convolute(gx_mask, padded, width, height);
    gy = convolute(gy_mask, padded, width, height);

    for (u32 i = 0; i < width * height; ++i) {
        sobel_result[i] = sqrtf(gx[i]*gx[i] + gy[i]*gy[i]);
    }

    ext = extremas(sobel_result, width * height);
    for (u32 i = 0; i < width * height; ++i) {
        color_byte = (u8)(255 * (sobel_result[i] - ext.min) / (ext.max - ext.min));
        result[i] = bytes_to_grayscale(color_byte, 0xFF);
    }
    free(sobel_result);
    free(padded);
    free(lum);

    return result;

}
