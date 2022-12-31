#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <omp.h>
#include "helpers.h"

int main()
{

    int thread_id = omp_get_thread_num();
    int max_no_thread = omp_get_max_threads();
    printf("Thread ID: %d\n", thread_id);
    printf("Maximum Nb of Threads: %d\n", max_no_thread);
    omp_set_num_threads(max_no_thread);
    
    
        // Define allowable filters
    char *filters = "begra";

    // Get filter flag and check validity
    char *filter = "g";
    
    char *infile = "images/large.bmp";
    char *outfile = "output/out-large-parallel.bmp";

    // Open input file
    FILE *inptr = fopen(infile, "r");
    if (inptr == NULL)
    {
        fprintf(stderr, "Could not open %s.\n", infile);
        return 4;
    }

    // Open output file
    FILE *outptr = fopen(outfile, "w");
    if (outptr == NULL)
    {
        fclose(inptr);
        fprintf(stderr, "Could not create %s.\n", outfile);
        return 5;
    }

    // Read infile's BITMAPFILEHEADER
    BITMAPFILEHEADER bf;
    fread(&bf, sizeof(BITMAPFILEHEADER), 1, inptr);

    // Read infile's BITMAPINFOHEADER
    BITMAPINFOHEADER bi;
    fread(&bi, sizeof(BITMAPINFOHEADER), 1, inptr);

    // Ensure infile is (likely) a 24-bit uncompressed BMP 4.0
    //if (bf.bfType != 0x4d42 || bf.bfOffBits != 54 || bi.biSize != 40 ||
    //    bi.biBitCount != 24 || bi.biCompression != 0)
    //{
    //    fclose(outptr);
    //    fclose(inptr);
    //    fprintf(stderr, "Unsupported file format.\n");
    //    return 6;
    //}

    int height = abs(bi.biHeight);
    int width = bi.biWidth;

    // Allocate memory for image
    RGBTRIPLE(*image)[width] = calloc(height, width * sizeof(RGBTRIPLE));
    if (image == NULL)
    {
        fprintf(stderr, "Not enough memory to store image.\n");
        fclose(outptr);
        fclose(inptr);
        return 7;
    }

    // Determine padding for scanlines
    int padding = (4 - (width * sizeof(RGBTRIPLE)) % 4) % 4;
    int i;
    #pragma omp parallel for default(shared) shared(padding) private(i)
    // Iterate over infile's scanlines
    for (i = 0; i < height; i++)
    {
        // Read row into pixel array
        fread(image[i], sizeof(RGBTRIPLE), width, inptr);

        // Skip over padding
        fseek(inptr, padding, SEEK_CUR);
    }
    // Calculate the time taken by fun()
    clock_t t;
    t = clock();
    // Filter image

    grayscale(height, width, image);


    // Write outfile's BITMAPFILEHEADER
    fwrite(&bf, sizeof(BITMAPFILEHEADER), 1, outptr);

    // Write outfile's BITMAPINFOHEADER
    fwrite(&bi, sizeof(BITMAPINFOHEADER), 1, outptr);
    
    int ii;
    int k;
    #pragma omp parallel for default(shared) shared(padding) private(i,k)
    // Write new pixels to outfile
    for (ii = 0; ii < height; ii++)
    {
        // Write row to outfile
        fwrite(image[ii], sizeof(RGBTRIPLE), width, outptr);

        // Write padding at end of row
        for (k = 0; k < padding; k++)
        {
            fputc(0x00, outptr);
        }
    }

    // Free memory for image
    free(image);

    // Close infile
    fclose(inptr);

    // Close outfile
    fclose(outptr);
    
    // Print the runtime
    t = clock() - t;
    double time_taken = ((double)t)/CLOCKS_PER_SEC; // in seconds
    printf("Edit function took %f seconds to execute \n", time_taken);
    return 0;
}
