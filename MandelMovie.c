/*********************
**  Mandelbrot fractal movie generator
** clang -Xpreprocessor -fopenmp -lomp -o Mandelbrot Mandelbrot.c
** by Dan Garcia <ddgarcia@cs.berkeley.edu>
** Modified for this class by Justin Yokota and Chenyu Shi
**********************/

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>
#include "ComplexNumber.h"
#include "Mandelbrot.h"
#include <sys/types.h>
#include "ColorMapInput.h"
#include <string.h>

void printUsage(char* argv[])
{
  printf("Usage: %s <threshold> <maxiterations> <center_real> <center_imaginary> <initialscale> <finalscale> <framecount> <resolution> <output_folder> <colorfile>\n", argv[0]);
  printf("    This program simulates the Mandelbrot Fractal, and creates an iteration map of the given center, scale, and resolution, then saves it in output_file\n");
}


/*
This function calculates the threshold values of every spot on a sequence of frames. The center stays the same throughout the zoom. First frame is at initialscale, and last frame is at finalscale scale.
The remaining frames form a geometric sequence of scales, so
if initialscale=1024, finalscale=1, framecount=11, then your frames will have scales of 1024, 512, 256, 128, 64, 32, 16, 8, 4, 2, 1.
As another example, if initialscale=10, finalscale=0.01, framecount=5, then your frames will have scale 10, 10 * (0.01/10)^(1/4), 10 * (0.01/10)^(2/4), 10 * (0.01/10)^(3/4), 0.01 .
*/
void MandelMovie(double threshold, u_int64_t max_iterations, ComplexNumber* center, double initialscale, double finalscale, int framecount, u_int64_t resolution, u_int64_t ** output){
    // YOUR CODE HERE
//    int i = 0;
//    if (framecount != 1) {
//    for (double scale = initialscale; scale != finalscale; scale = scale * exp(log(finalscale / initialscale) / (framecount - 1))) {
//      Mandelbrot(threshold, max_iterations, center, scale, resolution, output[i]);
//      i = i + 1;
 //   }
//    }
//    Mandelbrot(threshold, max_iterations, center, finalscale, resolution, output[i]);
    double scale = initialscale;
    if (framecount == 1) {
      Mandelbrot(threshold, max_iterations, center, finalscale, resolution, output[0]);
      return;
    }
    for (int frame = 0; frame < framecount; frame = frame + 1) {
      Mandelbrot(threshold, max_iterations, center, scale, resolution, output[frame]);
      scale = scale * exp(log(finalscale / initialscale) / (framecount - 1));
    }  
}


/**************
**This main function converts command line inputs into the format needed to run MandelMovie.
**It then uses the color array from FileToColorMap to create PPM images for each frame, and stores it in output_folder
***************/
int main(int argc, char* argv[])
{
	//Tips on how to get started on main function:
	//MandelFrame also follows a similar sequence of steps; it may be useful to reference that.
	//Mayke you complete the steps below in order.

	//STEP 1: Convert command line inputs to local variables, and ensure that inputs are valid.
	/*
	Check the spec for examples of invalid inputs.
	Remember to use your solution to B.1.1 to process colorfile.
	*/

	//YOUR CODE HERE
  if (argc != 11) {
    printf("%s: Wrong number of arguments, expecting 10\n", argv[0]);
		printUsage(argv);
		return 1;
  }
  double threshold, initialscale, finalscale;
  int framecount;
	ComplexNumber *center;
	u_int64_t maxiterations, resolution;
  char *colorfile;
  uint8_t **colors;
  int *colorcount;

  threshold = atof(argv[1]);
	maxiterations = (u_int64_t)atoi(argv[2]);
	center = newComplexNumber(atof(argv[3]), atof(argv[4]));
	initialscale = atof(argv[5]);
  finalscale = atof(argv[6]);
  framecount = atof(argv[7]);
	resolution = (u_int64_t)atoi(argv[8]);
  colorfile = argv[10];
  colorcount = (int *) malloc(sizeof(int));
  if (colorcount == NULL) {
     freeComplexNumber(center);
     return 1;
}
  colors = FileToColorMap(colorfile, colorcount);
  if (colors == NULL) {
    freeComplexNumber(center);
    free(colorcount);
    return 1;
  }

  if (threshold <= 0 || maxiterations <= 0 || initialscale <= 0
    || finalscale <= 0 || framecount > 10000 || framecount <= 0
    || resolution < 0 || (framecount == 1 && initialscale != finalscale)) {
      freeComplexNumber(center);
      for (int i = 0; i < (*colorcount); i = i + 1) {
        free(colors[i]);
      }
      free(colorcount);
      free(colors);
      return 1;
  }

	//STEP 2: Run MandelMovie on the correct arguments.
	/*
	MandelMovie requires an output array, so make sure you allocate the proper amount of space.
	If allocation fails, free all the space you have already allocated (including colormap), then return with exit code 1.
	*/

	//YOUR CODE HERE
  u_int64_t size = 2 * resolution + 1;
  u_int64_t **arr = (u_int64_t **) malloc(framecount * sizeof(u_int64_t *));
  if (arr == NULL) {
    for (int i = 0; i < *colorcount; i = i + 1) {
      free(colors[i]);
    }
    free(colors);
    free(colorcount);
    freeComplexNumber(center);
    return 1;
  }
  for (int i = 0; i < framecount; i = i + 1) {
    arr[i] = (uint64_t *) malloc(size * size * sizeof(uint64_t));
    if (arr[i] == NULL) {
      for (int j = i - 1; j >= 0; j = j - 1) {
        free(arr[j]);
      }
      for (int j = 0; j < *colorcount; j = j + 1) {
        free(colors[j]);
      }
      free(colors);
      free(arr);
      free(colorcount);
      freeComplexNumber(center);
      return 1;
    }
  }
  MandelMovie(threshold, maxiterations, center, initialscale, finalscale, framecount, resolution, arr);




	//STEP 3: Output the results of MandelMovie to .ppm files.
	/*
	Convert from iteration count to colors, and output the results into output files.
	Use what we showed you in Part B.1.2, create a seqeunce of ppm files in the output folder.
	Feel free to create your own helper function to complete this step.
	As a reminder, we are using P6 format, not P3.
	*/

	//YOUR CODE HERE
  for (int i = 0; i < framecount; i = i + 1) {
    char buffer[100];
    sprintf(buffer, "%s/frame%05d.ppm", argv[9], i);
    FILE *out = fopen(buffer, "w+");
    if (out == NULL) {
      for (int k  = framecount - 1; k >= 0; k = k - 1) {
        free(arr[k]);
      }
      for (int j = 0; j < *colorcount; j = j + 1) {
        free(colors[j]);
      }
      free(colors);
      free(arr);
      free(colorcount);
      freeComplexNumber(center);
      return 1;
    }

    fprintf(out, "%s %lu %lu %u\n", "P6", size, size, 255);
    for (int j = 0; j < size * size; j = j + 1) {
		if((arr[i][j] % (*colorcount)) == 0) {
			uint8_t *black = (uint8_t *) malloc(3 * sizeof(uint8_t));
			black[0] = 0;
			black[1] = 0;
			black[2] = 0;
			fwrite(black, sizeof(uint8_t), 3, out);
			free(black);
		} else {
			fwrite(colors[((arr[i][j]-1) % (*colorcount))], sizeof(uint8_t), 3, out);
		}
}	
    fclose(out);
  
}





	//STEP 4: Free all allocated memory
	/*
	Make sure there's no memory leak.
	*/
	//YOUR CODE HERE
  freeComplexNumber(center);
  for (int i = 0; i < *colorcount; i = i + 1) {
    free(colors[i]);}
  for (int i = 0; i < framecount; i = i + 1) {
    free(arr[i]);
  }
  free(colors);
  free(arr);
  free(colorcount);
  return 0;
}

