// Implements a dictionary's functionality

#include <stdbool.h>
#include <stdio.h>
#include <strings.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#include "dictionary.h"

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
}
node;

// Number of buckets in hash table
const unsigned int N = 26;

// Hash table
node *table[N];

// Global dictionary size variable
int dict_size = 0;

// Returns true if word is in dictionary else false
bool check(const char *word)
{
    //DEBUG
    //printf("%s, %i\n", word, hash(word));
    // Point cursor at bucket
    node *cursor = table[hash(word)];

    // If bucket has no nodes, return false
    if (cursor == NULL)
    {
        return false;
    }
    // Compare words then move to next node
    do
    {
        if (!strcasecmp(cursor->word, word))
        {
            return true;
        }
        else
        {
            cursor = cursor->next;
        }
    }
    while (cursor != NULL);
    return false;
}

// Hashes word to a number
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
    // Open file
    FILE *inptr = fopen(dictionary, "r");
    if (inptr == NULL)
    {
        fprintf(stderr, "Could not open %s.\n", dictionary);
        return false;
    }

    // Initiate char * variable
    char word_in[LENGTH];

    // Iterate through file
    while (fscanf(inptr, "%s", word_in) != EOF)
    {
        // Create new node
        node *n = malloc(sizeof(node));
        if (n == NULL)
        {
            printf("Memory allocation error.\n");
            return false;
        }
        strcpy(n->word, word_in);
        n->next = NULL;

        // Get bucket index
        int index = hash(word_in);

        // Add node to front of bucket's list (Don't have to find end)
        n->next = table[index];
        table[index] = n;

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

// Unloads dictionary from memory, returning true if successful else false
bool unload(void)
{
    // Initiate cursor and tmp
    node *cursor = NULL;
    node *tmp = NULL;

    // Iterate through buckets
    for (int i = 0; i < N; i++)
    {
        cursor = table[i];
        while (cursor != NULL)
        {
            tmp = cursor;
            cursor = cursor->next;
            free(tmp);
        }
    }
    return true;
}
