//STANDARD INCLUDES
#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <unistd.h>

//USER WRITTEN INCLUDES
//#include "PpmProcessor.h"
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
                redShift = optarg;
                break;
            case 'g':
                greenShift = optarg;
                break;
            case 'b':
                blueShift = optarg;
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

	//define and read headers
    bmpHeader = malloc(sizeof(struct BMP_Header));
    dibHeader = malloc(sizeof(struct DIB_Header));
	readBMPHeader(in_file, bmpHeader);
    readDIBHeader(in_file, dibHeader);

	int imageHeight = dibHeader->height;
	int imageWidth = dibHeader->width;

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
	readPixelsBMP(in_file, image_array, imageWidth, imageHeight);

	//close the input file
	fclose(in_file);

	//open the output file
	FILE *out_file = fopen(out_filename, "w");
	if (!out_file)
	    perror("fopen");

    //write headers out to file
    writeBMPHeader(out_file, bmpHeader);
    writeDIBHeader(out_file, dibHeader);

    //write pixels out to file
    writePixelsBMP(out_file, image_array, imageWidth, imageHeight);

    //close the output file
    fclose(out_file);
	return(EXIT_SUCCESS);
}
