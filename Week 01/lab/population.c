// #include "cs50.c"
#include <cs50.h>
#include <stdio.h>
#include <limits.h>

int main(void)
{
    // TODO: Prompt for start size
    int min_init_population = 9;
    
    int init_population = INT_MIN;
    while (init_population < min_init_population){
        init_population = get_int("Initial Population size: ");
    }

    // TODO: Prompt for end size    
    int end_population = INT_MIN;
    while (end_population < init_population){
        end_population = get_int("End Population size: ");
    }

    // TODO: Calculate number of years until we reach threshold
    int years = 0;
    int curr_population = init_population;
    while (curr_population < end_population){
        curr_population += (curr_population/3) - (curr_population/4);
        years++;
    }

    // TODO: Print number of years
    printf("Years: %i", years);

}