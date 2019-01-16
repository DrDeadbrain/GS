/*
 *Implementation for the input module
 */

#define _CRT_SECURE_NO_DEPRECATE
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "input.h"
#include "error.h"
#include "vlarray.h"

#define DEBUG_RLE 0

/**
 *reads bitmap header from file
 *
 *@param[in] file: file to read from
 *@param[out] fileheader: pointer to where header should be stored
 *@param[out] infoheader: pointer to where info header should be stored
 @return error code
 @retval 0: no error
 @retval: E_FAILED_TO_READ: error reading from file
 */
static int read_header(FILE* file, BitmapFileHeader *fileHeader, BitmapInfoHeader *infoHeader) {
	size_t readSize;

	//read file header
	readSize = fread(fileHeader, sizeof(BitmapFileHeader), 1, file);
	if (readSize != 1) {
		return E_FAILED_TO_READ;
	}

	//read info Header
	readSize = fread(infoHeader, sizeof(BitmapInfoHeader), 1, file);
	if (readSize != 1) {
		return E_FAILED_TO_READ;
	}
	return 0;
}

/**
 *reads color table from file
 *
 *@param[in] file: file to read from
 *@return: pointer to where color table is stored on heap
 *@retval NULL: there was error 
 */
static ColorTableEntry* read_color_table(FILE* file) {
	size_t readSize;

	ColorTableEntry *colorTable = (ColorTableEntry*)calloc(COLOR_TABLE_SIZE, sizeof(ColorTableEntry));
	if (colorTable == NULL) {
		return NULL;
	}

	//read Table
	readSize = fread(colorTable, sizeof(ColorTableEntry), COLOR_TABLE_SIZE, file);
	if (readSize != COLOR_TABLE_SIZE) {
		free(colorTable);
		return NULL;
	}
	return colorTable;
}

/**
* reads pixel data from a file, assuming 24bit per pixel
*
* @param[in]      file       the file to read from
* @param[in, out] image_data pointer to a structure containing width and
*                            height of the image. Will contain pointer to
*                            where pixel data is stored on the heap when done
* @return error code
* @retval 0                     no error
* @retval E_FAILED_TO_READ      there was an error reading from the file
* @retval E_NO_MEMORY           couldn't allocate sufficient memory
* @retval E_INDEX_OUT_OF_BOUNDS tried to write to invalid indices in VLA
*/
static int read_pixels_24bit(FILE* file, vlArray* image_data) {
	size_t readSize;
	PixelData pData;

	// allocate memory for all pixels
	PixelData *data = (PixelData*)calloc(
		image_data->width * image_data->height, sizeof(PixelData));
	if (data == NULL) {
		return E_NO_MEMORY;
	}

	image_data->data = data;

	// Read image
	for (int y = image_data->height - 1; y >= 0; y--) {
		for (int x = 0; x < image_data->width; x++) {
			readSize = fread(&pData, sizeof(pData), 1, file);
			if (readSize != 1) {
				free(data);
				return E_FAILED_TO_READ;
			}

			// write to VLA
			if (vlArray_set(x, y, image_data, pData) != 0) {
				free(data);
				return E_INDEX_OUT_OF_BOUNDS;
			}
		}
	}

	return 0;
}

/**
* reads pixel data from a file, assuming 8bit per pixel
*
* @param[in]      file        the file to read from
* @param[in]      color_table the color table for converting from color index to actual color
* @param[in, out] image_data  pointer to a structure containing width and
*                             height of the image. Will contain pointer to
*                             where pixel data is stored on the heap when done
* @return error code
* @retval 0                     no error
* @retval E_FAILED_TO_READ      there was an error reading from the file
* @retval E_NO_MEMORY           couldn't allocate sufficient memory
* @retval E_INDEX_OUT_OF_BOUNDS tried to write to invalid indices in VLA
*/
static int read_pixels_8bit(FILE* file, ColorTableEntry* color_Table, vlArray* image_data) {
	size_t readSize;
	unsigned char index;

	//allocate memory for all pixels
	PixelData *data = (PixelData*)calloc(
		image_data->width * image_data->height, sizeof(PixelData));
	if (data == NULL) {
		return E_NO_MEMORY;
	}

	image_data->data = data;

	//read Image
	for (int y = image_data->height - 1; y >= 0; y--) {
		for (int x = 0; x < image_data->width; x++) {
			readSize = fread(&index, sizeof(index), 1, file);
			if (readSize != 1) {
				free(data);
				return E_FAILED_TO_READ;
			}
			//create color from pixelData
			ColorTableEntry c = color_Table[index];
			PixelData p = { c.blue, c.green, c.red };

			if (vlArray_set(x, y, image_data, p) != 0) {
				free(data);
				return E_INDEX_OUT_OF_BOUNDS;
			}
		}
	}
	return 0;
}

/**
* reads two bytes from a file
*
* @param[out] b1   pointer to the first byte to read
* @param[out] b2   pointer to the second byte to read
* @param[in]  file the file to read from
* @return error code
* @retval 0                no error
* @retval E_FAILED_TO_READ there was an error reading from the file
*/
static int read_two_bytes(unsigned char *b1, unsigned char *b2, FILE* file) {
	size_t readSize;
	readSize = fread(b1, sizeof(*b1), 1, file);
	if (readSize != 1) {
		return E_FAILED_TO_READ;
	}

	readSize = fread(b2, sizeof(*b2), 1, file);
	if (readSize != 1) {
		return E_FAILED_TO_READ;
	}
	return 0;
}

/**
* reads pixel data from a file, assuming 8 bit per pixel compressed with RLE encoding
*
* @param[in]      file        the file to read from
* @param[in]      color_table the color table for converting from color index to actual color
* @param[in, out] image_data  pointer to a structure containing width and
*                             height of the image. Will contain pointer to
*                             where pixel data is stored on the heap when done
* @return error code
* @retval 0                     no error
* @retval E_FAILED_TO_READ      there was an error reading from the file
* @retval E_NO_MEMORY           couldn't allocate sufficient memory
* @retval E_INDEX_OUT_OF_BOUNDS tried to write to invalid indices in VLA
*/
static int read_pixels_8bit_rle(FILE* file, ColorTableEntry* color_table, vlArray* image_data) {
	//allocate memory for all pixels
	PixelData *data = (PixelData*)calloc(
		image_data->width * image_data->height, sizeof(PixelData));
	if (data == NULL) {
		return E_NO_MEMORY;
	}
	image_data->data = data;

	unsigned char b1, b2;
	int current_x = 0;
	int current_y = image_data->height - 1;//image stored "upside down
	size_t readSize;

	while (1) {
		//read two bytes from Image
		if (read_two_bytes(&b1, &b2, file)) {
			free(data);
			return E_FAILED_TO_READ;
		}
#if DEBUG_RLE
		printf("%02x %02x", b1, b2);
#endif // DEBUG_RLE

		if (b1 == 0) {
			//control sequence
			if (b2 == 0) {
				//EOL
#if DEBUG_RLE
				printf("\n");
#endif //DEBUG_RLE

				current_y--;
				current_x = 0;
			}
			else if (b2 == 1) {
				//EOF
				break;
			}
			else if (b2 == 2) {
				//delta
				//DEBUG CODE!
				printf("\nDELTA found:)\n");
				//DEBUG CODE!

				//Read next two bytes
				if (read_two_bytes(b1, &b2, file)) {
					free(data);
					return E_FAILED_TO_READ;
				}
				current_x += b1;
				current_y += b2;
			}
			else {
				//absolute mode
#if DEBUG_RLE
				printf("ABS [");
#endif // DEBUG_RLE

				//b2 has number of pixels in absolute mode 
				for (int i = 0; i < b2; i++) {
					//read next byte
					readSize = fread(&b1, sizeof(b1), 1, file);
					if (readSize != 1) {
						free(data);
						return E_FAILED_TO_READ;
					}
#if DEBUG_RLE
					printf(" %02x", b1);
#endif // DEBUG_RLE

					//get color and write it to vlArray
					ColorTableEntry c = color_table[b1];
					PixelData p = { c.blue, c.green, c.red };

					if (vlArray_set(current_x, current_y, image_data, p) != 0) {
						free(data);
						return E_INDEX_OUT_OF_BOUNDS;
					}
					//increment
					current_x++;
				}
#if DEBUG_RLE
				printf(" ] ");
#endif // DEBUG_RLE

				int align = b2 % WORD_SIZE;
				if (align != 0) {
					//need to throw away some bits for aligning
					align = WORD_SIZE - align;
#if DEBUG_RLE
					printf("IGNORING %d FOR ALIGN ", align);
#endif // DEBUG_RLE
					if (fseek(file, align, SEEK_CUR) != 0) {
						free(data);
						return E_FAILED_TO_READ;
					}
				}
			}
		}
		else { /* if (b1 == 0) */
			//normal RLE mode
			//b1 is amount, b2 is color index
			ColorTableEntry c = color_table[b2];
			PixelData p = { c.blue, c.green, c.red };

			for (int i = 0; i < b1; i++) {
				//write pixel b1 times
				if (vlArray_set(current_x, current_y, image_data, p) != 0) {
					free(data);
					return E_INDEX_OUT_OF_BOUNDS;
				}
				current_x++;
			}
		}
	}
	return 0;
}

/**
* checks header fields for validity
*
* @param[in] fileHeader the bitmap file header
* @param[in] infoHeader the buitmap info header
* @retval true  bitmap is valid
* @retval false bitmap is invalid
*/
static bool is_valid(BitmapFileHeader *fileHeader, BitmapInfoHeader *infoHeader) {
	//check file header
	//type must be BM (ASCII)
	if (fileHeader->bfType != BF_TYPE) {
		return false;
	}

	//reserved must be 0
	if (fileHeader->bfReserved1 != 0 || fileHeader->bfReserved2 != 0) {
		return false;
	}
	//check info header 
	//info header size must be 40
	if (infoHeader->biSize != INFO_HEADER_SIZE) {
		return false;
	}
	//images width and height must be positive
	if (infoHeader->biWidth < 0 || infoHeader->biHeight < 0) {
		return false;
	}
	//planes must be one
	if (infoHeader->biPlanes != 1) {
		return false;
	}
	//bit count must be 9 or 24
	if (infoHeader->biBitCount != BITMAP_8BIT && infoHeader->biBitCount != BITMAP_24BIT) {
		return false;
	}
	//compression must be 0 or 1
	if (infoHeader->biCompression != COMPRESSION_RLE && infoHeader->biCompression != COMPRESSION_NONE) {
		return false;
	}
	//color used and color important must be 0
	if (infoHeader->biClrUsed != 0 || infoHeader->biClrImportant != 0) {
		return false;
	}
	//you made it till here ? Well congrats mate
	return true;
}

/*
 * Reads file and writes data to bitmap
 */
int input_read_file(char* path, Bitmap* bitmap) {
	FILE* bmpFile;

	//open file
	bmpFile = fopen(path, "rb");
	if (bmpFile == NULL) {
		return E_FAILED_TO_READ;
	}
	//read Headers
	BitmapFileHeader fileHeader;
	BitmapInfoHeader infoHeader;
	int res;
	res = read_header(bmpFile, &fileHeader, &infoHeader);
	if (res != 0) {
		fclose(bmpFile);
		return E_FAILED_TO_READ;
	}
	//Check integrity
	if (!is_valid(&fileHeader, &infoHeader)) {
		fclose(bmpFile);
		return E_INVALID_BMP;
	}

	//Debug code!
	bitmap->fileHeader = fileHeader;
	bitmap->infoHeader = infoHeader;
	//debug code

	//create structure for image data
	vlArray image_data;
	image_data.width = infoHeader.biWidth;
	image_data.height = infoHeader.biHeight;

	//check type for choosing the correct read() method
	if (infoHeader.biBitCount == 8) {
		//256 color Bitmap (8 Bit)

		//read color table
		ColorTableEntry *color_table;
		color_table = read_color_table(bmpFile);
		if (color_table == NULL) {
			fclose(bmpFile);
			return E_FAILED_TO_READ;
		}

		if (infoHeader.biCompression == 0) {
			//Bitmap is uncompressed 
			res = read_pixels_8bit(bmpFile, color_table, &image_data);
		}
		else {
			//Bitmap is compressed
			res = read_pixels_8bit_rle(bmpFile, color_table, &image_data);
		}
		//free colorTable
		free(color_table);

		//check wheter reading worked
		if (res != 0) {
			fclose(bmpFile);
			return res;
		}
	}
	else {
		//24 bit bitmap
		res = read_pixels_24bit(bmpFile, &image_data);
		if (res != 0) {
			fclose(bmpFile);
			return res;
		}
	}
	//everything worked-> clean up
	fclose(bmpFile);

	//copy structures to return them
	bitmap->fileHeader = fileHeader;
	bitmap->infoHeader = infoHeader;
	bitmap->imageData = image_data;
	return 0;
}
