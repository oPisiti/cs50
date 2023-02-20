// #include "cs50.c"
#include <cs50.h>
#include <stdio.h>

int main(void) 
{
    int height = -1;
    while (height < 1 || height > 8){
        height = get_int("Height: ");    
    }

    int spaces;
    for(int i = 1; i <= height; i++){
        spaces =  height - i;

        for(int s = 0; s < spaces; s++) printf(" ");
        for(int h = 0; h < i; h++)     printf("#");
        printf("  ");
        for(int h = 0; h < i; h++)     printf("#");
        
        if(i != height)                 printf("\n");
    }
}