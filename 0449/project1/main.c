#include <stdio.h>
#include <string.h>
#include "bmp.h"

// This is the main function, controlling what happens given the user input into
// argv 
int main(int argc, char *argv[]) {
    if (argc < 3) {
        printf("ERROR: Missing arguments.\n");
        return 1;
    }
    if (strcmp(argv[1], "--info") == 0) {
        BMPImage *image = readAndValidate(argv[2]);
        if(image != NULL){
            printInfo(image);
            freeImage(image);
        }
    } else if (strcmp(argv[1], "--reveal") == 0) {
        BMPImage *image = readAndValidate(argv[2]);
        if(image != NULL){
            image = revealHiddenImage(image);
            writeImage(argv[2], image);
            freeImage(image);
        }
    } else if (strcmp(argv[1], "--hide") == 0) {
        if (argc < 4) {
            printf("ERROR: Missing arguments.\n");
            return 1;
        }
        BMPImage *image = readAndValidate(argv[2]);
        if(image == NULL)
            return 1;
        BMPImage *imageToHide = readAndValidate(argv[3]);

        if (imageToHide != NULL) {
            image = hideImage(image, imageToHide);
            writeImage(argv[2], image);
            freeImage(image);
            freeImage(imageToHide);
        }
    }
    return 0;
}
