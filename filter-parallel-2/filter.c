#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <omp.h>
#include "helpers.h"
#include <time.h>
#include <mpi.h>
//#include <mpi.h>

int main(int argc, char *argv[])
{
    int thread_id;
    thread_id = omp_get_thread_num();
    printf("Thread ID: %d\n", thread_id );
    
    // MPI
    int process_Rank, size_Of_Cluster;
    MPI_Status status;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &process_Rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size_Of_Cluster);

    printf("Hello World from process %d of %d\n", process_Rank, size_Of_Cluster);
    MPI_Finalize();
    // argv to nrows, ncols
    //int ncols = atoi(argv[1]);
    //int nrows = atoi(argv[2]);
    //int omp_chunk_size = 100;
    //int mpi_chunks = numnodes;
    //int mpi_chunk_size = (ncols * nrows) / mpi_chunks;

    // Define allowable filters
    char *filters = "begra";

    // Get filter flag and check validity
    char filter = getopt(argc, argv, filters);
    if (filter == '?')
    {
        fprintf(stderr, "Invalid filter.\n");
        return 1;
    }

    // Ensure only one filter
    if (getopt(argc, argv, filters) != -1)
    {
        fprintf(stderr, "Only one filter allowed.\n");
        return 2;
    }

    // Ensure proper usage
    if (argc != optind + 2)
    {
        fprintf(stderr, "Usage: filter [flag] infile outfile\n");
        return 3;
    }

    // Remember filenames
    char *infile = argv[optind];
    char *outfile = argv[optind + 1];	


//    if (process_Rank == 0)
  //  {
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
//    }

    // Read infile's BITMAPFILEHEADER
    BITMAPFILEHEADER bf;
    fread(&bf, sizeof(BITMAPFILEHEADER), 1, inptr);

    // Read infile's BITMAPINFOHEADER
    BITMAPINFOHEADER bi;
    fread(&bi, sizeof(BITMAPINFOHEADER), 1, inptr);

    // Ensure infile is (likely) a 24-bit uncompressed BMP 4.0
  //  if (bf.bfType != 0x4d42 || bf.bfOffBits != 54 || bi.biSize != 40 ||
  //      bi.biBitCount != 24 || bi.biCompression != 0)
  //  {
  //      fclose(outptr);
  //      fclose(inptr);
  //      fprintf(stderr, "Unsupported f	ile format.\n");
  //      return 6;
  //  }

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

    // Iterate over infile's scanlines
    for (int i = 0; i < height; i++)
    {
        // Read row into pixel array
        fread(image[i], sizeof(RGBTRIPLE), width, inptr);

        // Skip over padding
        fseek(inptr, padding, SEEK_CUR);
    }

    // Filter image
    // Calculate the time taken by fun()
    clock_t t;
    t = clock();
    switch (filter)
    {
        // Blur
        case 'b':
            blur(height, width, image);
            break;

        // Edges
        case 'e':
            edges(height, width, image);
            break;

        // Grayscale
        case 'g':
            grayscale(height, width, image);
            break;

        // Reflect
        case 'r':
            reflect(height, width, image);
            break;
            
        // Reflect
        case 'a':      
            blur(height, width, image);
            edges(height, width, image);
            grayscale(height, width, image);
            reflect(height, width, image);
            break;
            
                        
    }

    // Write outfile's BITMAPFILEHEADER
    fwrite(&bf, sizeof(BITMAPFILEHEADER), 1, outptr);

    // Write outfile's BITMAPINFOHEADER
    fwrite(&bi, sizeof(BITMAPINFOHEADER), 1, outptr);

    // Write new pixels to outfile
    for (int i = 0; i < height; i++)
    {
        // Write row to outfile
        fwrite(image[i], sizeof(RGBTRIPLE), width, outptr);

        // Write padding at end of row
        for (int k = 0; k < padding; k++)
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
    t = clock() - t;
    
    // Print the runtime
    double time_taken = ((double)t)/CLOCKS_PER_SEC; // in seconds
    printf("Edit function took %f seconds to execute \n", time_taken);
    return 0;
}
