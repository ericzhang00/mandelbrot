/*********************
**  Mandelbrot fractal
** clang -Xpreprocessor -fopenmp -lomp -o Mandelbrot Mandelbrot.c
** by Dan Garcia <ddgarcia@cs.berkeley.edu>
** Modified for this class by Justin Yokota and Chenyu Shi
**********************/

#include <stdio.h>
#include <stdlib.h>
#include "ComplexNumber.h"
#include "Mandelbrot.h"
#include <sys/types.h>

/*
This function returns the number of iterations before the initial point >= the threshold.
If the threshold is not exceeded after maxiters, the function returns 0.
*/
u_int64_t MandelbrotIterations(u_int64_t maxiters, ComplexNumber * point, double threshold)
{
    //YOUR CODE HERE
  u_int64_t iterations = 0;
  ComplexNumber* z = newComplexNumber(0, 0);
  ComplexNumber* c = newComplexNumber(Re(point), Im(point));
  while (iterations < maxiters) {
    if (ComplexAbs(z) >= threshold) {
      freeComplexNumber(c);
      freeComplexNumber(z);
      return iterations;
    }
    iterations = iterations + 1;
    ComplexNumber* temp = ComplexProduct(z, z);
    freeComplexNumber(z);
    z = ComplexSum(temp, c);
    freeComplexNumber(temp);
  }
  freeComplexNumber(c);
  freeComplexNumber(z);
  return 0;
}

/*
This function calculates the Mandelbrot plot and stores the result in output.
The number of pixels in the image is resolution * 2 + 1 in one row/column. It's a square image.
Scale is the the distance between center and the top pixel in one dimension.
*/
void Mandelbrot(double threshold, u_int64_t max_iterations, ComplexNumber* center, double scale, u_int64_t resolution, u_int64_t * output){
    //YOUR CODE HERE
    int offset = 0;
    double increase = scale / resolution;
    for (int row = 0; row < resolution * 2 + 1; row = row + 1) {
      for (int col = 0; col < resolution * 2 + 1; col = col + 1) {
        ComplexNumber* curr = newComplexNumber(Re(center) - scale + col * increase, Im(center) + scale - row * increase);
        output[offset] = MandelbrotIterations(max_iterations, curr, threshold);
        freeComplexNumber(curr);
        offset = offset + 1;
      }
    }

}
