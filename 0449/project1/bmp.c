#include "bmp.h"

// This function is used to read and validate a given BMP file.
// It checks that the formatting is correct and puts the header information and
// pixels into the correct structs
BMPImage* readAndValidate(const char *filename) {
    FILE *file = fopen(filename, "rb");
    if (file == NULL) {
        printf("ERROR: File not found.\n");
        return NULL;
    }
    BMPImage *image  = (BMPImage*)malloc(sizeof(BMPImage));
    if (image == NULL) {
        printf("ERROR: Could not allocate enough memory for image.\n");
        fclose(file);
        free(image);
        return NULL;
    }
    fread(&image->bmpHeader, sizeof(BMPFileHeader), 1, file);
    
    if (image->bmpHeader.format[0] != 'B' || image->bmpHeader.format[1] != 'M') {
        printf("ERROR: The format is not supported.\n");
        fclose(file);
        free(image);
        return NULL;
    }
    fread(&image->dibHeader, sizeof(DIBHeader), 1, file);
    
    if (image->dibHeader.headerSize != 40) {
        printf("ERROR: The format is not supported.\n");
        fclose(file);
        free(image);
        return NULL;
    }

    if (image->dibHeader.bitsPerPixel != 24) {
        printf("ERROR: The format is not supported.\n");
        fclose(file);
        free(image);
        return NULL;
    }

    image->pixels = (Pixel*)malloc(image->dibHeader.width * image->dibHeader.height * sizeof(Pixel));
    if(image->pixels == NULL) {
        printf("ERROR: Could not allocate enough memory for pixel data.\n");
        fclose(file);
        free(image);
        return NULL;
    }

    fseek(file, image->bmpHeader.pixelArrayOffset, SEEK_SET);

    for(int row = 0; row < image->dibHeader.height; row++) {
        for(int col = 0; col < image->dibHeader.width; col++) {
            fread(&image->pixels[row * image->dibHeader.width + col], sizeof(Pixel), 1, file);
        }
        int padding = (4 - (image->dibHeader.width * sizeof(Pixel)) % 4) % 4;
        fseek(file, padding, SEEK_CUR);
    }

    fclose(file);
    return image;
}

// This function is used to easily free an image struct
void freeImage(BMPImage *image) {
    free(image->pixels);
    free(image);
}

// This function is used for the --info tag, printing out the header information
void printInfo(const BMPImage *image) {
    printf("=== BMP Header ===\n");
    printf("Type: %c%c\n", image->bmpHeader.format[0], image->bmpHeader.format[1]);
    printf("Size: %u\n", image->bmpHeader.fileSize);
    printf("Reserved 1: %u\n", image->bmpHeader.reserve1);
    printf("Reserved 2: %u\n", image->bmpHeader.reserve2);
    printf("Image Offset: %u\n\n", image->bmpHeader.pixelArrayOffset);
    printf("=== DIB Header ===\n");
    printf("Size: %u\n", image->dibHeader.headerSize);
    printf("Width: %d\n", image->dibHeader.width);
    printf("Height: %d\n", image->dibHeader.height);
    printf("# color planes: %hu\n", image->dibHeader.colorPanes);
    printf("# bits per pixel: %hu\n", image->dibHeader.bitsPerPixel);
    printf("Compression scheme: %u\n", image->dibHeader.compression);
    printf("Image size: %u\n", image->dibHeader.imageSize);
    printf("Horizontal resolution: %d\n", image->dibHeader.xResolution);
    printf("Vertical resolution: %d\n", image->dibHeader.yResolution);
    printf("# colors in palette: %u\n", image->dibHeader.colorsInPallette);
    printf("# important colors: %u\n", image->dibHeader.importantColors);
}

// This function is used to reveal a hidden image in another one, it does this by
// switching the MSB's with the LSB's
BMPImage* revealHiddenImage(BMPImage *image) {
    for(int row = 0; row < image->dibHeader.height; row++) {
        for(int col = 0; col < image->dibHeader.width; col++) {
            Pixel *pixel = &image->pixels[row * image->dibHeader.width + col];
            pixel->red = ((pixel->red & 0xF0) >> 4) | ((pixel->red & 0x0F) << 4);
            pixel->green = ((pixel->green &0xF0) >> 4) | ((pixel->green & 0x0F) << 4);
            pixel->blue = ((pixel->blue &0xF0) >> 4) | ((pixel->blue & 0x0F) << 4);
        }
    }
    return image;
}

// This function is used to write an BMPImage back into a file
void writeImage(const char *filename, BMPImage *image) {
    FILE *file = fopen(filename, "wb");
    if(file == NULL) {
        printf("ERROR: Could not open file for writing");
        return;
    }

    fwrite(&image->bmpHeader, sizeof(BMPFileHeader), 1, file);
    fwrite(&image->dibHeader, sizeof(DIBHeader), 1, file);

    for(int row = 0; row < image->dibHeader.height; row++){
        for(int col = 0; col < image->dibHeader.width; col++){
            fwrite(&image->pixels[row * image->dibHeader.width + col], sizeof(Pixel), 1, file);
        }
        int padding = (4 - (image->dibHeader.width * sizeof(Pixel)) % 4) % 4;
        if (padding > 0) {
            unsigned char paddingBytes[3] = {0, 0, 0};
            fwrite(paddingBytes, padding, 1, file);
        }
    }
}

// This function is used to hide a BMPImage within another given BMPImage
// It does this by butting the MSB's in the image to hide into the LSB's of the
// image to be hidden in
BMPImage* hideImage(BMPImage *image, BMPImage *imageToHide) {
    if(image->dibHeader.width != imageToHide->dibHeader.width || image->dibHeader.height != imageToHide->dibHeader.height) { 
        printf("ERROR: Images must have the same dimension");
        return NULL;
    }
    for(int row = 0; row < image->dibHeader.height; row++){
        for(int col = 0; col < image->dibHeader.width; col++){
            Pixel *pixel = &image->pixels[row * image->dibHeader.width + col];
            Pixel *hiddenPixel = &imageToHide->pixels[row * imageToHide->dibHeader.width + col];
            pixel->red = (pixel->red & 0xF0) | ((hiddenPixel->red & 0xF0) >> 4);
            pixel->green = (pixel->green & 0xF0) | ((hiddenPixel->green & 0xF0) >> 4);
            pixel->blue = (pixel->blue & 0xF0) | ((hiddenPixel->blue & 0xF0) >> 4);
        }
    }
    return image;
}
