#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"
#define STB_IMAGE_RESIZE_IMPLEMENTATION
#include "stb_image_resize.h"

#include "imageutil.h"


/*
*   Main function that takes in image paths and function names as arguments and applies the functions to the images.
*   Outputs the result to a directory named "output". If the directory does not exist, it will be created.
*
*   @param argc the number of arguments passed to the program
*   @param argv an array of strings containing the arguments
*   @returns 0 on successful completion of the program
*/
int main(int argc, char** argv){
    if (argc < 4){
        printf("Usage: ./program <function> <input file 1> <input file 2 (only needed for add, subtract, or dot)> <output directory>\n");
        return 1;
    }

    char* function = argv[1];
    char* input_file_1 = argv[2];
    char* input_file_2 = argc > 4 ? argv[3] : NULL;
    char* output_directory = argv[argc-1];

    int width, height, channels;
    imatrix* input_image_1 = init_from_file(input_file_1, &width, &height, &channels);
    imatrix* input_image_2 = NULL;

    if (input_file_2 != NULL){
        input_image_2 = init_from_file(input_file_2, &width, &height, &channels);
    }

    imatrix* output_image = NULL;

    if (strcmp(function, "add") == 0 && input_image_2 != NULL){
        output_image = input_image_1->add(input_image_1, input_image_2);
    }
    else if (strcmp(function, "subtract") == 0 && input_image_2 != NULL){
        output_image = input_image_1->subtract(input_image_1, input_image_2);
    }
    else if (strcmp(function, "dot") == 0 && input_image_2 != NULL){
        output_image = input_image_1->dot(input_image_1, input_image_2);
    }
    else if (strcmp(function, "scale") == 0){
        float alpha = 0.5; // default value
        if (argc > 4){
            alpha = atof(argv[4]);
        }
        output_image = input_image_1->scale(input_image_1, width, height, alpha);
    }
    else {
        printf("Invalid function name or insufficient number of arguments\n");
        return 1;
    }

    char output_filename[100];
    snprintf(output_filename, 100, "%s/output.png", output_directory);
    output_image->write_rgb_to_image(output_image);
    stbi_write_png(output_filename, output_image->width, output_image->height, CHANNEL_NUM, output_image->rgb_image, output_image->width * CHANNEL_NUM);

    free_imatrix(input_image_1);
    if (input_image_2 != NULL){
        free_imatrix(input_image_2);
    }
    free_imatrix(output_image);

    return 0;

}
