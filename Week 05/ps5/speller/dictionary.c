// Implements a dictionary's functionality

#include <ctype.h>
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

// TODO: Choose number of buckets in hash table
const unsigned int N = 26;

// Pointer to loaded dictionary
FILE *fDictPtr = NULL;

// Hash table
node *table[N];
bool bTableInit = false;

// Returns true if word is in dictionary, else false
bool check(const char *word)
{
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
    if(fDictPtr == NULL) return 1;

    // Making sure every item in the table array points to NULL
    for(int i = 0; i < N; i++){
        table[i] = NULL;
    }
    
    fseek(fDictPtr, 0, SEEK_SET);
    int index = 0;
    char word[LENGTH + 1];
    char c;
    while (fread(&c, sizeof(char), 1, fDictPtr)){
        // Allow only alphabetical characters and apostrophes
        if (isalpha(c) || (c == '\'' && index > 0)){
            // Append character to word
            word[index] = c;
            index++;

            // Ignore alphabetical strings too long to be words
            if (index > LENGTH)
            {
                // Consume remainder of alphabetical string
                while (fread(&c, sizeof(char), 1, fDictPtr) && isalpha(c));

                // Prepare for new word
                index = 0;
            }
        }

        // Ignore words with numbers (like MS Word can)
        else if (isdigit(c)){
            // Consume remainder of alphanumeric string
            while (fread(&c, sizeof(char), 1, fDictPtr) && isalnum(c));

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
    }

    return 0;
}

// Adds a word to a has table in the form of a node
void add_word(const char* word){
    // Getting index
    int iIndex = hash(word)%N;

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
unsigned int hash(const char *word)
{
    if(word[0] <= 90)   return word[0] - 65;
    else                return word[0] - 97;
}

// Loads dictionary into memory, returning true if successful, else false
bool load(const char *dictionary)
{
    // TODO
    fDictPtr = fopen(dictionary, "r");
    if(fDictPtr == NULL) return false;
 
    return true;
}

// Returns number of words in dictionary if loaded, else 0 if not yet loaded
unsigned int size(void)
{
    // TODO
    if(fDictPtr == NULL) return 0;

    fseek(fDictPtr, 0, SEEK_SET);
    int iTotalSize = 0, iWordSize = 0;
    char cBuffer;
    while(fread(&cBuffer, sizeof(char), 1, fDictPtr)){
        if(isalpha(cBuffer) || cBuffer == '\''){
            iWordSize++;
            continue;
        }

        if(iWordSize){
            iTotalSize++;
            iWordSize = 0;
        }
    }

    return iTotalSize;
}

// Unloads dictionary from memory, returning true if successful, else false
bool unload(void)
{
    // TODO
    int closed = fclose(fDictPtr);
    
    if(closed == 0) return true;
    return false;
}
