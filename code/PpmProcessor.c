/**
 * Implements PPM file processing functionality
 *
 * @author Canellas
 * @version 1.0
 */

//STANDARD INCLUDES
#include <stdio.h>
#include <ctype.h>

//USER DEFINED INCLUDES
#include "PpmProcessor.h"

/**
 * read PPM header of a file. Useful for converting files from BMP to PPM.
 *
 * @param  file: A pointer to the file being read or written
 * @param  header: Pointer to the destination PPM header
 */
void readPPMHeader(FILE* file, struct PPM_Header* header){

    fread(header->signature, sizeof(char), 2, file);
    fscanf(file, " ");
    fscanf(file, "%d", &header->width);
    fscanf(file, " ");
    fscanf(file, "%d", &header->height);
    fscanf(file, " ");
    fscanf(file, "%d", &header->maxval);
    fscanf(file, " ");

}


/**
 * write PPM header of a file. Useful for converting files from BMP to PPM.
 *
 * @param  file: A pointer to the file being read or written
 * @param  header: The header made by makePPMHeader function

 */
void writePPMHeader(FILE* file, struct PPM_Header* header){
    fprintf(file, "P6\n%d %d\n%d\n", header->width, header->height, header->maxval);
}

/**
 * make PPM header based on width and height. Useful for converting files from BMP to PPM.
 *
 * @param  header: Pointer to the destination PPM header
 * @param  width: Width of the image that this header is for
 * @param  height: Height of the image that this header is for
 */
void makePPMHeader(struct PPM_Header* header, int width, int height){
    header->signature[0] = 'P';
    header->signature[1] = '6';
    header->width = width;
    header->height = height;
    header->maxval = 255;
}

/**
 * read Pixels from PPM file based on width and height.
 *
 * @param  file: A pointer to the file being read or written
 * @param  pArr: Pixel array of the image that this header is for
 * @param  width: Width of the image that this header is for
 * @param  height: Height of the image that this header is for
 */
void readPixelsPPM(FILE* file, struct Pixel** pArr, int width, int height){
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            fread((&pArr[i][j].red), sizeof(char), 1, file);
            fread((&pArr[i][j].green), sizeof(char), 1, file);
            fread((&pArr[i][j].blue), sizeof(char), 1, file);
            printf("READING blue: %x green: %x red: %x \n", pArr[i][j].blue, pArr[i][j].green, pArr[i][j].red);
        }
    }
}
/**
 * write Pixels from PPM file based on width and height.
 *
 * @param  file: A pointer to the file being read or written
 * @param  pArr: Pixel array of the image that this header is for
 * @param  width: Width of the image that this header is for
 * @param  height: Height of the image that this header is for
 */
void writePixelsPPM(FILE* file, struct Pixel** pArr, int width, int height){
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            fwrite(&((pArr)[i][j].red), sizeof(char), 1, file);
            fwrite(&((pArr)[i][j].green), sizeof(char), 1, file);
            fwrite(&((pArr)[i][j].blue), sizeof(char), 1, file);
            printf("WRITING blue: %x green: %x red: %x \n", ((  pArr)[i][j].blue), ((  pArr)[i][j].green), ((  pArr)[i][j].red));
        }
    }
}
