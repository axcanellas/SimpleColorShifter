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
    /*//read in the magic number
    header->signature[0] = fgetc(file);
    header->signature[1] = fgetc(file);
    //discard whitespace
    readWhitespace(file);
    //read in width
    header->width = readDecimalDigits(file);
    //discard whitespace
    readWhitespace(file);
    //read in height
    header->height = readDecimalDigits(file);
    //discard whitespace
    readWhitespace(file);
    //read in maximum color value
    header->maxval = readDecimalDigits(file);
    //consume that last single whitespace
    fgetc(file);*/


   fscanf(file,"\n%c%c%c%c%c\n",
          &header->signature[0],
          &header->signature[1],
          &header->width,
          &header->height,
          &header->maxval);

}


/**
 * write PPM header of a file. Useful for converting files from BMP to PPM.
 *
 * @param  file: A pointer to the file being read or written
 * @param  header: The header made by makePPMHeader function

 */
void writePPMHeader(FILE* file, struct PPM_Header* header){

}

/**
 * make PPM header based on width and height. Useful for converting files from BMP to PPM.
 *
 * @param  header: Pointer to the destination PPM header
 * @param  width: Width of the image that this header is for
 * @param  height: Height of the image that this header is for
 */
void makePPMHeader(struct PPM_Header* header, int width, int height){

}

/**
 * read Pixels from PPM file based on width and height.
 *
 * @param  file: A pointer to the file being read or written
 * @param  pArr: Pixel array of the image that this header is for
 * @param  width: Width of the image that this header is for
 * @param  height: Height of the image that this header is for
 */
void readPixelsPPM(FILE* file, struct Pixel*** pArr, int width, int height){
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            fread(&(( * pArr)[i][j].green), sizeof(char), 1, file);
            fread(&(( * pArr)[i][j].green), sizeof(char), 1, file);
            fread(&(( * pArr)[i][j].blue), sizeof(char), 1, file);
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

}

/*
 * read bytes and convert them to their decimal equivalents
 *
 * @param file: the file pointer
 * @return int: decimal form of the number that was read in
 */
int readDecimalDigits(FILE *file) {
    int c = getc(file);
    //convert to decimal
    int val = charToDecimal(c);

    while(1) {
        //get the following char
        c = getc(file);
        //return if whitespace or EOF
        if(isspace(c) == ' ' && c != EOF) {
            //don't consume the character
            ungetc(c, file);
            return val;
        }

        int digit = charToDecimal(c);

        val = (val*10) + digit;
    }

}

/*
 * Read whitespace in PPM header
 *
 * @param: pointer to the file being read
 */
void readWhitespace(FILE *file) {
    int c;
    //skip leading whitespace
    c = getc(file);
    if (c == ' ' && c != EOF) {
        c = getc(file);
    }
    //don't consume the last character
    ungetc(c, file);
}

/*
 * Convert a single digit represented as a char into its respective decimal value
 *
 * @param: int val: number to convert to integer
 */
int charToDecimal(int val) {
    switch(val) {
        case '0':
            return 0;
        case '1':
            return 1;
        case '2':
            return 2;
        case '3':
            return 3;
        case '4':
            return 4;
        case '5':
            return 5;
        case '6':
            return 6;
        case '7':
            return 7;
        case '8':
            return 8;
        case '9':
            return 9;
        default:
            return -1;
    }
}
