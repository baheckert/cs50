#include <cs50.h>
#include <stdio.h>
#include <string.h>

// Max voters and candidates
#define MAX_VOTERS 100
#define MAX_CANDIDATES 9

// preferences[i][j] is jth preference for voter i
int preferences[MAX_VOTERS][MAX_CANDIDATES];

// Candidates have name, vote count, eliminated status
typedef struct
{
    string name;
    int votes;
    bool eliminated;
}
candidate;

// Array of candidates
candidate candidates[MAX_CANDIDATES];

// Numbers of voters and candidates
int voter_count;
int candidate_count;

// Function prototypes
bool vote(int voter, int rank, string name);
void tabulate(void);
bool print_winner(void);
int find_min(void);
bool is_tie(int min);
void eliminate(int min);

int main(int argc, string argv[])
{
    // Check for invalid usage
    if (argc < 2)
    {
        printf("Usage: runoff [candidate ...]\n");
        return 1;
    }

    // Populate array of candidates
    candidate_count = argc - 1;
    if (candidate_count > MAX_CANDIDATES)
    {
        printf("Maximum number of candidates is %i\n", MAX_CANDIDATES);
        return 2;
    }
    for (int i = 0; i < candidate_count; i++)
    {
        candidates[i].name = argv[i + 1];
        candidates[i].votes = 0;
        candidates[i].eliminated = false;
    }

    voter_count = get_int("Number of voters: ");
    if (voter_count > MAX_VOTERS)
    {
        printf("Maximum number of voters is %i\n", MAX_VOTERS);
        return 3;
    }

    // Keep querying for votes
    for (int i = 0; i < voter_count; i++)
    {

        // Query for each rank
        for (int j = 0; j < candidate_count; j++)
        {
            string name = get_string("Rank %i: ", j + 1);

            // Record vote, unless it's invalid
            if (!vote(i, j, name))
            {
                printf("Invalid vote.\n");
                return 4;
            }
        }

        printf("\n");
    }

    // Keep holding runoffs until winner exists
    while (true)
    {
        // Calculate votes given remaining candidates
        tabulate();

        // Check if election has been won
        bool won = print_winner();
        if (won)
        {
            break;
        }

        // Eliminate last-place candidates
        int min = find_min();
        bool tie = is_tie(min);

        // If tie, everyone wins
        if (tie)
        {
            for (int i = 0; i < candidate_count; i++)
            {
                if (!candidates[i].eliminated)
                {
                    printf("%s\n", candidates[i].name);
                }
            }
            break;
        }

        // Eliminate anyone with minimum number of votes
        eliminate(min);

        // Reset vote counts back to zero
        for (int i = 0; i < candidate_count; i++)
        {
            candidates[i].votes = 0;
        }
    }
    return 0;
}

// Record preference if vote is valid
bool vote(int voter, int rank, string name)
{
    // Iterate through candidates
    for (int i = 0; i < candidate_count; i++)
    {
        // If candidate found
        if (!strcmp(candidates[i].name, name))
        {
            // Add candidate as this voters rank preference
            preferences[voter][rank] = i;
            return true;
        }
    }
    // If no candidate found
    return false;
}

// Takes voter and rank and adds vote from preferences array
void add_vote(int voter, int rank)
{
    int v = preferences[voter][rank];
    if (!candidates[v].eliminated)
    {
        candidates[v].votes++;
        return;
    }
    else
    {
        add_vote(voter, rank + 1);
        return;
    }
}

// Tabulate votes for non-eliminated candidates
void tabulate(void)
{
    // Iterate through voters
    for (int i = 0; i < voter_count; i++)
    {
        // Add vote
        add_vote(i, 0);
    }
    return;
}

// Print the winner of the election, if there is one
bool print_winner(void)
{
    // Iterate through candidate array
    for (int i = 0; i < candidate_count; i++)
    {
        // If candidate has over half of all votes
        if (candidates[i].votes > (float) voter_count / 2)
        {
            // Print winner and return true
            printf("%s\n", candidates[i].name);
            return true;
        }
    }
    // No majority exists
    return false;
}

// Return the minimum number of votes any remaining candidate has
int find_min(void)
{
    // Assume minimum votes is at highest possible
    int m = voter_count;

    // Iterate through candidate array
    for (int i = 0; i < candidate_count; i++)
    {
        // If candidate is not eliminated and has less votes than m
        if (!candidates[i].eliminated && candidates[i].votes < m)
        {
            // Assign votes as minimum
            m = candidates[i].votes;
        }
    }
    return m;
}

// Return true if the election is tied between all candidates, false otherwise
bool is_tie(int min)
{
    // Initialize votes as 0
    int v = 0;

    // Iterate through candidate array
    for (int i = 0; i < candidate_count; i++)
    {
        // Check if candidate has been eliminated
        if (!candidates[i].eliminated)
        {
            // If v is still 0, use candidates votes as v
            if (v == 0)
            {
                v = candidates[i].votes;
            }
            // Otherwise check if candidate has different votes count
            else if (v != candidates[i].votes)
            {
                return false;
            }
        }
    }
    return true;
}

// Eliminate the candidate (or candidates) in last place
void eliminate(int min)
{
    // Iterate through candidate array
    for (int i = 0; i < candidate_count; i++)
    {
        // If candidate has not been eliminated and has min votes, set eliminiated to true
        if (!candidates[i].eliminated && candidates[i].votes == min)
        {
            candidates[i].eliminated = true;
        }
    }
    return;
}
