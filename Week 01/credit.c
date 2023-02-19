// #include "cs50.c"
#include <cs50.h>
#include <stdio.h>
#include <math.h>

int get_num_digits(long long n);
int get_sum_digits(int dig);
int get_first_two_digits(long long n, int num_digits);

int main(void) {
    long long number = -1;
    while(number < 0){
        number = get_long_long("Number: ");
    }

    int num_digits = get_num_digits(number);

    int even_digits[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    int odd_sum = 0;

    // Getting the digits in order
    long long mod = 10;
    int digit;
    for(int i = num_digits-1; i >= 0; i--){
        digit = (number%mod)/(mod/10);

        // Even index from the end
        if((num_digits-i) & 1)  odd_sum += digit;
        
        // Odd index from the end
        else                    even_digits[(num_digits-i+1)/2] = digit;

        mod *= 10;
    }

    // Adding the Evens * 2 and odd_sum
    int full_sum = odd_sum;
    int double_digit;
    for(int i = 0; i < sizeof(even_digits)/sizeof(even_digits[0]); i++){
        double_digit = even_digits[i] * 2;
        full_sum += get_sum_digits(double_digit);
    }

    int first_two_digits = get_first_two_digits(number, num_digits);

    int last_digit = full_sum%10;
    if(last_digit){
        printf("INVALID\n");
        return 0;
    }

    if(num_digits == 13){
        if(first_two_digits/10 == 4){
            printf("VISA\n");
            return 0;
        }
    }
    else if(num_digits == 15){
        if(first_two_digits == 34 || first_two_digits == 37){
            printf("AMEX\n");
            return 0;
        }
    }
    else if(num_digits == 16){
        if(first_two_digits >= 51 && first_two_digits <= 55){
            printf("MASTERCARD\n");
            return 0;
        }
        else if(first_two_digits/10 == 4){
            printf("VISA\n");
            return 0;
        }
    }

    printf("INVALID\n");
    return 0;

}

int get_num_digits(long long n){
    int count = 0;
    
    while(n){
        n /= 10;
        count++;
    }

    return count;
}

int get_sum_digits(int dig){
    return dig/10 + dig%10;
}

int get_first_two_digits(long long n, int num_digits){
    // printf("N:   %llu\n", n);
    // printf("POW: %llu\n", (long long)pow(10, num_digits-1));
    return n/(long long)pow(10, num_digits-2);
}