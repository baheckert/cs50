//Inclusions
#include <stdio.h>
#include <cs50.h>

int get_height(void);

int main(void)
{
    //get height
    int height = get_height();

    int width = 1;

    //make pyramid
    for (int r = 0; r < height; r++)
    {
        for (int s1 = 1; s1 < height - r; s1++)
        {
            printf(" ");
        }
        for (int p1 = 0; p1 < width; p1++)
        {
            printf("#");
        }
        printf("  ");
        for (int p2 = 0; p2 < width; p2++)
        {
            printf("#");
        }
        printf("\n");
        width++;
    }

}

//prompt user for an integer between 1 and 8
int get_height(void)
{
    int h;

    do
    {
        h = get_int("Height: ");
    }

    while (h < 1 || h > 8);
    return h;
}