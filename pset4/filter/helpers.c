#include "helpers.h"
#include <stdio.h>
#include <math.h>

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    // Iterate through pixels in image
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            // Calculate average of red, blue, and green
            float avg = ((float) image[i][j].rgbtRed + (float) image[i][j].rgbtGreen + (float) image[i][j].rgbtBlue) / 3.0;

            // Assign average value to each color
            image[i][j].rgbtRed = round(avg);
            image[i][j].rgbtGreen = round(avg);
            image[i][j].rgbtBlue = round(avg);
        }
    }
    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    // Create placeholder image
    RGBTRIPLE out_image[height][width];

    // Iterate through pixels in image
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            // Put row into placeholder in reverse order
            out_image[i][j] = image[i][width - 1 - j];
        }
    }

    // Put placeholder image into image
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            image[i][j] = out_image[i][j];
        }
    }
    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    // Create placeholder image
    RGBTRIPLE out_image[height][width];

    // Iterate through pixels in image
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            // Initialize values as black
            int red = 0x00;
            int green = 0x00;
            int blue = 0x00;

            // Count pixels added
            int cnt = 0;

            // Iterate through 3x3 grid around pixel
            for (int x = i - 1; x <= i + 1; x++)
            {
                if (x >= 0 && x <= height - 1)
                {
                    for (int y = j - 1; y <= j + 1; y++)
                    {
                        if (y >= 0 && y <= width - 1)
                        {
                            cnt++;
                            red += image[x][y].rgbtRed;
                            green += image[x][y].rgbtGreen;
                            blue += image[x][y].rgbtBlue;
                        }
                    }
                }
            }

            // Assign color values to placeholder pixel
            out_image[i][j].rgbtRed = round((float) red / (float) cnt);
            out_image[i][j].rgbtGreen = round((float) green / (float) cnt);
            out_image[i][j].rgbtBlue = round((float) blue / (float) cnt);

            //DEBUG
            //printf("cnt: %i, blue: %i, old blue: %i, new blue: %i\n", cnt, blue, image[i][j].rgbtBlue, out_image[i][j].rgbtBlue);
        }
    }

    // Put placeholder image into image
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            image[i][j] = out_image[i][j];
        }
    }
    return;
}

// Takes rgbtriple color value and apllies Sobel operator
// Input: image height, image width, color value, x position, y postition
RGBTRIPLE sobel(int height, int width, RGBTRIPLE image[height][width], int x, int y)
{
    // Create new pixel
    RGBTRIPLE newPixel;

    // Create new value variables
    int gxRed = 0;
    int gyRed = 0;
    int gxGreen = 0;
    int gyGreen = 0;
    int gxBlue = 0;
    int gyBlue = 0;

    // Create Gx and Gy arrays for multipliers
    int Gx[3][3] = {{-1, 0, 1}, {-2, 0, 2}, {-1, 0, 1}};
    int Gy[3][3] = {{-1, -2, -1}, {0, 0, 0}, {1, 2, 1}};

    // Iterate through 3x3 grid around pixel
    for (int i = x - 1, sx = 0; sx < 3; i++, sx++)
    {
        for (int j = y - 1, sy = 0; sy < 3; j++, sy++)
        {
            // Check if pixel is within image
            if (i >= 0 && i <= height - 1 && j >= 0 && j <= width - 1)
            {
                // Multiply pixel's color values by Gx and Gy and add to new values
                gxRed += image[i][j].rgbtRed * Gx[sx][sy];
                gyRed += image[i][j].rgbtRed * Gy[sx][sy];
                gxGreen += image[i][j].rgbtGreen * Gx[sx][sy];
                gyGreen += image[i][j].rgbtGreen * Gy[sx][sy];
                gxBlue += image[i][j].rgbtBlue * Gx[sx][sy];
                gyBlue += image[i][j].rgbtBlue * Gy[sx][sy];

                //DEBUG
                //printf("%i += %i * %i\n", gxRed, image[i][j].rgbtRed, Gx[sx][sy]);
            }
        }
    }

    //DEBUG
    //printf("gx: %i, gy: %i, sobel: %i\n", gxRed, gyRed, (int)pow(gxRed, 2) + (int)pow(gyRed, 2));

    // Add new values to newPixel
    if (sqrt(pow(gxRed, 2) + pow(gyRed, 2)) > 255)
    {
        newPixel.rgbtRed = 255;
    }
    else
    {
        newPixel.rgbtRed = round(sqrt(pow(gxRed, 2) + pow(gyRed, 2)));
    }
    if (sqrt(pow(gxGreen, 2) + pow(gyGreen, 2)) > 255)
    {
        newPixel.rgbtGreen = 255;
    }
    else
    {
        newPixel.rgbtGreen = round(sqrt(pow(gxGreen, 2) + pow(gyGreen, 2)));
    }
    if (sqrt(pow(gxBlue, 2) + pow(gyBlue, 2)) > 255)
    {
        newPixel.rgbtBlue = 255;
    }
    else
    {
        newPixel.rgbtBlue = round(sqrt(pow(gxBlue, 2) + pow(gyBlue, 2)));
    }
    return newPixel;
}

// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width])
{
    // Create placeholder image
    RGBTRIPLE out_image[height][width];

    // Iterate through pixels in image
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            out_image[i][j] = sobel(height, width, image, i, j);

            //DEBUG
            //printf("old value: %i, sobel value: %i\n", image[i][j].rgbtRed, out_image[i][j].rgbtRed);
        }
    }

    // Put placeholder image into image
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            image[i][j] = out_image[i][j];
        }
    }
    return;
}
