#include <stdio.h>
#include <stdlib.h>

#define VALID_SIG 0x4D42 // "BM"
#define DIB_OFFSET 14

typedef unsigned char byte;

typedef short i16;
typedef unsigned short u16;
typedef int i32;
typedef unsigned int u32;
typedef long long i64;
typedef unsigned long long u64;

#pragma pack(push, 1)

typedef struct FileHeader_s {
    u16 signature;
    u32 fileSize;
    u16 reserved1;
    u16 reserved2;
    u32 pixelArrayOffset;
} FileHeader;

typedef struct BITMAPINFOHEADER_s {
    u32 dibHeaderSize;
    i32 imageWidth;
    i32 imageHeight;
    u16 numColorPlanes;
    u16 bitsPerPixel;
    u32 compressionMethod;
    u32 totalImageSize;
    i32 horizontalResolution;
    i32 verticalResolution;
    u32 numColorsInPalette;
    u32 numImportantColors;
    
} BITMAPINFOHEADER;

#pragma pack(pop)

byte *readFileToBuffer(char *filePath) {
    byte *result = NULL;
    FILE *file = fopen(filePath, "r");
    u32 fileSize = 0;
    
    if(file) {
        fseek(file, 0, SEEK_END);
        fileSize = ftell(file);
        fseek(file, 0, SEEK_SET);
        
        result = malloc(fileSize);
        if(result) {
            u32 bytesRead = fread(result, 1, fileSize, file);
            if(bytesRead != fileSize) {
                //Error reading file (fread didnt read the entire file)
                perror(filePath);
                free(result);
                result = NULL;
            }
        }
    }
    else {
        // Problem creating file struct
        perror(filePath);
    }
    
    if(file) {
        fclose(file);
    }
    
    return result;
}

int main(int argc, char **argv) {
    (void) argc;
    (void) argv;
    
    printf("Welcome back my friends to the show that never ends\n");
    
    byte *fileBuffer = readFileToBuffer("test.bmp");
    FileHeader *fHeader = (FileHeader *) fileBuffer;
    BITMAPINFOHEADER *dibHeader = (BITMAPINFOHEADER *) (fileBuffer + DIB_OFFSET);
    
    if(fHeader->signature == VALID_SIG) {
        printf("Valid File Sig: true\n");
    }
    else {
        printf("Valid File Sig: false\n");
    }
    
    printf("File Size: %u\n", fHeader->fileSize);
    printf("Offset To Pixel Data: %u\n", fHeader->pixelArrayOffset);
    
    printf("\n");
    
    printf("DIB Header Size: %u\n", dibHeader->dibHeaderSize);
    printf("Bitmap Width: %d\n", dibHeader->imageWidth);
    printf("Bitmap Height: %d\n", dibHeader->imageHeight);
    printf("Number of Color Planes: %hu\n", dibHeader->numColorPlanes);
    printf("Color Depth: %hu\n", dibHeader->bitsPerPixel);
    printf("Compression Method: %u\n", dibHeader->compressionMethod);
    printf("Total Image Size: %u\n", dibHeader->totalImageSize);
    printf("Horizontal Resolution: %u\n", dibHeader->horizontalResolution);
    printf("Vertical Resolution: %u\n", dibHeader->verticalResolution);
    printf("Number of Colors in Palette: %u\n", dibHeader->numColorsInPalette);
    printf("Number of Important Colors: %u\n", dibHeader->numImportantColors);
    
    free(fileBuffer);
    
    return 0;
}