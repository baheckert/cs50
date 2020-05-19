// Brad Heckert
// dictionary.c V2.1
// 2.1: Changed trie value to true/false instead of entire word
// 2.0: Changed dictionary from hash table to trie structure





// Implements a dictionary's functionality

#include <stdbool.h>
#include <stdio.h>
#include <strings.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#include "dictionary.h"

// Number of children in trie 26 letters and '
const unsigned int N = 27;

// Represents a node in a hash table
typedef struct trie
{
    bool end;
    struct trie *next[N];
}
trie;

// Create pointer to root
trie *root;

// Global dictionary size variable
int dict_size = 0;

// Returns true if word is in dictionary else false
bool check(const char *word)
{
    // Point cursor at root
    trie *cursor = root;

    // Iterate through word and follow path
    for (int i = 0; i < LENGTH + 1; i++)
    {
        // If terminating char, check 'end' for true at trie
        if (word[i] == '\0')
        {
            if (cursor->end)
            {
                return true;
            }
            else
            {
                return false;
            }
        }

        // Get key of letter
        int key = (int)tolower(word[i]) - 96;
        if ((int)tolower(word[i]) == 39)
        {
            key = 0;
        }

        // Check for null path
        if (cursor->next[key] == NULL)
        {
           return false;
        }
        else
        {
            // Move cursor to next trie
            cursor = cursor->next[key];
        }
    }
    // Should not reach end without returning bool
    printf("Check function reached end.\n");
    return false;
}

// Hash is not called/used in trie implementation
unsigned int hash(const char *word)
{
    int ascii_modifier = 97;
    // First letter in lowercase
    char a = tolower(word[0]);
    return (int)a - ascii_modifier;
}

// Loads dictionary into memory, returning true if successful else false
bool load(const char *dictionary)
{
    // Create root trie
    trie *r = malloc(sizeof(trie));
    if (r == NULL)
    {
        printf("Memory allocation error.\n");
        return false;
    }
    // Set all root pointers to NULL
    for (int t = 0; t < N; t++)
    {
        r->next[t] = NULL;
    }
    // Point *root at root trie
    root = r;

    // Open file
    FILE *inptr = fopen(dictionary, "r");
    if (inptr == NULL)
    {
        fprintf(stderr, "Could not open %s.\n", dictionary);
        return false;
    }

    // Initiate char * variable
    char word_in[LENGTH + 1];

    // Iterate through file
    while (fscanf(inptr, "%s", word_in) != EOF)
    {
        // Point cursor at root
        trie *cursor = root;

            // Iterate through new word
        for (int i = 0; i < LENGTH + 1; i++)
        {
            //DEBUG
            //printf("value: %s\nletter: %c\n", cursor->word, word_in[i]);

            // If terminating char, check for end true
            if (word_in[i] == '\0')
            {
                if (!cursor->end)
                {
                    cursor->end = true;
                    break;
                }
                else
                {
                    printf("Attempted to add word to filled trie\n");
                    return false;
                }
            }
            else
            {
                // Get key of letter
                int key = (int)word_in[i] - 96;
                if ((int)word_in[i] == 39)
                {
                    key = 0;
                }

                // Check for null path and move cursor
                if (cursor->next[key] == NULL)
                {
                    // Create trie
                    trie *n = malloc(sizeof(trie));
                    if (n == NULL)
                    {
                        printf("Memory allocation error.\n");
                        return false;
                    }
                    // Initialize new trie values
                    n->end = false;
                    for (int t = 0; t < N; t++)
                    {
                        n->next[t] = NULL;
                    }
                    // Put next to new trie
                    cursor->next[key] = n;

                    // Move cursor to next trie
                    cursor = cursor->next[key];

                    //DEBUG
                    //printf("cursor moved to: %i\n", key);
                }
                else
                {
                    // Move cursor to next trie
                    cursor = cursor->next[key];

                    //DEBUG
                    //printf("cursor moved to: %i\n", key);
                }
            }
        }

        // Count dictionary addition
        dict_size++;
    }

    fclose(inptr);
    return true;
}

// Returns number of words in dictionary if loaded else 0 if not yet loaded
unsigned int size(void)
{
    // Return dictionary size
    return dict_size;
}

// Recursive function frees all children of given trie
void free_trie (trie *n)
{
    // Iterate over each next pointer and free them
    for (int i = 0; i < N; i++)
    {
        if (n->next[i] != NULL)
        {
            free_trie(n->next[i]);
            n->next[i] = NULL;
        }
    }

    // free n
    free(n);
}

// Unloads dictionary from memory, returning true if successful else false
bool unload(void)
{
    free_trie(root);
    return true;
}
