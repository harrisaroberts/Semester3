#ifndef BMP_H
#define BMP_H

#include <stdio.h>
#include <stdlib.h>

// Define structures
// This struct is used for the BMP file header
#pragma pack(1)
typedef struct {
    char format[2];
    unsigned int fileSize;
    unsigned short reserve1;
    unsigned short reserve2;
    unsigned int pixelArrayOffset;
} BMPFileHeader;
#pragma pack(4)

// This struct is used for the DIB header
#pragma pack(1)
typedef struct {
    unsigned int headerSize;
    int width;
    int height;
    unsigned short colorPanes;
    unsigned short bitsPerPixel;
    unsigned int compression;
    unsigned int imageSize;
    int xResolution;
    int yResolution;
    unsigned int colorsInPallette;
    unsigned int importantColors;
} DIBHeader;
#pragma pack(4)

// This struct is used for the pixel array
#pragma pack(1)
typedef struct {
    unsigned char blue;
    unsigned char green;
    unsigned char red;
} Pixel;
#pragma pack(4)

// This struct is used to organize all the other structs into one BMPImage, holding
// all the information about a given BMP image
#pragma pack(1)
typedef struct {
    BMPFileHeader bmpHeader;
    DIBHeader dibHeader;
    Pixel *pixels;
} BMPImage;
#pragma pack(4)

// Define functions
BMPImage* readAndValidate(const char *filename);
void freeImage(BMPImage *image);
void printInfo(const BMPImage *image);
BMPImage* revealHiddenImage(BMPImage *image);
void writeImage(const char *filename, BMPImage *image);
BMPImage* hideImage(BMPImage *image, BMPImage *imageToHide);

#endif
