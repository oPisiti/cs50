#include <cs50.h>
#include <math.h>
#include <stdio.h>

int main(void){
    char* sText = get_string("Text: ");
    
    // Counting
    char cCurr_char = *sText;
    int iSumLetters = 0, iSumWords = 0, iSumSentences = 0;
    int iWordFirstIndex = 0, iWordLastIndex = 0;
    while(cCurr_char != '\0'){

        // Counting letters
        if((cCurr_char >= 65 && cCurr_char <= 90) || (cCurr_char >= 97 && cCurr_char <= 122))
            iSumLetters++;

        // Counting words
        if(iWordFirstIndex != iWordLastIndex && 
           (cCurr_char == ' ' || cCurr_char == '.' || cCurr_char == '!' | cCurr_char == '?')){
            iSumWords++;   
            iWordFirstIndex = iWordLastIndex + 1;         
        }

        // Counting sentences
        if(cCurr_char == '.' || cCurr_char == '!' | cCurr_char == '?')
            iSumSentences++;

        cCurr_char = *(sText + iWordLastIndex + 1);
        iWordLastIndex++;
    }

    // Calculation index
    float L = 100 * (float)iSumLetters / iSumWords;
    float S = 100 * (float)iSumSentences / iSumWords;

    float fIndex = 0.0588 * L - 0.296 * S - 15.8;
    int iPartialIndex = (int)fIndex;
    float fDecimalIndex = fIndex - iPartialIndex;
    int iIndex = fDecimalIndex < 0.5 ? iPartialIndex : iPartialIndex + 1;

    // Outputting the grade
    if(iIndex < 1)        printf("Before Grade 1\n");
    else if(iIndex >= 16) printf("Grade 16+\n");
    else                  printf("Grade %i\n", iIndex);
}