#include <stdbool.h>
#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "wav.h"

int check_format(WAVHEADER header);
int get_block_size(WAVHEADER header);

int main(int argc, char *argv[])
{
    // Ensure proper usage
    if(argc != 3){
        printf("Usage: $ ./reverse input.wav output.wav\n");
        return 1;
    }

    // Open input file for reading
    FILE *fptr;
    fptr = fopen(argv[1], "r");
    if(fptr == NULL){
        printf("File does not exist\n");
        return 1; 
    }

    // Read header
    WAVHEADER header;

    // https://cplusplus.com/reference/cstdio/fread/
    fread(header.chunkID,        sizeof(BYTE),  4, fptr);
    fread(&header.chunkSize,     sizeof(DWORD), 1, fptr);
    fread(header.format,         sizeof(BYTE),  4, fptr);    
    fread(header.subchunk1ID,    sizeof(BYTE),  4, fptr);
    fread(&header.subchunk1Size, sizeof(DWORD), 1, fptr);
    fread(&header.audioFormat,   sizeof(WORD),  1, fptr);
    fread(&header.numChannels,   sizeof(WORD),  1, fptr);
    fread(&header.sampleRate,    sizeof(DWORD), 1, fptr);
    fread(&header.byteRate,      sizeof(DWORD), 1, fptr);    
    fread(&header.blockAlign,    sizeof(WORD),  1, fptr);
    fread(&header.bitsPerSample, sizeof(WORD),  1, fptr);
    fread(header.subchunk2ID,    sizeof(BYTE),  4, fptr);
    fread(&header.subchunk2Size, sizeof(DWORD), 1, fptr);


    // Use check_format to ensure WAV format
    _Bool bIsValid = check_format(header);
    if(!bIsValid){
        printf("Input file is not a valid WAV file\n");
        return 1;
    }

    // Open output file for writing
    FILE* pOut;
    pOut = fopen(argv[2], "w");
    if(pOut == NULL){
        printf("Unable to create/open output file called %s\n", argv[2]);
        return 1;
    }

    // Write header to file
    // https://www.tutorialspoint.com/c_standard_library/c_function_fwrite.htm
    fwrite(header.chunkID,        sizeof(BYTE),  4, pOut);
    fwrite(&header.chunkSize,     sizeof(DWORD), 1, pOut);
    fwrite(header.format,         sizeof(BYTE),  4, pOut);    
    fwrite(header.subchunk1ID,    sizeof(BYTE),  4, pOut);
    fwrite(&header.subchunk1Size, sizeof(DWORD), 1, pOut);
    fwrite(&header.audioFormat,   sizeof(WORD),  1, pOut);
    fwrite(&header.numChannels,   sizeof(WORD),  1, pOut);
    fwrite(&header.sampleRate,    sizeof(DWORD), 1, pOut);
    fwrite(&header.byteRate,      sizeof(DWORD), 1, pOut);    
    fwrite(&header.blockAlign,    sizeof(WORD),  1, pOut);
    fwrite(&header.bitsPerSample, sizeof(WORD),  1, pOut);
    fwrite(header.subchunk2ID,    sizeof(BYTE),  4, pOut);
    fwrite(&header.subchunk2Size, sizeof(DWORD), 1, pOut);

    // Use get_block_size to calculate size of block
    int iBlockSize = get_block_size(header);
    
    // Write reversed audio to file
    uint8_t buffer[iBlockSize];
    const FILE* pEndHeader = fptr + ftell(fptr);

    fseek(fptr, -iBlockSize, SEEK_END);
    while((fptr + ftell(fptr)) >= pEndHeader){
        fread(buffer, iBlockSize, 1, fptr);
        fwrite(buffer, iBlockSize, 1, pOut);
        fseek(fptr, -2 * iBlockSize, SEEK_CUR);
    }

    fclose(fptr);
    fclose(pOut);

    return 0;
}

int check_format(WAVHEADER header)
{
    if(strcmp(header.format, "WAVE")) return true;
    return false;
}

int get_block_size(WAVHEADER header)
{
    int iSize = header.numChannels * header.bitsPerSample / 8;
    return iSize;
}