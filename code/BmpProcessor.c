#include <stdio.h>
#include <optarg.h>
#include "../include/BmpProcessor.h"

//fread(addressData, sizeData, numbersData, ptrToFile)
//fwrite(addressData, sizeData, numbersData, pointerToFile)

/**
 * read BMP header of a file. Useful for converting files from PPM to BMP.
 *
 * @param  file: A pointer to the file being read or written
 * @param  header: Pointer to the destination BMP header
 */
void readBMPHeader(FILE* file, struct BMP_Header* header) {

 file = fopen(filename, "rb");

    if(file != NULL ) {
			//read in the bitmap bmp header
			fread(&header, sizeof(struct BMP_Header), 1, file);
			//check ID field to verify format
			if (header->signature[0] != 'B' || header->signature[1] != 'M') {
				fclose(file);
			}
			readDIBHeader(file)
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
			//read in the DIB header
			fread()
}
