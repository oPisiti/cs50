#include <cs50.h>
#include <stdio.h>
#include <string.h>

// Max number of candidates
#define MAX 9

// preferences[i][j] is number of voters who prefer i over j
int preferences[MAX][MAX];

// locked[i][j] means i is locked in over j
bool locked[MAX][MAX];

// Each pair has a winner, loser
typedef struct
{
    int winner;
    int loser;
}
pair;

// Array of candidates
string candidates[MAX];
pair pairs[MAX * (MAX - 1) / 2];

int pair_count;
int candidate_count;

int visited[MAX*MAX];
int visited_count;

// Function prototypes
bool vote(int rank, string name, int ranks[]);
void record_preferences(int ranks[]);
void add_pairs(void);
void sort_pairs(void);
void lock_pairs();
void print_winner();
bool have_visited(int iWinnerNum, int visited[], int visited_count);
void add_visited(int iWinnerNum, int iLoserNum, int visited[], int visited_count);

int main(int argc, string argv[])
{
    // Check for invalid usage
    if (argc < 2)
    {
        printf("Usage: tideman [candidate ...]\n");
        return 1;
    }

    // Populate array of candidates
    candidate_count = argc - 1;
    if (candidate_count > MAX)
    {
        printf("Maximum number of candidates is %i\n", MAX);
        return 2;
    }
    for (int i = 0; i < candidate_count; i++)
    {
        candidates[i] = argv[i + 1];
    }

    // Clear graph of locked in pairs
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = 0; j < candidate_count; j++)
        {
            locked[i][j] = false;
        }
    }

    pair_count = 0;
    int voter_count = get_int("Number of voters: ");

    // Query for votes
    for (int i = 0; i < voter_count; i++)
    {
        // ranks[i] is voter's ith preference
        int ranks[candidate_count];

        // Query for each rank
        for (int j = 0; j < candidate_count; j++)
        {
            string name = get_string("Rank %i: ", j + 1);

            if (!vote(j, name, ranks))
            {
                printf("Invalid vote.\n");
                return 3;
            }
        }


        record_preferences(ranks);

        printf("\n");
    }

    add_pairs();
    sort_pairs();
    lock_pairs();
    print_winner();
    return 0;
}

// Update ranks given a new vote
bool vote(int rank, string name, int ranks[])
{
    int iCandidateIndex;

    // Checking if name is valid
    for(int i = 0; i < candidate_count; i++){
        if(strcmp(candidates[i], name) == 0){
            iCandidateIndex = i;

            // Updating ranks
            ranks[rank] = iCandidateIndex;
            return true;
        }
    }

    return false;    
}

// Update preferences given one voter's ranks
void record_preferences(int ranks[])
{
    for(int i = 0; i < candidate_count; i++){
        for(int j = i+1; j < candidate_count; j++){
            preferences[ranks[i]][ranks[j]]++;
        }
    }

    return;
}

// Record pairs of candidates where one is preferred over the other
void add_pairs(void)
{
    for(int i = 0; i < MAX; i++){
        for(int j = i; j < MAX; j++){
            if(preferences[i][j] < preferences[j][i]){
                pair new_pair = {j, i};
                pairs[pair_count] = new_pair;
                pair_count++;
            }
            else if(preferences[i][j] > preferences[j][i]){
                pair new_pair = {i, j};
                pairs[pair_count] = new_pair;
                pair_count++;
            }
        }
    }

    return;
}

// Sort pairs in decreasing order by strength of victory
void sort_pairs(void)
{
    int iIndexMaxItem;
    int iMaxItem;

    for(int i = 0; i < pair_count; i++){
        // Finding the remaining max item
        iIndexMaxItem = i;
        iMaxItem = pairs[i].winner;
        for(int p = i+1; p < pair_count; p++){
            if(pairs[p].winner > iMaxItem){
                iIndexMaxItem = p;
                iMaxItem = pairs[p].winner;            
            }
        }

        // Switching the position of the max item
        if(iMaxItem > pairs[i].winner){
            pair aux = pairs[i];
            pairs[i] = pairs[iIndexMaxItem];
            pairs[iIndexMaxItem] = aux;
        }
    }

    // printf("Sorted: ");
    // for(int i = 0; i < pair_count; i++){
    //     printf("%i ", pairs[i].winner);
    // }

    return;
}

// Lock pairs into the candidate graph in order, without creating cycles
void lock_pairs()
{
    // All the visited nodes 
    // int visited[pair_count];
    visited_count = 0;

    for(int i = 0; i < pair_count; i++){
        if(!have_visited(pairs[i].winner, visited, visited_count)){
            locked[pairs[i].winner][pairs[i].loser] = true;
            add_visited(pairs[i].winner, pairs[i].loser, visited, visited_count);
            visited_count++;
        }
    }
    
    return;
}

// Checks if a node has been visited
bool have_visited(int iWinnerNum, int visited[], int visited_count){
    for(int i = 0; i < visited_count; i++){
        if(iWinnerNum == visited[i]){
            return true;
        }
    }

    return false;
}

// Adds a node to a visited array
void add_visited(int iWinnerNum, int iLoserNum, int visited[], int visited_count){
    // Visiting a node that points to the first node
    if(visited[0] == iLoserNum){
        // Shifting left by one
        int aux1;
        int aux2;
        for(int i = 1; i < visited_count; i++){
            aux1 = visited[i-1];
            aux2 = visited[i];
            visited[i] = aux1;
            aux1 = aux2;
        }

        // Adding the winner's ID to the first element of array
        visited[0] = iWinnerNum;
        return;
    }

    // Visiting a node that must have been pointer at by the last node
    visited[visited_count] = iWinnerNum;
}

// Print the winner of the election
void print_winner()
{
    printf("%s\n", candidates[visited[0]]);

    return;
}