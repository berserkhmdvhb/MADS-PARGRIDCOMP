#include "helpers.h"
#include <math.h>
#include <stdio.h>
#include <omp.h>
// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    int max_no_thread = omp_get_max_threads();
    omp_set_num_threads(max_no_thread);
    //omp_set_num_threads(2);
    //Check if all array elements are not equal
    //int n = 3;
    //while(--n>0 && a[n]==a[0]);
    //if (n!=0)
    int i,j;	
    #pragma omp parallel for default(shared) private(i, j) collapse(2)
    for (i = 0; i < height; i++)
    {
        for (j = 0; j < width; j++)
        {
            BYTE *b = &image[i][j].rgbtBlue;
            BYTE *g = &image[i][j].rgbtGreen;
            BYTE *r = &image[i][j].rgbtRed;
            //int colors[3] = {b, g, r};
            //Check if all array elements are not equal
            //int n = 3;
            //while(--n > 0 && (*a[n]) == (*a[0]));
            //if (n != 0)
            int avg = round((float)(*b + *g + *r) / 3);
            *b = *g = *r = avg;
            //printf("%d",avg);
        }
    }


    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    int k = 0;
    int i,j;
    #pragma omp parallel for default(shared) private(i, j) collapse(2)
    for (i = 0; i < height; i++)
    {
        for (j = 0; j < width / 2; j++)
        {
            RGBTRIPLE temp = image[i][j];
            image[i][j] = image[i][width - (j + 1)];
            image[i][width - (j + 1)] = temp;
        }
    }
    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    //create a duplicate (backup) of the image (so that values are not lost when changing)
    RGBTRIPLE imagedp[height][width];
    #pragma omp parallel for default(shared) collapse(2)
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            imagedp[i][j] = image[i][j];
        }
    }
		
    #pragma omp parallel for default(shared) collapse(2)	
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            BYTE *b = &image[i][j].rgbtBlue;
            BYTE *g = &image[i][j].rgbtGreen;
            BYTE *r = &image[i][j].rgbtRed;

            //initializing bounding sums for each color
            float boundB = 0;
            float boundG = 0;
            float boundR = 0;

            //form the bounding box of elements of the center element and append to bound array
            //It will include pairs:
            //(i-1, j-1), (i-1, j), (i-1, j+1),
            //(i, j-1), (i, j), (i, j+1),
            //(i+1, j-1), (i+1, j), (i+1, j+1)



            ///////////////////handle edges and corner first//////////////
            ///check first and last row, first and last column
            ///initialize MR1 & MR2 (MC1 & MC2) for eliminating rows (columns in edges and corners
            //defining modifying row and column variables
            int MR1 = 0, MR2 = 0, MC1 = 0, MC2 = 0;
            //int count = 1;

            //first row
            if (i == 0)
            {
                //upper left corner
                if (j == 0)
                {
                    MR1 = 1;
                    MC1 = 1;
                    //count = 4;
                }
                //upper right corner
                else if (j == (width - 1))
                {
                    MR1 = 1;
                    MC2 = -1;
                    //count = 4;
                }
                //edges in first row
                else
                {
                    MR1 = 1;
                    //count = 6;
                }
            }

            //last row
            else if (i == (height - 1))
            {
                //lower left corner
                if (j == 0)
                {
                    MR2 = -1;
                    MC1 = 1;
                    //count = 4;
                }
                //lower right corner
                else if (j == (width - 1))
                {
                    MR2 = -1;
                    MC2 = -1;
                    //count = 4;
                }
                //edges in last row
                else
                {
                    MR2 = -1;
                    //count = 6;
                }
            }
            //first column
            else if (j == 0)
            {
                //only edges
                if (i != 0 && i != (height - 1))
                {
                    MC1 = 1;
                    //count = 6;
                }
            }
            //last column
            else if (j == (width - 1))
            {
                //only edges
                if (i != 0 && i != (height - 1))
                {
                    MC2 = -1;
                    //count = 6;
                }
            }
            ////////////////////////////Cases for edges and corners finished/////////
            else
            {
                MR1 = 0, MR2 = 0, MC1 = 0, MC2 = 0;
                //count = 9;
            }

	    #pragma omp parallel for default(shared) collapse(2)
            int k = 0;
            for (int n = (-1 + MR1); n < (2 + MR2); n++)
            {
                for (int m = (-1 + MC1); m < (2 + MC2); m++)
                {
                    boundB += imagedp[i + n][j + m].rgbtBlue;
                    boundG += imagedp[i + n][j + m].rgbtGreen;
                    boundR += imagedp[i + n][j + m].rgbtRed;

                    //debugging for (i=5, j=4):
                    //}
                    //if (i > 4 && i < 6 && j > 3 && j < 5)
                    //{
                    //    printf("(%i, %i): n: %i,m: %i", i, j, n, m);
                    //    printf("\n MR1: %i, MR2: %i, MC1: %i, MC2: %i\n\n", MR1, MR2, MC1, MC2);
                    //}
                    k++;
                }
            }
            //if (i > 5 && i < 7 && j > 3 && j < 5)
            //{
            //    printf("orig blue: %u \n sum: %u\n average: %f\n", *g, boundG, round(boundG / k));
            //}

            //if (count != k)
            //{
            //    printf("Counting is done wrong\n k: %i, count: %i", k, count);
            //}

            // replace the center with average;
            *b = round(boundB / k);
            *g = round(boundG / k);
            *r = round(boundR / k);
        }
    }


    return;
}

// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE imagedp[height][width];
    #pragma omp parallel for default(shared) collapse(2)
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            imagedp[i][j] = image[i][j];
        }
    }

    #pragma omp parallel for default(shared) collapse(2)
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {

            BYTE *b = &image[i][j].rgbtBlue;
            BYTE *g = &image[i][j].rgbtGreen;
            BYTE *r = &image[i][j].rgbtRed;

            //initializing bounding sums for each color, for both x and y axis
            float boundBx = 0;
            float boundGx = 0;
            float boundRx = 0;
            float boundBy = 0;
            float boundGy = 0;
            float boundRy = 0;

            //form the bounding box of elements of the center element and append to bound array
            //It will include pairs:
            //(i-1, j-1), (i-1, j), (i-1, j+1),
            //(i, j-1), (i, j), (i, j+1),
            //(i+1, j-1), (i+1, j), (i+1, j+1)



            ///////////////////handle edges and corner first//////////////
            ///check first and last row, first and last column
            ///initialize MR1 & MR2 (MC1 & MC2) for eliminating rows (columns in edges and corners
            //defining modifying row and column variables
            int MR1 = 0, MR2 = 0, MC1 = 0, MC2 = 0;
            //int count = 1;

            //first row
            if (i == 0)
            {
                //upper left corner
                if (j == 0)
                {
                    MR1 = 1;
                    MC1 = 1;
                }
                //upper right corner
                else if (j == (width - 1))
                {
                    MR1 = 1;
                    MC2 = -1;
                    //count = 4;
                }
                //edges in first row
                else
                {
                    MR1 = 1;
                }
            }

            //last row
            else if (i == (height - 1))
            {
                //lower left corner
                if (j == 0)
                {
                    MR2 = -1;
                    MC1 = 1;
                }
                //lower right corner
                else if (j == (width - 1))
                {
                    MR2 = -1;
                    MC2 = -1;
                }
                //edges in last row
                else
                {
                    MR2 = -1;
                }
            }
            //first column
            else if (j == 0)
            {
                //only edges
                if (i != 0 && i != (height - 1))
                {
                    MC1 = 1;
                }
            }
            //last column
            else if (j == (width - 1))
            {
                //only edges
                if (i != 0 && i != (height - 1))
                {
                    MC2 = -1;
                }
            }
            ////////////////////////////Cases for edges and corners finished/////////
            else
            {
                MR1 = 0, MR2 = 0, MC1 = 0, MC2 = 0;
            }


            //Initialise Sobel kernerls
            int G_x[3][3] = {{-1, 0, 1}, {-2, 0, 2}, {-1, 0, 1}};
            int G_y[3][3] = {{-1, -2, -1}, {0, 0, 0}, {1, 2, 1}};

            int k = 0;
            #pragma omp parallel for default(shared) collapse(2)
            for (int n = (-1 + MR1); n < (2 + MR2); n++)
            {
                for (int m = (-1 + MC1); m < (2 + MC2); m++)
                {
                    boundBx += imagedp[i + n][j + m].rgbtBlue * G_x[n + 1][m + 1];
                    boundBy += imagedp[i + n][j + m].rgbtBlue * G_y[n + 1][m + 1];
                    boundGx += imagedp[i + n][j + m].rgbtGreen * G_x[n + 1][m + 1];
                    boundGy += imagedp[i + n][j + m].rgbtGreen * G_y[n + 1][m + 1];
                    boundRx += imagedp[i + n][j + m].rgbtRed * G_x[n + 1][m + 1];
                    boundRy += imagedp[i + n][j + m].rgbtRed * G_y[n + 1][m + 1];

                    //debugging for (i=5, j=4):
                    //}
                    //if (i > 4 && i < 6 && j > 3 && j < 5)
                    //{
                    //    printf("(%i, %i): n: %i,m: %i", i, j, n, m);
                    //    printf("\n MR1: %i, MR2: %i, MC1: %i, MC2: %i\n\n", MR1, MR2, MC1, MC2);
                    //}
                    k++;
                }
            }
            //defining Sobel operators
            int optb = round(sqrt(boundBx * boundBx + boundBy * boundBy));
            int optg = round(sqrt(boundGx * boundGx + boundGy * boundGy));
            int optr = round(sqrt(boundRx * boundRx + boundRy * boundRy));

            //crap at 255
            if (optb > 255)
            {
                optb = 255;
            }
            if (optg > 255)
            {
                optg = 255;
            }
            if (optr > 255)
            {
                optr = 255;
            }

            // replace the center with the operator;
            *b = optb;
            *g = optg;
            *r = optr;
        }
    }



    return;
}
