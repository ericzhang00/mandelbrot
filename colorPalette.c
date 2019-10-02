/*********************
**  Color Palette generator
** Skeleton by Justin Yokota
**********************/

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>
#include <string.h>
#include "ColorMapInput.h"

//You don't need to call this function but it helps you understand how the arguments are passed in
void usage(char* argv[])
{
	printf("Incorrect usage: Expected arguments are %s <inputfile> <outputfolder> <width> <heightpercolor>", argv[0]);
}

//Creates a color palette image for the given colorfile in outputfile. Width and heightpercolor dictates the dimensions of each color. Output should be in P3 format
int P3colorpalette(char* colorfile, int width, int heightpercolor, char* outputfile)
{
	//YOUR CODE HERE
	int *colorcount = (int *) malloc(sizeof(int));
	// if (colorcount == NULL) {
	// 	free(colorcount);
	// 	return 1;
	// }
	uint8_t **arr = FileToColorMap(colorfile, colorcount);
	if (width < 1 || heightpercolor < 1 || arr == NULL) {
		free(colorcount);
		free(arr);
		return 1;
	}
	FILE* out = fopen(outputfile, "w");
	fprintf(out, "%s %d %d %u", "P3", width, (*colorcount) * heightpercolor, 255);
	for (int color = 0; color < (*colorcount); color = color + 1) {
		for (int colorrow = 0; colorrow < heightpercolor; colorrow = colorrow + 1) {
			fprintf(out, "\n");
			for (int x = 0; x < width - 1; x = x + 1) {
				fprintf(out, "%u %u %u ", arr[color][0], arr[color][1], arr[color][2]);
			}
			fprintf(out, "%u %u %u", arr[color][0], arr[color][1], arr[color][2]);
		}
	}
	for(int i = 0; i < (*colorcount); i = i + 1) {
		free(arr[i]);
	}
	free(arr);
	free(colorcount);
	fclose(out);
	return 0;
}

//Same as above, but with P6 format
int P6colorpalette(char* colorfile, int width, int heightpercolor, char* outputfile)
{
	//YOUR CODE HERE
	int *colorcount = (int *) malloc(sizeof(int));
	if (colorcount == NULL) {
		free(colorcount);
		return 1;
	}
	uint8_t **arr = FileToColorMap(colorfile, colorcount);
	if (width < 1 || heightpercolor < 1 || arr == NULL) {
		free(colorcount);
		free(arr);
		return 1;
	}
	FILE* out = fopen(outputfile, "w");
	fprintf(out, "%s %d %d %u\n", "P6", width, *colorcount * heightpercolor, 255);
	for (int color = 0; color < *colorcount; color = color + 1) {
		for (int pixel = 0; pixel < width * heightpercolor; pixel = pixel + 1) {
			fwrite(arr[color], sizeof(uint8_t), 3, out);
		}
	}
	for(int i = 0; i < (*colorcount); i = i + 1) {
		free(arr[i]);
	}
	free(colorcount);
	free(arr);
	fclose(out);
	return 0;
}


//The one piece of c code you don't have to read or understand. Still, might as well read it, if you have time.
int main(int argc, char* argv[])
{
	if (argc != 5)
	{
		usage(argv);
		return 1;
	}
	int width = atoi(argv[3]);
	int height = atoi(argv[4]);
	char* P3end = "/colorpaletteP3.ppm";
	char* P6end = "/colorpaletteP6.ppm";
	char buffer[strlen(argv[2]) + strlen(P3end)];
	sprintf(buffer, "%s%s", argv[2], P3end);
	int failed = P3colorpalette(argv[1], width, height, buffer);
	if (failed)
	{
		printf("Error in making P3colorpalette");
		return 1;
	}
	sprintf(buffer, "%s%s", argv[2], P6end);
	failed = P6colorpalette(argv[1], width, height, buffer);
	if (failed)
	{
		printf("Error in making P6colorpalette");
		return 1;
	}
	printf("Success, stored in %s, %s", P3end, P6end);
	return 0;
}
