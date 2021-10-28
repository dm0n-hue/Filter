// Implement a program that applies filters to BMPs

#include "helpers.h"
#include <math.h>

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    //holds value for RGB values in each iteration
    double r = 0, g = 0, b = 0;
    //holds average of all three RGB values in a pixel
    double ave = 0;

    //iterates through each pixel
    for (int i = 0; i < height; i++)
    {
        for (int n = 0; n < width; n++)
        {
            r = image[i][n].rgbtRed;
            g = image[i][n].rgbtGreen;
            b = image[i][n].rgbtBlue;

            ave = (int) round((r + g + b) / 3);

            image[i][n].rgbtRed = ave;
            image[i][n].rgbtGreen = ave;
            image[i][n].rgbtBlue = ave;
        }
    }
    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    //variable that holds the length of an image's width
    int len = width - 1;

    for (int i = 0; i < height; i++)
    {
        //function will choose this "if" statement if the image has an even width length
        if (width % 2 == 0)
        {
            for (int n = 0; n < width / 2; n++)
            {
                //temporary variable to "hold" the pixel being iterated through
                RGBTRIPLE temp = image[i][n];
                //replacing pixels in original image
                image[i][n] = image[i][len - n];
                //adding the "held" pixel back into the original image
                image[i][len - n] = temp;
            }
        }
        //function will choose this satement if the image has an odd width length
        else
        {
            for (int m = 0; m < (width - 1) / 2; m++)
            {
                RGBTRIPLE temp = image[i][m];
                image[i][m] = image[i][len - m];
                image[i][len - m] = temp;
            }
        }
    }
    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    //store blurred pixels into this variable through each iteration
    RGBTRIPLE oldImage[height][width];

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            //will be used to store (and add) pixel values from different rows and columns
            double sumRed = 0, sumGreen = 0, sumBlue = 0;
            //finds the average number of pixels iterated through
            double average = 0;
            //can iterate through a maximum of 9 pixels as long it passes the "if" statement in the nested for loop
            for (int n = -1; n < 2; n++)
            {
                for (int m = -1; m < 2; m++)
                {
                    //boundary checking
                    if (i + n > height - 1 || i + n < 0 || j + m > width - 1 || j + m < 0)
                    {
                        continue;
                    }
                    //adds pixel values into temporary value holders
                    sumRed += image[i + n][j + m].rgbtRed;
                    sumGreen += image[i + n][j + m].rgbtGreen;
                    sumBlue += image[i + n][j + m].rgbtBlue;
                    //adds the number of times the for loops are iterated through (how many pixels have values of either 4, 6, or 9)
                    average++;
                }
            }
            //store the values into a temporary image variable
            oldImage[i][j].rgbtRed = (int) round(sumRed / average);
            oldImage[i][j].rgbtGreen = (int) round(sumGreen / average);
            oldImage[i][j].rgbtBlue = (int) round(sumBlue / average);
        }
    }
    //places the blurred image into the original image variable
    for (int e = 0; e < height; e++)
    {
        for (int o = 0; o < width; o++)
        {
            image[e][o] = oldImage[e][o];
        }
    }
    return;
}

// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width])
{
    //store the blurred pixel into this variable in each iteration
    RGBTRIPLE oldImage[height][width];

    //hardcoded kernels
    int gX[3][3] = {{-1, 0, 1}, {-2, 0, 2}, {-1, 0, 1}};
    int gY[3][3] = {{-1, -2, -1}, {0, 0, 0}, {1, 2, 1}};

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            //stores Gx and Gy RGB values
            double redX = 0, greenX = 0, blueX = 0, redY = 0, greenY = 0, blueY = 0;
            //will hold the G value for each RGB value
            double gRed = 0, gGreen = 0, gBlue = 0;
            //can iterate through a maximum of 9 pixels as long it passes the "if" statement in the nested for loop
            for (int n = -1; n < 2; n++)
            {
                for (int m = -1; m < 2; m++)
                {
                    //boundary checking
                    if (i + n > height - 1 || i + n < 0 || j + m > width - 1 || j + m < 0)
                    {
                        continue;
                    }
                    redX += image[i + n][j + m].rgbtRed * gX[n + 1][m + 1];
                    greenX += image[i + n][j + m].rgbtGreen * gX[n + 1][m + 1];
                    blueX += image[i + n][j + m].rgbtBlue * gX[n + 1][m + 1];

                    redY += image[i + n][j + m].rgbtRed * gY[n + 1][m + 1];
                    greenY += image[i + n][j + m].rgbtGreen * gY[n + 1][m + 1];
                    blueY += image[i + n][j + m].rgbtBlue * gY[n + 1][m + 1];
                }
            }

            //sobel operation
            gRed = (int) round(sqrt((redX * redX) + (redY * redY)));
            gGreen = (int) round(sqrt((greenX * greenX) + (greenY * greenY)));
            gBlue = (int) round(sqrt((blueX * blueX) + (blueY * blueY)));
            //checks each RGB's G value to make sure if it's over 255 or not
            if (gRed > 255)
            {
                oldImage[i][j].rgbtRed = 255;
            }
            else
            {
                oldImage[i][j].rgbtRed = gRed;
            }

            if (gGreen > 255)
            {
                oldImage[i][j].rgbtGreen = 255;
            }
            else
            {
                oldImage[i][j].rgbtGreen = gGreen;
            }

            if (gBlue > 255)
            {
                oldImage[i][j].rgbtBlue = 255;
            }
            else
            {
                oldImage[i][j].rgbtBlue = gBlue;
            }
        }
    }
    //stores the temporary image variable into the original image variable
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            image[i][j] = oldImage[i][j];
        }
    }
    return;
}
