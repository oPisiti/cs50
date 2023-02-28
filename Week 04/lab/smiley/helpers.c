#include "helpers.h"

void colorize(int height, int width, RGBTRIPLE image[height][width])
{
    BYTE bNewR = 0x3f, bNewG = 0x05, bNewB = 0x59;
    BYTE* bR;
    BYTE* bG;
    BYTE* bB;

    for(int i = 0; i < height; i++){
        for(int j = 0; j < width; j++){
            bR = &image[i][j].rgbtRed;
            bG = &image[i][j].rgbtGreen;
            bB = &image[i][j].rgbtBlue;

            if(*bR == 0x0 && *bG == 0x0 && *bB == 0x0){
                *bR = bNewR;
                *bG = bNewG;
                *bB = bNewB;
            }
        }
    }
}
