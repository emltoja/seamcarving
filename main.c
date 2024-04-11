#include <stdio.h>
#include <assert.h>
#include "bitarray.h"
#include "types.h"
#include "filters.h"
#include "carving.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

int main(int argc, char** argv) {

    assert (argc == 4), "Usage: .\\main.exe <input_image_path> <output_image_path> <width_trim>";

    u32* pixels;
    u32 width, height;
    u32 width_trim = atoi(argv[3]);

    const u8* filepath = argv[1];
    const u8* outpath = argv[2];

    pixels = stbi_load(filepath, &width, &height, NULL, 4);
    if (pixels == NULL) {
        printf("Failed to load image: %s\n", stbi_failure_reason());
        return 2;
    }


    remove_multiple_vertical_seams(pixels, width, height, width_trim, &width);
    // mark_multiple_vertical_seams(pixels, width, height, width_trim);

    stbi_write_jpg(outpath, width, height, 4, pixels, 100);

    free(pixels);
    return 0; 
}