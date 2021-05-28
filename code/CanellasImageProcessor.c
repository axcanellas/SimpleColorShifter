//STANDARD INCLUDES
#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <unistd.h>

//USER WRITTEN INCLUDES
#include "PpmProcessor.h"
#include "PixelProcessor.h"
#include "BmpProcessor.h"

//FORWARD DECLARATIONS
int processImage();

//MACRO DEFINITIONS
#define MAX_FILENAME_LENGTH 32
#define MAXIMUM_IMAGE_SIZE 1000

//GLOBAL VARIABLES
//int imageWidth, imageHeight;
char* in_filename;
char* out_filename;
int redShift, greenShift, blueShift;

struct BMP_Header* bmpHeader;
struct DIB_Header* dibHeader;
struct PPM_Header* ppmHeader;
struct Pixel** image_array;

int main(int argc, char* argv[]) {

	int option;
    opterr = 0;
	while ( (option = getopt(argc, argv, "i:o:r:g:b:")) != -1 ) {
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

	//if infile is a BMP, define and read headers accordingly
		/*bmpHeader = malloc(sizeof(struct BMP_Header));
		dibHeader = malloc(sizeof(struct DIB_Header));
		readBMPHeader(in_file, bmpHeader);
		readDIBHeader(in_file, dibHeader);

		int imageHeight = dibHeader->height;
		int imageWidth = dibHeader->width;
        */

	//else, if infile is PPM, define and read headers accordingly
		ppmHeader = malloc(sizeof(struct PPM_Header));
		readPPMHeader(in_file, ppmHeader);
		int imageHeight = ppmHeader->height;
		int imageWidth = ppmHeader->width;

	if(imageHeight > MAXIMUM_IMAGE_SIZE || imageWidth > MAXIMUM_IMAGE_SIZE) {
		//headerDestructor();
		fprintf(stderr, "ERROR: Image exceeds maximum image side (%dH x %dW)\n", imageHeight, imageWidth);
		return EXIT_FAILURE;
	}

	//allocate memory for pixel buffer in a 2D array
	image_array = (struct Pixel **) malloc(sizeof(struct Pixel*) * imageHeight);
	for (int i = 0; i < imageWidth; i++) {
		image_array[i] = (struct Pixel *) malloc(sizeof(struct Pixel) * imageWidth);
	}

	//read in the pixels from the image
	//TODO: readPixelsBMP(in_file, image_array, imageWidth, imageHeight);
	readPixelsPPM(in_file, &image_array, imageWidth, imageHeight);

	//close the input file
	fclose(in_file);

	//color-shift the pixels stored in image_array
	colorShiftPixels(image_array, imageWidth, imageHeight, redShift, greenShift, blueShift);

	//open the output file
	FILE *out_file = fopen(out_filename, "w");
	if (!out_file)
		perror("fopen");

	//if outfile is a BMP, write BMP/DIB headers out to file
	//TODO: writeBMPHeader(out_file, bmpHeader);
	//TODO: writeDIBHeader(out_file, dibHeader);

	//else if outfile is a PPM, write PPM header out to file
	// [TODO]

	//write pixels out to file
	writePixelsBMP(out_file, image_array, imageWidth, imageHeight);

	//close the output file
	fclose(out_file);

	//free all allocated memory
	// [TODO]
	return(EXIT_SUCCESS);
}
