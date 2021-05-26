#include <stdio.h>
#include "../include/PpmProcessor.h"
#include "../include/PixelProcessor.h"
#include "../include/BmpProcessor.h"

int main() {
	int option;
	while ( (c = getiot(argc, argv, "i:o")) != -1 ) {
		switch(option) {
			case 'i':
				in_filename = optarg;
				break;
			case 'o':
				out_filename = optarg;
				break;
			case ':':
				fprintf(stderr, "\nERROR : option '-%c' requires an argument\n", optopt);
				exit(EXIT_FAILURE);
			case '?':
			default:
				fprintf(stderr, "\nERROR : option '-%c' is invalid\n", optopt);
		}
	}

	if (processImage() != EXIT_SUCCESS) {
		exit(EXIT_FAILURE);
	}
	headerDestructor();
	imageDestructor(dibHeader->width);
	return EXIT_SUCCESS;
}

int processImage() {
	in_file = fopen(in_filename, "rb");
	if (in_file == NULL) {
		perror(ERR_FILE_OPEN_INPUT);
		return EXIT_FAILURE;
	}

	//define and read headers
	bmpHeader = malloc(sizeof(struct BMP_Header));
	dibHeader = malloc(sizeof(struct DIB_Header));
	readBMPHeader(in_file, BMP_Header);
	readDIBHeader(in_file, DIB_Header);

	imageHeight = dibHeader->height;
	imageWidth = dibHeader->width;

	if(imageHeight > MAXIMUM_IMAGE_SIZE || imageWidth > MAXIMUM_IMAGE_SIZE) {
		headerDestructor();
		fprintf(stderr, "ERROR: Image exceeds maximum image side (%dH x %dW)\n", imageHeight, imageWidth);
		return EXIT_FAILURE;
	}

	//allocate memory for pixel buffer in a 2D array
	image_array = malloc(sizeof(struct Pixel*) * imageHeight);
	for (int i = 0; i < imageWidth; i++) {
		image_array[i] = malloc(sizeof(struct Pixel) * imageWidth);
	}

	//read in the pixels from the image
	readPixelsBMP(in_file, image_array, imageWidth, imageHeight);

	//close the input file
	fclose(in_file);
}
