/*********************
**  Color Map generator
** Skeleton by Justin Yokota
**********************/

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>
#include <string.h>
#include "ColorMapInput.h"


/**************
**This function reads in a file name colorfile.
**It then uses the information in colorfile to create a color array, with each color represented by an int[3].
***************/
uint8_t** FileToColorMap(char* colorfile, int* colorcount)
{
	//YOUR CODE HERE
  FILE* ptr = fopen(colorfile, "r");
  if (ptr == NULL) {
    fclose(ptr);
    return NULL;
  }
  if (fscanf(ptr, "%d", colorcount) != 1 || (*colorcount) < 1) {
    fclose(ptr);
    return NULL;
  }
  uint8_t **arr = (uint8_t**) malloc(*colorcount * sizeof(uint8_t*));
  // if (arr == NULL) {
  //   fclose(ptr);
  //   return NULL;
  // }
  // int r, g, b;
  for (int i = 0; i < (*colorcount); i = i + 1) {
    arr[i] = (uint8_t*) malloc(3 * sizeof(uint8_t));
    if(arr[i] == NULL || fscanf(ptr, "%d %d %d", &arr[i][0], &arr[i][1], &arr[i][2]) != 3
    || arr[i][0] < 0 || arr[i][0] > 255 || arr[i][1] < 0
    || arr[i][1] > 255 || arr[i][2] < 0 || arr[i][2] > 255) {
      for (int j = i; j >= 0; j = j - 1) {
        free(arr[j]);
      }
      free(arr);
      fclose(ptr);
      return NULL;
    }
    // arr[i][0] = r;
		// arr[i][1] = g;
		// arr[i][2] = b;
  }
  fclose(ptr);
  return arr;
}
