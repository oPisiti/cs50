#include <cs50.h>
#include <ctype.h>
#include <string.h>
#include <stdio.h>

string string2lower(string str);
char findCipherLetterLower(string sKey, char cCurrent);
bool isAllAlphabetic(string sKey);
bool repeatedCharInKey(string sKey);

int main(int argc, string argv[]){
    // --- Dealing with errors ---
    // Args
    if(argc != 2){
        printf("Usage: ./substitution key\n");
        return 1;
    }

    string sKey = argv[1];
    sKey = string2lower(sKey);

    // Length
    if(strlen(sKey) != 26){
        printf("Key must contain 26 characters.\n");
        return 1;
    }

    // Non alphabetical char in key
    if(!isAllAlphabetic(sKey)){
        printf("Key must contain only alphanumeric chars\n");
        return 1;
    }

    // Repeated char in key
    if(repeatedCharInKey(sKey)){
        printf("Key must not contain repeated chars\n");
        return 1;
    }


    // --- Actual Code
    // Getting Plain text
    string sPlainText = get_string("plaintext: ");
    const int iPlainTextLen = strlen(sPlainText);

    // Substitution
    string sCipherText = sPlainText;
    char cCurrent;
    char cCipherDelta;
    for(int i = 0; i < iPlainTextLen; i++){
        cCurrent = sPlainText[i];        
        cCipherDelta = findCipherLetterLower(sKey, cCurrent);

        // Is upper case letter
        if(cCurrent >= 65 && cCurrent <= 90){
            cCurrent = -32 + cCipherDelta;
        }
        // Is lower case letter
        else if(cCurrent >= 97 && cCurrent <= 122){
            cCurrent = cCipherDelta;
        }

        sCipherText[i] = cCurrent;
    }

    // Showing the Cipher text
    printf("ciphertext: %s\n", sCipherText);

}

string string2lower(string str){
    const int iStrLen = strlen(str);
    string out = str;

    for(int i = 0; i < iStrLen; i++){
        out[i] = tolower(str[i]);
    }

    return out;
}

char findCipherLetterLower(string sKey, char cCurrent){
    return sKey[tolower(cCurrent) - 97];
}

bool isAllAlphabetic(string sKey){
    char curr;
    for(int i = 0; i < strlen(sKey); i++){
        curr = sKey[i];
        if(curr < 65 || (curr > 90 && curr < 97) || curr > 122){
            return false;
        }
    }

    return true;
}

bool repeatedCharInKey(string sKey){
    const int iAlphabeticSize = 26;
    bool aContainsLetter[iAlphabeticSize];
    int iCurrIndex;
    const string lowerKeys = string2lower(sKey);

    for(int i = 0; i < iAlphabeticSize; i++) aContainsLetter[i] = false;

    for(int i = 0; i < iAlphabeticSize; i++){
        iCurrIndex = lowerKeys[i] - 97;
        if(aContainsLetter[iCurrIndex]) return true;
        
        aContainsLetter[iCurrIndex] = true;
    }

    return false;
}