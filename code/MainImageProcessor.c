/**
 * Color shifts images based on user input using '-r', -'g', '-b' options for shifting each color channel.
 * Works with bmp and ppm images. User can specify which type is desired for output using '-t' option.
 * Output filename is specified with '-o' option.
 *
 * @author Canellas
 * @version 1.0
 */

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

/**
 * The main entry point of the program. Take in user-defined arguments using getopt
 * and store them in global variables to be used in processImage(), the main image
 * processing function.
 *
 * @param argc: number of user arguments
 * @param argv[]: string containing the user arguments
 */
int main(int argc, char* argv[]) {

    int option;
    opterr = 0;
    while ( (option = getopt(argc, argv, ":o:r:g:b:t")) != -1 ) {
        switch(option) {
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
    for (int i = optind; i < argc; i++) {
        in_filename = argv[i];
    }

	if (processImage(in_filename) != EXIT_SUCCESS) {
		exit(EXIT_FAILURE);
	}
	return EXIT_SUCCESS;
}

/**
 *
 * The main image processing function reads in and writes out the
 * appropriate image headers and pixel array based on user's choice
 * of input and output file types.
 *
 * @param  in_filename: name of the input file to be processed
 * @return  exit failure or exit success flag
 */
int processImage(char* in_filename) {

    //open input file and check for errors
    FILE* in_file = fopen(in_filename, "rb");
    if (in_file == NULL) {
        perror("ERROR: Could not open input file \n");
        return EXIT_FAILURE;
    }

    //read first 2 bytes to determine input filetype
    char signature[2];
    fread(signature, sizeof(char), 2, in_file);
    rewind(in_file); //reset file read position before moving on

    //if the signature is a BMP...
    if (strcmp(signature, BMP_SIGNATURE) == 0) {
        //set the appropriate flag (we'll need this for writing later)
        inputIsBMP = 1;
        //allocate space for and read in appropriate headers
        bmpHeader = malloc(sizeof(struct BMP_Header));
        dibHeader = malloc(sizeof(struct DIB_Header));
        readBMPHeader(in_file, bmpHeader); //stores the image's BMP header into a BMP header struct
        readDIBHeader(in_file, dibHeader); //stores the image's DIB header into a DIB header struct

        int imageHeight = dibHeader->height;
        int imageWidth = dibHeader->width;

        //abort if image exceeds maximum size
        if(imageHeight > MAXIMUM_IMAGE_SIZE || imageWidth > MAXIMUM_IMAGE_SIZE) {
            headerDestructor(bmpHeader);
            headerDestructor(dibHeader);
            fprintf(stderr, "ERROR: Image exceeds maximum image size (%dH x %dW)\n", imageHeight, imageWidth);
            return EXIT_FAILURE;
        }

        //allocate memory for pixel buffer in a 2D array
        image_array = allocateMemoryForPixelArray(imageHeight, imageWidth);

        //read the image's pixels into image_array
        readPixelsBMP(in_file, image_array, imageWidth, imageHeight);

        //color-shift the pixels stored in image_array
        colorShiftPixels(image_array, imageWidth, imageHeight, redShift, greenShift, blueShift);

    //if the signature is a PPM...
    } else if (strcmp(signature, PPM_SIGNATURE) == 0) {
        //inputIsBMP flag already initialized to 0
        //allocate space for and read in appropriate headers
        ppmHeader = malloc(sizeof(struct PPM_Header));
        readPPMHeader(in_file, ppmHeader); //stores the image's PPM header into a PPM header struct

        int imageHeight = ppmHeader->height;
        int imageWidth = ppmHeader->width;

        //abort if image exceeds maximum size
        if(imageHeight > MAXIMUM_IMAGE_SIZE || imageWidth > MAXIMUM_IMAGE_SIZE) {
            headerDestructor(ppmHeader);
            fprintf(stderr, "ERROR: Image exceeds maximum image size (%dH x %dW)\n", imageHeight, imageWidth);
            return EXIT_FAILURE;
        }

        //allocate memory for pixel buffer in a 2D array
        image_array = allocateMemoryForPixelArray(imageHeight, imageWidth);

        //read the image's pixels into image_array
        readPixelsPPM(in_file, image_array, imageWidth, imageHeight);

        //color-shift the pixels stored in image_array
        colorShiftPixels(image_array, imageWidth, imageHeight, redShift, greenShift, blueShift);
    }

    //close the input file
    fclose(in_file);

    //open the output file
    FILE *out_file = fopen(out_filename, "w");
    if (!out_file)
        perror("fopen out");

    //if user did not specify output file format (-t option)
    //or if they specified BMP, set up to write a BMP file...
    if (t_option == 0 || strcmp(outFileFormat, "BMP") == 0) {

        //if input file is a PPM...
        if(inputIsBMP == 0) {
            //allocate space and create the appropriate headers
            bmpHeader = malloc(sizeof(struct BMP_Header));
            dibHeader = malloc(sizeof(struct DIB_Header));
            //these functions create bmp-type headers from a ppm header
            makeBMPHeader(bmpHeader, ppmHeader->width, ppmHeader->height);
            makeDIBHeader(dibHeader, ppmHeader->width, ppmHeader->height);
        }

        //write BMP/DIB headers out to file
        writeBMPHeader(out_file, bmpHeader);
        writeDIBHeader(out_file, dibHeader);

        //write pixels out to file
        writePixelsBMP(out_file, image_array, dibHeader->width, dibHeader->height);

        //close the output file
        fclose(out_file);

        //free all allocated memory
        headerDestructor(bmpHeader);
        headerDestructor(dibHeader);
        freeImageArray(image_array, dibHeader->width, dibHeader->height);
        return(EXIT_SUCCESS);

    //else if user specified output file format (-t option) as PPM,
    //set up to write a PPM file...
    } else if (strcmp(outFileFormat, "PPM") == 0) {

        //if the input file is a BMP...
        if(inputIsBMP == 1) {
            //allocate space and create the appropriate header
            ppmHeader = malloc(sizeof(struct BMP_Header));
            //this function creates a ppm-type header from bmp-type headers
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

/**
 * Allocates memory for and creates the 2D array in which the pixel
 * buffer will be stored
 *
 * @param  imageHeight: height of the image
 * @param  imageWidth: width of the image
 * @return  the pixel array struct in which the pixel buffer will be stored
 **/
struct Pixel** allocateMemoryForPixelArray(int imageHeight, int imageWidth) {
    image_array = (struct Pixel **) malloc(sizeof(struct Pixel*) * imageHeight);
    for (int i = 0; i < imageHeight; i++) {
        image_array[i] = (struct Pixel *) malloc(sizeof(struct Pixel) * imageWidth);
    }
    return image_array;
}

/**
 * destroys header of any type and frees its memory
 **/
void headerDestructor(void* header) {
    if (header) {
        free(header);
    }
}

/**
 * iteratively frees all inner pixel arrays, then frees
 * the outer array
 **/
void freeImageArray(struct Pixel** pArr, int height, int width) {
    for (int i = 0; i < height; i++) {
        free(pArr[i]);
    }
    free(pArr);
}
