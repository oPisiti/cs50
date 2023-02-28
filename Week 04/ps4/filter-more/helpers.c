#include "helpers.h"
#include <math.h>
#include <stdio.h>

void apply_convolution(int height, int width, RGBTRIPLE image[height][width], int i, int j, const int KERNEL_SIZE, const int Kx[KERNEL_SIZE][KERNEL_SIZE], const int Ky[KERNEL_SIZE][KERNEL_SIZE], RGBTRIPLE output[height][width]);

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    BYTE bAverage;
    for(int i = 0; i < height; i++){
        for(int j = 0; j < width; j++){
            bAverage = round(((float)image[i][j].rgbtRed + (float)image[i][j].rgbtGreen + (float)image[i][j].rgbtBlue)/3);

            image[i][j].rgbtRed   = bAverage;
            image[i][j].rgbtGreen = bAverage;
            image[i][j].rgbtBlue  = bAverage;
        }
    }

    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE tAux;
    for(int i = 0; i < height; i++){
        for(int j = 0; j < width/2; j++){
            tAux = image[i][j];
            image[i][j] = image[i][width - 1 - j];
            image[i][width - 1 - j] = tAux;
        }
    }

    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE tAux[height][width];
    for(int i = 0; i < height; i++){
        for(int j = 0; j < width; j++){
            tAux[i][j] = image[i][j];
        }
    }

    float fSumR, fSumG, fSumB;
    int iNumValidNeighbors;
    int iMinI, iMaxI, iMinJ, iMaxJ;

    for(int i = 0; i < height; i++){
        // Defining min and max i values
        if(i == 0)        iMinI = i;
        else              iMinI = i - 1;

        if(i == height-1) iMaxI = i;
        else              iMaxI = i + 1;

        for(int j = 0; j < width; j++){
            // Defining min and max j values
            if(j == 0)        iMinJ = j;
            else              iMinJ = j - 1;

            if(j == height-1) iMaxJ = j;
            else              iMaxJ = j + 1;

            fSumR = 0;
            fSumG = 0;
            fSumB = 0;
            
            iNumValidNeighbors = (iMaxI + 1 - iMinI) * (iMaxJ + 1 - iMinJ);

            // Getting Sum of neighboring RGB values
            for(int x = iMinI; x <= iMaxI; x++){
                for(int y = iMinJ; y <= iMaxJ; y++){
                    fSumR += image[x][y].rgbtRed;
                    fSumG += image[x][y].rgbtGreen;
                    fSumB += image[x][y].rgbtBlue;
                }
            }

            // Averaging
            fSumR /= iNumValidNeighbors;
            fSumG /= iNumValidNeighbors;
            fSumB /= iNumValidNeighbors;

            tAux[i][j].rgbtRed   = round(fSumR);
            tAux[i][j].rgbtGreen = round(fSumG);
            tAux[i][j].rgbtBlue  = round(fSumB);            
        }
    }

    for(int i = 0; i < height; i++){
        for(int j = 0; j < width; j++){
            image[i][j] = tAux[i][j];
        }
    }

    return;
}

// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width])
{
    // --- Declarations and definitions ---
    const int KERNEL_SIZE = 3;

    const int KERNEL_GX[KERNEL_SIZE][KERNEL_SIZE] = {
        {-1, 0, 1},
        {-2, 0, 2},
        {-1, 0, 1},
    };
    const int KERNEL_GY[KERNEL_SIZE][KERNEL_SIZE] = {
        {-1, -2, -1},
        { 0,  0,  0},
        { 1,  2,  1},
    };

    RGBTRIPLE tAux[height][width];
    for(int i = 0; i < height; i++){
        for(int j = 0; j < width; j++){
            tAux[i][j] = image[i][j];
        }
    }

    // Applying the kernels to each pixel of image
    for(int i = 0; i < height; i++){
        for(int j = 0; j < width; j++){
            apply_convolution(height, width, image, i, j, KERNEL_SIZE, KERNEL_GX, KERNEL_GY, tAux);
        }
    }

    for(int i = 0; i < height; i++){
        for(int j = 0; j < width; j++){
            image[i][j] = tAux[i][j];
        }
    }

    return;
}

// Applies a kernel to an image and writes into an output matrix
void apply_convolution(int height, int width, RGBTRIPLE image[height][width], int i, int j, const int KERNEL_SIZE, 
                       const int Kx[KERNEL_SIZE][KERNEL_SIZE], const int Ky[KERNEL_SIZE][KERNEL_SIZE], RGBTRIPLE output[height][width]){
                        
    // --- Declarations and definitions ---    
    int fSumRX = 0;
    int fSumGX = 0;
    int fSumBX = 0;
    int fSumRY = 0;
    int fSumGY = 0;
    int fSumBY = 0;

    int iMinI, iMaxI, iMinJ, iMaxJ;

    if(i == 0)          iMinI = i;
    else                iMinI = i - 1;
    if(i == height - 1) iMaxI = height - 1;
    else                iMaxI = i + 1;

    if(j == 0)          iMinJ = j;
    else                iMinJ = j - 1;
    if(j == width - 1)  iMaxJ = width - 1;
    else                iMaxJ = j + 1;

    // Index of the middle item in the kernel
    int iMidPosI = KERNEL_SIZE/2;
    int iMidPosJ = KERNEL_SIZE/2;

    // Current position in the Kernel
    int iCurKernelX, iCurKernelY;

    // Main loop
    for(int x = iMinI; x <= iMaxI; x++){
        iCurKernelX = x - i + iMidPosI;
        
        for(int y = iMinJ; y <= iMaxJ; y++){
            iCurKernelY = y - j + iMidPosJ;

            fSumRX += Kx[iCurKernelX][iCurKernelY] * image[x][y].rgbtRed;
            fSumGX += Kx[iCurKernelX][iCurKernelY] * image[x][y].rgbtGreen;
            fSumBX += Kx[iCurKernelX][iCurKernelY] * image[x][y].rgbtBlue;

            fSumRY += Ky[iCurKernelX][iCurKernelY] * image[x][y].rgbtRed;
            fSumGY += Ky[iCurKernelX][iCurKernelY] * image[x][y].rgbtGreen;
            fSumBY += Ky[iCurKernelX][iCurKernelY] * image[x][y].rgbtBlue;            
        }
    }

    float fRawValueRed   = round(sqrt((float)fSumRX * fSumRX + (float)fSumRY * fSumRY));
    float fRawValueGreen = round(sqrt((float)fSumGX * fSumGX + (float)fSumGY * fSumGY));
    float fRawValueBlue  = round(sqrt((float)fSumBX * fSumBX + (float)fSumBY * fSumBY));

    output[i][j].rgbtRed   = fRawValueRed   > 255 ? 255 : fRawValueRed;
    output[i][j].rgbtGreen = fRawValueGreen > 255 ? 255 : fRawValueGreen;
    output[i][j].rgbtBlue  = fRawValueBlue  > 255 ? 255 : fRawValueBlue;

}