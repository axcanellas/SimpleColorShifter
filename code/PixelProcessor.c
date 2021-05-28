//STANDARD INCLUDES
#include <stdio.h>

//USER WRITTEN INCLUDES
#include "BmpProcessor.h"

//FORWARD DECLARATIONS
int clampPixels(int val);

/**
 * Shift color of Pixel array. The dimension of the array is width * height. The shift value of RGB is
 * rShift, gShift，bShift. Useful for color shift.
 *
 * @param  pArr: Pixel array of the image that this header is for
 * @param  width: Width of the image that this header is for
 * @param  height: Height of the image that this header is for
 * @param  rShift: the shift value of color r shift
 * @param  gShift: the shift value of color g shift
 * @param  bShift: the shift value of color b shift
 */
void colorShiftPixels(struct Pixel** pArr, int width, int height, int rShift, int gShift, int bShift) {
    for (int i = 0; i < width; i++) {
        for (int j = 0; j < height; j++) {
            pArr[i][j].red = clampPixels(pArr[i][j].red + rShift);
            pArr[i][j].green = clampPixels(pArr[i][j].blue + bShift);
            pArr[i][j].blue = clampPixels(pArr[i][j].green + gShift);

        }
    }
}

/**
 * Clamp pixel values so they stay within 0 and 255
 * @param value: the value to be clamped
 * @return 0 is under 0, 255 if over 255, otherwise return original value
 */
int clampPixels(int val) {
    if (val > 255) {
        return 255;
    }
    if (val < 0) {
        return 0;
    }
    else {
        return val;
    }
}
