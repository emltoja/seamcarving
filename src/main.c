#include <stdio.h>
#include <assert.h>
#include "..\headers\bitarray.h"
#include "..\headers\types.h"
#include "..\headers\filters.h"
#include "..\headers\carving.h"
#include "..\headers\log.h"

#define STB_IMAGE_IMPLEMENTATION
#include "..\headers\stb_image.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "..\headers\stb_image_write.h"

int main(int argc, char** argv) {

    assert (argc == 4), "Usage: .\\main.exe <input_image_path> <output_image_path> <width_trim>";

    u8* pixels;
    i32 width, height;
    i32 new_width;
    u32 width_trim = atoi(argv[3]);

    const char* filepath = argv[1];
    const char* outpath = argv[2];

    pixels = stbi_load(filepath, &width, &height, NULL, 4);
    if (pixels == NULL) {
        log_error("Failed to load image: %s\n", stbi_failure_reason());
        return 2;
    } else {
        log_info("Loaded image with width: %d, height: %d\n", width, height);
    }

    remove_multiple_vertical_seams(pixels, width, height, width_trim, &new_width);
    // mark_multiple_vertical_seams(pixels, width, height, width_trim);

    stbi_write_jpg(outpath, new_width, height, 4, pixels, 100);

    log_info("Saved image to: %s\n", outpath);

    free(pixels);

    return 0; 
}