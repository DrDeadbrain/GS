/*
 *Implementation of the output module
 */

#pragma warning(disable:4996)

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "error.h"
#include "vlarray.h"
#include "output.h"

/*
 *Writes file to file system 
 */
int output_write_files(char* path, Bitmap* bitmap) {
	FILE *save = fopen(path, "r+b");
	if (save == NULL) {
		save = fopen(path, "w+b");
		if (save == NULL) {
			return E_FAILED_TO_WRITE;
		}
	}
	vlArray* image_data = &bitmap->imageData;

	//adjust fileHeader
	bitmap->fileHeader.bfOffBits = sizeof(BitmapFileHeader) + sizeof(BitmapInfoHeader);
	bitmap->fileHeader.bfSize = bitmap->fileHeader.bfOffBits + sizeof(PixelData) * image_data->width * image_data->height;

	//adjust InfoHeader
	bitmap->infoHeader.biBitCount = BITMAP_24BIT;
	bitmap->infoHeader.biCompression = COMPRESSION_NONE;

	//write fileHeader & infoHeader
	fwrite(&bitmap->fileHeader, sizeof(bitmap->fileHeader), 1, save);
	fwrite(&bitmap->infoHeader, sizeof(bitmap->infoHeader), 1, save);

	//readout vlArray and write pixelData to file
	for (int y = image_data->height - 1; y >= 0; y--) {
		for (int x = 0; x < image_data->width; x++) {
			PixelData p;
			if (vlArray_get(x, y, image_data, &p) != 0) {
				fclose(save);
				return E_INDEX_OUT_OF_BOUNDS;
			}
			fwrite(&p, sizeof(PixelData), 1, save);
		}
	}
	//fwrite(bitmap->imageData.data, sizeof(PixelData) * image_data->width * image_data->height, 1, save);

	fclose(save);
	return 0;
}