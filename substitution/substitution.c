//Substitution
//A program that encrypts messages using a substitution cipher

//Libraries
#include <stdio.h>
#include <cs50.h>
#include <string.h>
#include <ctype.h>

//Declare functions
char sub(char c);
bool is_invalid_key(string k);

//global variables
string key;
string plain_text;

int main(int argc, string argv[])
{
    //Check argument usage
    if (argc != 2)
    {
        printf("Usage: ./substitution key\n");
        //exit program
        return 1;
    }
    else if (strlen(argv[1]) != 26)
    {
        printf("Key must contain 26 characters.\n");
        //exit program
        return 2;
    }
    else if (is_invalid_key(argv[1]))
    {
        printf("Invalid key.\n");
        //exit program
        return 3;
    }
    //If key looks good, get plaintext and store key
    else
    {
        key = argv[1];
        plain_text = get_string("plaintext: ");
    }

    //start ciphertext printing
    printf("ciphertext: ");

    //Iterate through plaintext
    for (int i = 0, l = strlen(plain_text); i < l; i++)
    {
        //if char is alphabetical, print substitution
        if (isalpha(plain_text[i]))
        {
            printf("%c", sub(plain_text[i]));
        }
        else
        {
            printf("%c", plain_text[i]);
        }
    }
    printf("\n");

}

//Takes plain text char, checks case, returns cipher char in same case
char sub(char c)
{
    if (isupper(c))
    {
        //use ascii int as index for key
        int i = (int)c - 65;
        return toupper(key[i]);
    }
    else
    {
        //use ascii int as index for key
        int i = (int)c - 97;
        return tolower(key[i]);
    }
}

//Checks key for non-alpha or duplicate characters
bool is_invalid_key(string k)
{
    for (int i = 0, l = strlen(k) - 1; i < l; i++)
    {
        if (isalpha(k[i]))
        {
            for (int x = i + 1; x < l + 1; x++)
            {
                if (k[i] == k[x])
                {
                    return true;
                }
            }
        }
        else
        {
            return true;
        }
    }
    return false;

}


