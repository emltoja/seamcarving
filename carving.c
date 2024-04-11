#include "carving.h"

u32* lowest_energy_seam(u32* energy_map, u32 width, u32 height) {

    u32* cum_energies = malloc(width * height * sizeof(u32));
    u32* seam = malloc(height * sizeof(u32));
    u32 left, mid, right;

    if (seam == NULL) {
        printf("Failed to allocate memory for seam\n");
        return NULL;
    }

    if (cum_energies == NULL) {
        printf("Failed to allocate memory for cum_energies \n");
        return NULL;
    }

    for (u32 i = 0; i < width; ++i) {
        cum_energies[i] = energy_map[i];
    }

    for (u32 i = 1; i < height; ++i) {
        for (u32 j = 0; j < width; ++j) {
            left = j == 0 ? UINT32_MAX : cum_energies[(i - 1)*width + j - 1];
            mid = cum_energies[(i - 1)*width + j];
            right = j == width - 1 ? UINT32_MAX : cum_energies[(i - 1)*width + j + 1];
            cum_energies[i*width + j] = energy_map[i*width + j] + minimum((u32[]){left, mid, right}, 3);
        }
    }

    seam[height - 1] = argmin(cum_energies + (height - 1)*width, width);
    
    for (u32 i = height - 2; i >= 0; --i) {
        u32 j = seam[i + 1];
        left = j == 0 ? UINT32_MAX : cum_energies[i*width + j - 1];
        mid = cum_energies[i*width + j];
        right = j == width - 1 ? UINT32_MAX : cum_energies[i*width + j + 1];
        seam[i] = j + argmin((u32[]){left, mid, right}, 3) - 1;
    }

    free(cum_energies);

    return seam;

}


u32** get_multiple_lowest_energy_seams(u32* energy_map, u32 width, u32 height, u32 num_seams) {

    u32** seams = malloc(num_seams * sizeof(u32*));
    u32* last_row;
    unsigned char* already_used = get_bitarray(width * height);
    u32 left, mid, right;
    u32* cum_energies = malloc(width * height * sizeof(u32));

    for (u32 i = 0; i < width * height; i++) {
        clear_bit(already_used, i);
    }

    if (seams == NULL) {
        printf("Failed to allocate memory for seams\n");
        return NULL;
    }

    for (u32 i = 0; i < num_seams; i++) {
        seams[i] = malloc(height * sizeof(u32));
        if (seams[i] == NULL) {
            printf("Failed to allocate memory for seam %d\n", i);
            return NULL;
        }
    }

    for (u32 i = 0; i < width; ++i) {
        cum_energies[i] = energy_map[i];
    }

    for (u32 i = 1; i < height; ++i) {
        for (u32 j = 0; j < width; ++j) {
            left = j == 0 ? UINT32_MAX : cum_energies[(i - 1)*width + j - 1];
            mid = cum_energies[(i - 1)*width + j];
            right = j == width - 1 ? UINT32_MAX : cum_energies[(i - 1)*width + j + 1];
            cum_energies[i*width + j] = energy_map[i*width + j] + minimum((u32[]){left, mid, right}, 3);
        }
    }

    last_row = multiple_argmin(cum_energies + (height - 1)*width, width, num_seams);

    for (u32 i = 0; i < num_seams; ++i) {
        seams[i][height - 1] = last_row[i];
        for (u32 j = height - 2; j >= 0; --j) {

            u32 k = seams[i][j + 1];
            u32 idx = j*width + k;

            left = ((k == 0) | get_bit(already_used, idx - 1)) ? UINT32_MAX : cum_energies[idx- 1];
            mid = get_bit(already_used, idx) ? UINT32_MAX : cum_energies[idx];
            right = ((k == width - 1) | get_bit(already_used, idx + 1)) ? UINT32_MAX : cum_energies[idx + 1];

            u32 chosen = k + argmin((u32[]){left, mid, right}, 3) - 1;

            set_bit(already_used, j*width + chosen);
            seams[i][j] = chosen;
            
        }
    }

    free(cum_energies);
    free(last_row);
    free(already_used);

    return seams;

}

void mark_seam_red_vertical(u32* img, u32* seam, u32 width, u32 height) {
    for (u32 i = 0; i < height; ++i) {
        img[i*width + seam[i]] = bytes_to_rgba(0xFF, 0, 0, 0xFF);
    }
}

void mark_seam_red_horizontal(u32* img, u32* seam, u32 width) {
    for (u32 i = 0; i < width; ++i) {
        img[seam[i]*width + i] = bytes_to_rgba(0xFF, 0, 0, 0xFF);
    }
}


void transpose_image(u32* img, u32 width, u32 height) {

    u32* transposed = malloc(width * height * sizeof(u32));

    for (u32 i = 0; i < height * width; ++i) {
        transposed[i] = img[i];
    }

    for (u32 i = 0; i < height; ++i) {
        for (u32 j = 0; j < width; ++j) {
            img[i*width + j] = transposed[j*height + i];
        }
    }

    free(transposed);

}



void remove_seam(u32* img, u32* seam, u32 width, u32 height, u32* new_width) {
    
    u32 step = 0;
    for (u32 i = 0; i < height * (width - 1); ++i) {
        if ((i+step) % width == seam[step]) {
            step++;
        }
        img[i] = img[i+step];
    }
    *new_width = width - 1;
}


void mark_multiple_vertical_seams(u32* img, u32 width, u32 height, u32 num_seams) {

    u32* energy_map;
    u32 energy_map_width = width;
    // u32** seams = get_multiple_lowest_energy_seams(energy_map, width, height, num_seams);

    for (u32 i = 0; i < num_seams; ++i) {
        energy_map = sobel_filter(img, energy_map_width, height);
        u32* seam = lowest_energy_seam(energy_map, energy_map_width, height);
        mark_seam_red_vertical(img, seam, width, height);
        remove_seam(energy_map, seam, width, height, &width);
    }

    free(energy_map);

}


u32* translate_seam(u32** previous_seams, u32* seam, u32 count, u32 height) {
    
        u32* translated = malloc(height * sizeof(u32));
        if (translated == NULL) {
            printf("Failed to allocate memory for translated\n");
            return NULL;
        }
    
        for (u32 i = 0; i < height; i++) {
            translated[i] = seam[i];
        }
    
        for (u32 i = 0; i < count; i++) {
            for (u32 j = 0; j < height; j++) {
                if (translated[j] >= previous_seams[i][j]) {
                    translated[j]++;
                }
            }
        }
    
        return translated;
}

void remove_multiple_vertical_seams(u32* img, u32 width, u32 height, u32 num_seams, u32* new_width) {

    u32* energy_map;
    u32* seam;
    
    *new_width = width;
    for (u32 i = 0; i < num_seams; ++i) {
        energy_map = sobel_filter(img, *new_width, height);
        seam = lowest_energy_seam(energy_map, *new_width, height);
        remove_seam(img, seam, *new_width, height, new_width);
    }

}
