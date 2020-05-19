//Libraries
#include <stdio.h>
#include <cs50.h>
#include <string.h>
#include <ctype.h>

//Declare functions
int count_letters(string t);
int count_words(string t);
int count_sentences(string t);

int main(void)
{
    //Get text input from user
    string text = get_string("Text: ");

    //Assign counts to variables
    float letters = count_letters(text);
    float words = count_words(text);
    float sentences = count_sentences(text);

    //Calculate index
    float l = letters / words * 100;
    float s = sentences / words * 100;
    float index = 0.0588 * l - 0.296 * s -  15.8;

    //If index is 16 or above, print out "Grade 16+"
    if (index > 15)
    {
        printf("Grade 16+\n");
    }
    //If index is less than 1, print out "Before Grade 1"
    else if (index < 1)
    {
        printf("Before Grade 1\n");
    }
    //Otherwise, print out index as nearest whole number
    else
    {
        printf("Grade %1.f\n", index);
    }
}

//Count alphabetical chars
int count_letters(string t)
{
    int count = 0;
    for (int i = 0, l = strlen(t); i < l; i++)
    {
        if (isalpha(t[i]))
        {
            count++;
        }
    }
    return count;
}

//Count spaces and infer word count per problem set instructions
int count_words(string t)
{
    int count = 0;
    for (int i = 0, l = strlen(t); i < l; i++)
    {
        if (isspace(t[i]))
        {
            count++;
        }
    }
    return count + 1;
}

//Count ".", "!", and "?" and infer sentence count per problem set instructions
int count_sentences(string t)
{
    int count = 0;
    for (int i = 0, l = strlen(t); i < l; i++)
    {
        if (t[i] == '.' || t[i] == '!' || t[i] == '?')
        {
            count++;
        }
    }
    return count;
}