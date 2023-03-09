// Implements a dictionary's functionality

#include <ctype.h>
#include <math.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <strings.h>

#include "dictionary.h"

// Functions declarations
int create_table();
void add_word(const char* word);

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
}
node;

// Choose number of buckets in hash table
unsigned int N;

// Loaded dictionary
char* sDictionaryWords;
bool bDictLoaded = false;
long lDictSize;

// Hash table
const float fLoadFactor = 0.75f;
node** table;
bool bTableInit = false;
uint8_t iNumCharsToHash = 3;

// Returns true if word is in dictionary, else false
bool check(const char *word){

    // Making sure the has table has been initialized
    if(!bTableInit){
        create_table();
        bTableInit = true;
    }

    // Searching the hash table for word
    int iTableIndex = hash(word);
    node* pCurrNode = table[iTableIndex];
    while(pCurrNode != NULL){
        if(strcasecmp(pCurrNode->word, word) == 0) return true;
        pCurrNode = pCurrNode->next;
    }
 
    return false;
}

int create_table(){
    if(!bDictLoaded) return 1;

    // Declaring table array (pointers to the first node of linked list)
    N = round(pow(26, iNumCharsToHash));
    table = malloc(N * sizeof(node*));

    // Making sure every item in the table array points to NULL
    for(int i = 0; i < N; i++){
        table[i] = NULL;
    }
    
    long lDictIndex = 0;
    int index = 0;
    char word[LENGTH + 1];
    char c;
    while (sDictionaryWords[lDictIndex] != '\0'){
        c = sDictionaryWords[lDictIndex];

        // Allow only alphabetical characters and apostrophes
        if (isalpha(c) || (c == '\'' && index > 0)){
            // Append character to word
            word[index] = c;
            index++;

            // Ignore alphabetical strings too long to be words
            if (index > LENGTH)
            {
                // Consume remainder of alphabetical string
                while(isalpha(sDictionaryWords[lDictIndex])) lDictIndex++;

                // Prepare for new word
                index = 0;
            }
        }

        // Ignore words with numbers (like MS Word can)
        else if (isdigit(c)){
            // Consume remainder of alphanumeric string
            while(isalnum(sDictionaryWords[lDictIndex])) lDictIndex++;

            // Prepare for new word
            index = 0;
        }

        // We must have found a whole word
        else if (index > 0){
            // Terminate current word
            word[index] = '\0';

            add_word(word);

            // Prepare for next word
            index = 0;
        }

        lDictIndex++;
    }

    return 0;
}

// Adds a word to a has table in the form of a node
void add_word(const char* word){
    // Getting index
    int iIndex = hash(word);

    // Creating a new node to hold the word
    node* pNewNode = malloc(sizeof(node));    
    int iLetterIndex = 0;
    while(word[iLetterIndex] != '\0'){
        pNewNode->word[iLetterIndex] = word[iLetterIndex];
        iLetterIndex++;
    }
    pNewNode->word[iLetterIndex] = '\0';

    // Switcheroo
    pNewNode->next = table[iIndex];
    table[iIndex] = pNewNode;
}

// Hashes word to a number
unsigned int hash(const char *word){
    uint8_t res = 0;
    for(int i = 0; i < iNumCharsToHash; i++){
        if(word[i] == '\0') break;
        res += tolower(word[i]);
    }
    while(res >= N) res -= N;

    return res;
}

// Loads dictionary into memory, returning true if successful, else false
bool load(const char *dictionary){

    FILE* fptr = fopen(dictionary, "r");
    fseek(fptr, 0, SEEK_END);
    lDictSize = ftell(fptr);
    rewind(fptr);

    sDictionaryWords = malloc(lDictSize + 1);
    if(sDictionaryWords == NULL) return false;
    bDictLoaded = true;

    fread(sDictionaryWords, lDictSize, 1, fptr);
    sDictionaryWords[lDictSize] = '\0';
 
    fclose(fptr);

    return true;
}

// Returns number of words in dictionary if loaded, else 0 if not yet loaded
unsigned int size(void){

    if(!bDictLoaded) return 0;

    long lFileCharIndex = 0;
    int iTotalSize = 0;
    while(sDictionaryWords[lFileCharIndex] != '\0'){
        if(sDictionaryWords[lFileCharIndex]== '\n') iTotalSize++;

        lFileCharIndex++;
    }

    return iTotalSize;
}

// Unloads dictionary from memory, returning true if successful, else false
bool unload(void){

    // Closing file
    free(sDictionaryWords);
    
    // Freeing the hash table
    node* pCurrNode;
    node* pPrevNode;
    for(int i = 0; i < N; i++){
        pCurrNode = table[i];
        while(pCurrNode != NULL){
            pPrevNode = pCurrNode;
            pCurrNode = pCurrNode->next;
            free(pPrevNode);
        }
    }
    free(table);

    return true;
}
