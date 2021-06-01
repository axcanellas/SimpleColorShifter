//STANDARD INCLUDES
#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <unistd.h>
#include <string.h>

//USER WRITTEN INCLUDES
#include "PpmProcessor.h"
#include "PixelProcessor.h"
#include "BmpProcessor.h"

//FORWARD DECLARATIONS
int processImage();
struct Pixel** allocateMemoryForPixelArray(int imageHeight, int imageWidth);
void freeImageArray(struct Pixel** pArr, int height, int width);
void headerDestructor(void* header);

//MACRO DEFINITIONS
#define MAX_FILENAME_LENGTH 32
#define MAXIMUM_IMAGE_SIZE 1000
#define BMP_SIGNATURE "BM"
#define PPM_SIGNATURE "P6"

//GLOBAL VARIABLES
//int imageWidth, imageHeight;
char* in_filename;
char* out_filename;
char* outFileFormat;
int redShift = 0;
int greenShift = 0;
int blueShift = 0;
int t_option = 0;
int inputIsBMP = 0;

struct BMP_Header* bmpHeader;
struct DIB_Header* dibHeader;
struct PPM_Header* ppmHeader;
struct Pixel** image_array;

int main(int argc, char* argv[]) {

	int option;
    opterr = 0;
	while ( (option = getopt(argc, argv, ":i:o:r:g:b:t")) != -1 ) {
		switch(option) {
			case 'i':
				in_filename = optarg;
                break;
			case 'o':
				out_filename = optarg;
				break;
			case 'r':
				redShift = atoi(optarg);
				break;
			case 'g':
				greenShift = atoi(optarg);
				break;
            case 'b':
                blueShift = atoi(optarg);
                break;
            case 't':
                outFileFormat = optarg;
                t_option = 1;
                break;
			case ':':
				fprintf(stderr, "\nERROR : option '-%c' requires an argument\n", optopt);
				exit(EXIT_FAILURE);
			case '?':
			default:
				fprintf(stderr, "\nERROR : option '-%c' is invalid\n", optopt);
		}
	}

	if (processImage(in_filename) != EXIT_SUCCESS) {
		exit(EXIT_FAILURE);
	}
	//headerDestructor();
	//imageDestructor(dibHeader->width);
	return EXIT_SUCCESS;
}

int processImage(char* in_filename) {

	FILE* in_file = fopen(in_filename, "rb");
	if (in_file == NULL) {
		perror("ERROR: Could not open input file \n");
		return EXIT_FAILURE;
	}

	//read first 2 bytes to determine input filetype
    unsigned char signature[2];
    fread(signature, sizeof(char), 2, in_file);
    rewind(in_file);
    if (strcmp(signature, BMP_SIGNATURE) == 0) {
        inputIsBMP = 1;
        bmpHeader = malloc(sizeof(struct BMP_Header));
        dibHeader = malloc(sizeof(struct DIB_Header));
        readBMPHeader(in_file, bmpHeader);
        readDIBHeader(in_file, dibHeader);

        int imageHeight = dibHeader->height;
        int imageWidth = dibHeader->width;

        if(imageHeight > MAXIMUM_IMAGE_SIZE || imageWidth > MAXIMUM_IMAGE_SIZE) {
            headerDestructor(bmpHeader);
            headerDestructor(dibHeader);
            fprintf(stderr, "ERROR: Image exceeds maximum image side (%dH x %dW)\n", imageHeight, imageWidth);
            return EXIT_FAILURE;
        }

        //allocate memory for pixel buffer in a 2D array
        image_array = allocateMemoryForPixelArray(imageHeight, imageWidth);

        //read in the pixels from the image
        readPixelsBMP(in_file, image_array, imageWidth, imageHeight);

        //close the input file
        fclose(in_file);

        //color-shift the pixels stored in image_array
        colorShiftPixels(image_array, imageWidth, imageHeight, redShift, greenShift, blueShift);

    } else if (strcmp(signature, PPM_SIGNATURE) == 0) {
        ppmHeader = malloc(sizeof(struct PPM_Header));
        readPPMHeader(in_file, ppmHeader);
        int imageHeight = ppmHeader->height;
        int imageWidth = ppmHeader->width;

        if(imageHeight > MAXIMUM_IMAGE_SIZE || imageWidth > MAXIMUM_IMAGE_SIZE) {
            headerDestructor(ppmHeader);
            fprintf(stderr, "ERROR: Image exceeds maximum image side (%dH x %dW)\n", imageHeight, imageWidth);
            return EXIT_FAILURE;
        }

        //allocate memory for pixel buffer in a 2D array
        image_array = allocateMemoryForPixelArray(imageHeight, imageWidth);

        //read in the pixels from the image
        readPixelsPPM(in_file, image_array, imageWidth, imageHeight);

        //close the input file
        fclose(in_file);

        //color-shift the pixels stored in image_array
        colorShiftPixels(image_array, imageWidth, imageHeight, redShift, greenShift, blueShift);

    }

    //open the output file
    FILE *out_file = fopen(out_filename, "w");
    if (!out_file)
        perror("fopen out");

    if (t_option == 0 || strcmp(outFileFormat, "BMP") == 0) {

        if(inputIsBMP == 0) {
            bmpHeader = malloc(sizeof(struct BMP_Header));
            dibHeader = malloc(sizeof(struct DIB_Header));
            makeBMPHeader(bmpHeader, ppmHeader->width, ppmHeader->height);
            makeDIBHeader(dibHeader, ppmHeader->width, ppmHeader->height);
        }

        //if outfile is a BMP, write BMP/DIB headers out to file
        writeBMPHeader(out_file, bmpHeader);
        writeDIBHeader(out_file, dibHeader);

        //write pixels out to file
        writePixelsBMP(out_file, image_array, dibHeader->width, dibHeader->height);

        //close the output file
        fclose(out_file);

        //free all allocated memory
        headerDestructor(bmpHeader);
        headerDestructor(dibHeader);
        return(EXIT_SUCCESS);

    } else if (strcmp(outFileFormat, "PPM") == 0) {

        if(inputIsBMP == 1) {
            ppmHeader = malloc(sizeof(struct BMP_Header));
            makePPMHeader(ppmHeader, dibHeader->width, dibHeader->height);
        }

        //write PPM header out to file
        writePPMHeader(out_file, ppmHeader);

        //write pixels out to file
        writePixelsPPM(out_file, image_array, ppmHeader->width, ppmHeader->height);

        //close the output file
        fclose(out_file);

        //free all allocated memory
        headerDestructor(ppmHeader);
        freeImageArray(image_array, ppmHeader->width, ppmHeader->height);
        image_array = NULL;

        return (EXIT_SUCCESS);
    }
    return (EXIT_FAILURE);
}

struct Pixel** allocateMemoryForPixelArray(int imageHeight, int imageWidth) {
    image_array = (struct Pixel **) malloc(sizeof(struct Pixel*) * imageHeight);
    for (int i = 0; i < imageHeight; i++) {
        image_array[i] = (struct Pixel *) malloc(sizeof(struct Pixel) * imageWidth);
    }
    return image_array;
}

void headerDestructor(void* header) {
    if (header) {
        free(header);
    }
}

void freeImageArray(struct Pixel** pArr, int height, int width) {
    for (int i = 0; i < height; i++) {
        free(pArr[i]);
    }
    free(pArr);
}

