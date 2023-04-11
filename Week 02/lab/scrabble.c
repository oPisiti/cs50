#include <cs50.h>
#include <ctype.h>
#include <stdio.h>
#include <string.h>

int get_score(string word);

int main(int argc, string argv[]){
    int n_players = 2;
    if (argc >= 3) n_players = argc-1;
    
    string words[n_players];

    // Getting the words
    if (argc >= 3){
        for(int i = 1; i < argc; i++){
            words[i-1] = argv[i];
        }
    }
    else{           
        for(int i = 0; i < n_players; i++){
            words[i] = get_string("Player %i: ", i+1);
        }
    }

    // Calculating the scores for the words
    int scores[n_players];
    for(int i = 0; i < n_players; i++){
        scores[i] = get_score(words[i]);
    }

    // Defining the winner
    int iFinalScore  = 0;
    int iWinnerIndex = 0;
    bool bAllEqual   = false;
    for(int i = 0; i < n_players; i++){
        if(scores[i] > iFinalScore){
            iFinalScore  = scores[i];
            iWinnerIndex = i;
            bAllEqual = false;
        }
        else if(scores[i] == iFinalScore){
            bAllEqual = true;
        }
    }

    if(bAllEqual){
        printf("Tie!\n");
        return 0;
    }

    printf("Player %i wins!\n", iWinnerIndex + 1);

}

int get_score(string word){
    // Returns the score for a string given the scrabble points system

    int score = 0;
    char curr;

    for(int i = 0; i < strlen(word); i++){
        curr = tolower(word[i]);

        if(curr == 'a' || curr == 'e' || curr == 'i' || curr == 'l' || curr == 'n' || curr == 'o' || curr == 'r' || curr == 's' || curr == 't' || curr == 'u'){
            score += 1;
        }
        else if(curr == 'd' || curr == 'g'){
            score += 2;
        }
        else if(curr == 'b' || curr == 'c' || curr == 'm' || curr == 'p'){
            score += 3;
        }
        else if(curr == 'f' || curr == 'h' || curr == 'v' || curr == 'w' || curr == 'y'){
            score += 4;
        }
        else if(curr == 'k'){
            score += 5;
        }
        else if(curr == 'j' || curr == 'x'){
            score += 8;
        }
        else if(curr == 'q' || curr == 'z'){
            score += 10;
        }
    }

    return score;
}