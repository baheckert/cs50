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

// Function prototypes
bool vote(int rank, string name, int ranks[]);
void record_preferences(int ranks[]);
void add_pairs(void);
void sort_pairs(void);
void lock_pairs(void);
void print_winner(void);
bool creates_cycle(int from, int to);

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

    //DEBUG
    //printf("DEBUG preference array:\n");
    //for (int i = 0, l = candidate_count; i < l; i++)
    //{
    //    for (int j = 0; j < l; j++)
    //    {
    //          printf("%i", preferences[i][j]);
    //    }
    //    printf("\n");
    //}

    add_pairs();
    //DEBUG
    //printf("DEBUG pairs added:\n");
    //for (int i = 0, l = pair_count; i < l; i++)
    //{
    //    printf("winner: %i, loser: %i\n", pairs[i].winner, pairs[i].loser);
    //}
    sort_pairs();
    lock_pairs();
    print_winner();
    return 0;
}

// Update ranks given a new vote
bool vote(int rank, string name, int ranks[])
{
    // Check Candidates for name, add candidate index to ranks array
    for (int i = 0; i < candidate_count; i++)
    {
        if (!strcmp(candidates[i], name))
        {
            ranks[rank] = i;
            return true;
        }
    }
    //if no 'name' in Candidates, return false
    return false;
}

// Update preferences given one voter's ranks
// 'ranks' example: {2, 0, 1}
// preferences[2][0] ++, preferences[2][1] ++, and preferences[0][1] ++
void record_preferences(int ranks[])
{
    // TRY: Might be possible to use recursion here?
    // For each rank except last, increment preference[i][j]
    for (int i = 0, l = candidate_count - 1; i < l; i++)
    {
        for (int j = i + 1, o = candidate_count; j < o; j++)
        {
            preferences[ranks[i]][ranks[j]]++;
        }
    }
    return;
}

// Record pairs of candidates where one is preferred over the other
void add_pairs(void)
{
    // Iterate through preferences, add winner and loser to pairs
    for (int i = 0, l = candidate_count; i < l; i++)
    {
        for (int j = 0; j < l; j++)
        {
            if (i != j && preferences[i][j] > preferences[j][i])
            {
                pairs[pair_count].winner = i;
                pairs[pair_count].loser = j;
                pair_count++;
            }
        }
    }
    return;
}

// Sort pairs in decreasing order by strength of victory
void sort_pairs(void)
{
    // Temporary sorted pair
    pair sorted[pair_count];

    // Fill sorted in decreasing order by strength of victory
    for (int i = 0; i < pair_count; i++)
    {
        // Current high strength of victory and index of that strength
        int h_index = 0;
        int h_strength = 0;

        // Find 'i'th strength of victory
        for (int j = 0; j < pair_count; j++)
        {
            int winner = preferences[pairs[j].winner][pairs[j].loser];
            int loser = preferences[pairs[j].loser][pairs[j].winner];
            if (winner - loser > h_strength)
            {
                h_strength = winner - loser;
                h_index = j;
            }
        }

        // Add 'i'th strength of victory to sorted[i]
        sorted[i].winner = pairs[h_index].winner;
        sorted[i].loser = pairs[h_index].loser;
        // Set pairs[h_index] to a tie
        pairs[h_index].loser = pairs[h_index].winner;
    }

    // TRY: can entire sorted struct be assigned to pairs struct without iteration?
    // Replace pairs with sorted
    for (int i = 0; i < pair_count; i++)
    {
        pairs[i].winner = sorted[i].winner;
        pairs[i].loser = sorted[i].loser;

        //DEBUG
        //printf("DEBUG pairs sorted:");
        //printf("pairs %i, winner: %i loser: %i \n", i, pairs[i].winner, pairs[i].loser);
        //printf("strength: %i - %i\n", preferences[pairs[i].winner][pairs[i].loser], preferences[pairs[i].loser][pairs[i].winner]);
    }
    return;
}

// Lock pairs into the candidate graph (bool locked[]) in order, without creating cycles
void lock_pairs(void)
{
    // Iterate through each pair
    for (int i = 0; i < pair_count; i++)
    {
        // Add 'i'th pair lock to locked array
        locked[pairs[i].winner][pairs[i].loser] = true;
        for (int x = 0; x < candidate_count; x++)
        {
            for (int y = 0; y < candidate_count; y++)
            {
                printf("%i", locked[x][y]);
            }
            printf("\n");
        }
        // Remove lock if cycle would be created
        if (creates_cycle(pairs[i].winner, pairs[i].loser))
        {
            locked[pairs[i].winner][pairs[i].loser] = false;
        }
    }

    return;
}

// Takes locked array, returns true if cycle will not be created
bool creates_cycle(int from, int to)
{
    // Base case, cycle found
    if (from == to)
    {
        return true;
    }
    else
    {
        for (int i = 0; i < candidate_count; i++)
        {
            if (locked[from][i])
            {
                return creates_cycle(i, to);
            }
        }

        // Does not form cycle
        return false;
    }
}

// Print the winner of the election
void print_winner(void)
{
    // Iterate through each column
    for (int j = 0; j < candidate_count; j++)
    {
        // Has a lock been found?
        bool lock = false;

        // Iterate through each row looking for a lock in column
        for (int i = 0; i < candidate_count; i++)
        {
            if (locked[i][j])
            {
                // Lock found in column
                lock = true;
            }
        }

        // If no lock in column, print candidate name
        if (!lock)
        {
            printf("%s\n", candidates[j]);
        }
    }
    return;
}

