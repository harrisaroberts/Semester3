#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#pragma pack(1)
typedef struct {
    char format[2];
    unsigned int fileSize;
    unsigned short reserve1;
    unsigned short reserve2;
    unsigned int pixelArrayOffset;
} BMPFileHeader;
#pragma pack(4)

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

#pragma pack(1)
typedef struct {
    unsigned char blue;
    unsigned char green;
    unsigned char red;
} Pixel;
#pragma pack(4)

#pragma pack(1)
typedef struct {
    BMPFileHeader bmpHeader;
    DIBHeader dibHeader;
    Pixel *pixels;
} BMPImage;
#pragma pack(4)

BMPImage* readAndValidateBMP(const char *filename) {
    FILE *file = fopen(filename, "rb");
    if (file == NULL) {
        printf("Error: File not found.\n");
        return NULL;
    }
    BMPImage *image  = (BMPImage*)malloc(sizeof(BMPImage));
    if (image == NULL) {
        printf("Error: Could not allocate enough memory for image.\n");
        fclose(file);
        return NULL;
    }
    fread(&image->bmpHeader, sizeof(BMPFileHeader), 1, file);
    
    if (image->bmpHeader.format[0] != 'B' || image->bmpHeader.format[1] != 'M') {
        printf("Error: The format is not supported(format).\n");
        fclose(file);
        free(image);
        return NULL;
    }
    fread(&image->dibHeader, sizeof(DIBHeader), 1, file);
    
    if (image->dibHeader.headerSize != 40) {
        printf("Error: The format is not supported(headerSize).\n");
        fclose(file);
        free(image);
        return NULL;
    }

    if (image->dibHeader.bitsPerPixel != 24) {
        printf("Error: The format is not supported(bitsPerPixel).\n");
        fclose(file);
        free(image);
        return NULL;
    }

    image->pixels = (Pixel*)malloc(image->dibHeader.width * image->dibHeader.height * sizeof(Pixel));
    if(image->pixels == NULL) {
        printf("Error: Could not allocate enough memory for pixel data.\n");
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

void freeBMPImage(BMPImage *image) {
    free(image->pixels);
    free(image);
}

void printBMPInfo(const BMPImage *image) {
    printf("=== BMP Header ===\n");
    printf("Type: %c %c\n", image->bmpHeader.format[0], image->bmpHeader.format[1]);
    printf("Size: %u\n", image->bmpHeader.fileSize);
    printf("Reserved 1: %u\n", image->bmpHeader.reserve1);
    printf("Reserved 2: %u\n", image->bmpHeader.reserve2);
    printf("Image Offset: %u\n", image->bmpHeader.pixelArrayOffset);
    printf("=== DIB Header ===\n");
    printf("Size: %u\n", image->dibHeader.headerSize);
    printf("Width: %d\n", image->dibHeader.width);
    printf("Height: %d\n", image->dibHeader.height);
    printf("# color panes: %hu\n", image->dibHeader.colorPanes);
    printf("# bits per pixel: %hu\n", image->dibHeader.bitsPerPixel);
    printf("Compression scheme: %u\n", image->dibHeader.compression);
    printf("Image size: %u\n", image->dibHeader.imageSize);
    printf("Horizontal resolution: %d\n", image->dibHeader.xResolution);
    printf("Verticle resolution: %d\n", image->dibHeader.yResolution);
    printf("# colors in palette: %u\n", image->dibHeader.colorsInPallette);
    printf("# important colors: %u\n", image->dibHeader.importantColors);
}

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

void writeBMPImage(const char *filename, BMPImage *image) {
    FILE *file = fopen(filename, "wb");
    if(file == NULL) {
        printf("Error: Could not open file for writing");
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

BMPImage* hideBMPImage(BMPImage *image, BMPImage *imageToHide) {
    if(image->dibHeader.width != imageToHide->dibHeader.width || image->dibHeader.height != imageToHide->dibHeader.height) { 
        printf("Error: Images must have the same dimension");
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

int main(int argc, char *argv[]) {
    if (argc < 3) {
        printf("Error: Missing Arguments.\n");
        return 1;
    }
    if (strcmp(argv[1], "--info") == 0) {
        BMPImage *image = readAndValidateBMP(argv[2]);
        if(image != NULL){
            printBMPInfo(image);
            freeBMPImage(image);
        }
    } else if (strcmp(argv[1], "--reveal") == 0) {
        BMPImage *image = readAndValidateBMP(argv[2]);
        if(image != NULL){
            image = revealHiddenImage(image);
            writeBMPImage(argv[2], image);
            freeBMPImage(image);
        }
    } else if (strcmp(argv[1], "--hide") == 0) {
        if (argc < 4) {
            printf("Error: Missing Arguments");
            return 1;
        }
        BMPImage *image = readAndValidateBMP(argv[2]);
        BMPImage *imageToHide = readAndValidateBMP(argv[3]);

        if (image != NULL && imageToHide != NULL) {
            image = hideBMPImage(image, imageToHide);
            writeBMPImage(argv[2], image);
            freeBMPImage(image);
            freeBMPImage(imageToHide);
        }
    }
    return 0;
}
