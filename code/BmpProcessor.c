/**
 * Implements BMP file processing functionality
 *
 * @author Canellas
 * @version 1.0
 */

#include <stdio.h>
#include <stdlib.h>
#include "BmpProcessor.h"
#include "PixelProcessor.h"

/**
 * read BMP header of a file. Useful for converting files from PPM to BMP.
 *
 * @param  file: A pointer to the file being read or written
 * @param  header: Pointer to the destination BMP header
 */
void readBMPHeader(FILE* file, struct BMP_Header* header) {

    if(file != NULL ) {
        //read in the bitmap bmp header
        fread(header->signature, (sizeof(unsigned char)), 2, file);
        fread(&header->size, (sizeof (unsigned int)), 1, file);
        fread(&header->reserved1, (sizeof (unsigned short)), 1, file);
        fread(&header->reserved2, (sizeof (unsigned short)), 1, file);
        fread(&header->offset_pixel_array, (sizeof (unsigned int)), 1, file);
		} else {
			printf("Could not open file.\n");
    }
}

/**
 * read DIB header from a file. Useful for converting files from PPM to BMP.
 *
 * @param  file: A pointer to the file being read or written
 * @param  header: Pointer to the destination DIB header
 */
void readDIBHeader(FILE* file, struct DIB_Header* header) {

    if(file != NULL ) {
        //read in the bitmap dib header
        fread(&header->size, sizeof(unsigned int), 1, file);
        fread(&header->width, sizeof(unsigned int), 1, file);
        fread(&header->height, sizeof(unsigned int), 1, file);
        fread(&header->planes, sizeof(unsigned short), 1, file);
        fread(&header->bits_per_pixel, sizeof(unsigned short), 1, file);
        fread(&header->compression, sizeof(unsigned int), 1, file);
        fread(&header->image_size, sizeof(unsigned int), 1, file);
        fread(&header->x_pixels_per_meter, sizeof(unsigned int), 1, file);
        fread(&header->y_pixels_per_meter, sizeof(unsigned int), 1, file);
        fread(&header->color_palette_size, sizeof(unsigned int), 1, file);
        fread(&header->important_color_count, sizeof(unsigned int), 1, file);
		} else {
			printf("Could not open file.\n");
    }
}

/**
 * read Pixels from BMP file based on width and height.
 *
 * @param  file: A pointer to the file being read or written
 * @param  pArr: Pixel array of the image that this header is for
 * @param  width: Width of the image that this header is for
 * @param  height: Height of the image that this header is for
 */
void readPixelsBMP(FILE* file, struct Pixel** pArr, int width, int height){
	for (int i = height-1; i>=0; i--) {
	    for (int j = 0; j<width; j++) {
            fread((&pArr[i][j].blue), sizeof(char), 1, file);
            fread((&pArr[i][j].green), sizeof(char), 1, file);
            fread((&pArr[i][j].red), sizeof(char), 1, file);
            printf("READING blue: %x green: %x red: %x \n", pArr[i][j].blue, pArr[i][j].green, pArr[i][j].red);
        }
	    int paddingRemainder = (width*3) % 4;
	    unsigned char* pad = malloc(sizeof(unsigned char) * paddingRemainder);
        fread(pad, sizeof(unsigned char), paddingRemainder, file);
	}
}

/**
 * write BMP header of a file. Useful for converting files from PPM to BMP.
 *
 * @param  file: A pointer to the file being read or written
 * @param  header: The header made by makeBMPHeader function
 */
void writeBMPHeader(FILE* file, struct BMP_Header* header){
    fwrite(&header->signature, (sizeof header->signature), 1, file);
    fwrite(&header->size, (sizeof header->size), 1, file);
    fwrite(&header->reserved1, (sizeof header->reserved1), 1, file);
    fwrite(&header->reserved2, (sizeof header->reserved2), 1, file);
    fwrite(&header->offset_pixel_array, (sizeof header->offset_pixel_array), 1, file);
}

/**
 * write DIB header of a file. Useful for converting files from PPM to BMP.
 *
 * @param  file: A pointer to the file being read or written
 * @param  header: The header made by makeDIBHeader function
 */
void writeDIBHeader(FILE* file, struct DIB_Header* header){
    fwrite(&header->size, sizeof(header->size), 1, file);
    fwrite(&header->width, sizeof(header->width), 1, file);
    fwrite(&header->height, sizeof(header->height), 1, file);
    fwrite(&header->planes, sizeof(header->planes), 1, file);
    fwrite(&header->bits_per_pixel, sizeof(header->bits_per_pixel), 1, file);
    fwrite(&header->compression, sizeof(header->compression), 1, file);
    fwrite(&header->image_size, sizeof(header->image_size), 1, file);
    fwrite(&header->x_pixels_per_meter, sizeof(header->x_pixels_per_meter), 1, file);
    fwrite(&header->y_pixels_per_meter, sizeof(header->y_pixels_per_meter), 1, file);
    fwrite(&header->color_palette_size, sizeof(header->color_palette_size), 1, file);
    fwrite(&header->important_color_count, sizeof(header->important_color_count), 1, file);
}

/**
 * write Pixels from BMP file based on width and height.
 *
 * @param  file: A pointer to the file being read or written
 * @param  pArr: Pixel array of the image that this header is for
 * @param  width: Width of the image that this header is for
 * @param  height: Height of the image that this header is for
 */
void writePixelsBMP(FILE* file, struct Pixel** pArr, int width, int height){
    for (int i = height-1; i>=0; i--) {
        for (int j = 0; j<width; j++) {
            fwrite((&pArr[i][j].blue), sizeof(unsigned char), 1, file);
            fwrite((&pArr[i][j].green), sizeof(unsigned char), 1, file);
            fwrite((&pArr[i][j].red), sizeof(unsigned char), 1, file);
            printf("WRITING blue: %x green: %x red: %x \n", pArr[i][j].blue, pArr[i][j].green, pArr[i][j].red);
        }
        int paddingRemainder = (width*3) % 4;
        unsigned char* pad = malloc(sizeof(unsigned char) * paddingRemainder);
        fwrite(pad, sizeof(unsigned char), paddingRemainder, file);
    }
}

void makeBMPHeader(struct BMP_Header *header, int width, int height) {
    header->signature[0] = 'B';
    header->signature[1] = 'M';
    header->size = width * height;
    header->reserved1 = 0;
    header->reserved2 = 0;
    header->offset_pixel_array = 54;
}

void makeDIBHeader(struct DIB_Header *header, int width, int height) {
    header->size = 40;
    header->width = width;
    header->height = height;
    header->planes = 1;
    header->bits_per_pixel = 24;
    header->compression = 0;
    header->image_size = width * height;
    header->x_pixels_per_meter = width;
    header-> y_pixels_per_meter = height;
    header->color_palette_size = 0;
    header->important_color_count = 0;
}
