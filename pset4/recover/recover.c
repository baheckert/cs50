#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#define BLOCK 512

typedef uint8_t BYTE;

int main(int argc, char *argv[])
{
    // Check for correct usage
    if (argc != 2)
    {
        printf("Usage: ./recover image\n");
        return 1;
    }

    // Open input file pointer
    FILE *inptr = fopen(argv[1], "r");
    if (inptr == NULL)
    {
        printf("Could not open %s.\n", argv[1]);
        return 1;
    }

    // Initiate output file pointer
    FILE *outptr = NULL;

    // Filename string
    char filename[8] = "000.jpg";

    // Create buffer
    BYTE buffer[BLOCK] = {0};

    // Count jpgs found
    int pic = 0;

    // Iterate through blocks of memory
    while (fread(buffer, BLOCK, 1, inptr) == 1)
    {
        // Spot likely start of jpg file
        if (buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff && (buffer[3] & 0xf0) == 0xe0)
        {
            // If first pic
            if (pic == 0)
            {
                // Create output file pointer
                outptr = fopen(filename, "w");
                if (outptr == NULL)
                {
                    printf("Could not create %s.\n", filename);
                    return 1;
                }

                // Write block to file
                fwrite(buffer, BLOCK, 1, outptr);

                // Add to pic counter
                pic++;
            }
            // If new pic
            else
            {
                // Close last file
                fclose(outptr);

                // Create new file name
                sprintf(filename, "%03i.jpg", pic);

                // Open new file
                outptr = fopen(filename, "w");

                // Write block to file
                fwrite(buffer, BLOCK, 1, outptr);

                // Add to pic counter
                pic++;
            }
        }
        else
        {
            // If first pic found
            if (pic > 0)
            {
                // Write block to file
                fwrite(buffer, BLOCK, 1, outptr);
            }
        }
    }
    // Input file reached end
    fclose(inptr);
    if (pic == 0)
    {
        printf("No images found.\n");
        return 1;
    }
    fclose(outptr);
    return 0;
}
