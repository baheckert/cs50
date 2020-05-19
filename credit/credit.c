//Inclusions
#include <stdio.h>
#include <cs50.h>

int main(void)
{
    //Get card number
    long card = get_long("Number: ");

    //Calculate checksum
    long n = card;
    int checksum;
    int pos = 1;
    while (n > 0)
    {
        int p = n % 10;
        n = n / 10;
        if (pos % 2 == 0)
        {
            p = p * 2;
            while (p  > 0)
            {
                int d = p % 10;
                p = p / 10;
                checksum += d;
            }
        }
        else
        {
            checksum += p;
        }
        pos++;
    }
    if (checksum % 10 == 0)
    {
        //American Express
        //15 digits, starts with 34 or 37
        if (card / 10000000000000 == 34 || card / 10000000000000 == 37)
        {
            printf("AMEX\n");
        }
        //Mastercard
        //16 digits, starts with 51-55
        else if (card / 100000000000000 >= 51 & card / 100000000000000 <= 55)
        {
            printf("MASTERCARD\n");
        }
        //VISA
        //13 or 16 digits, starts with 4
        else if (card / 1000000000000 == 4 || card / 1000000000000000 == 4)
        {
            printf("VISA\n");
        }
        else
        {
            printf("INVALID\n");
        }
    }
    else
    {
        printf("INVALID\n");
    }


}
