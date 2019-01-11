/*
 *Typedef for BMPs
 */
#include <stdio.h>
#include "packed.h"

#ifndef _BITMAP_TYPES_H
#define _BITMAP_TYPES_H

#include "ms_types.h"


//size of color table in bitmap
#define COLOR_TABLE_SIZE 256

//type BM for file header
#define BF_TYPE 0x4d42

//info header size
#define INFO_HEADER_SIZE 40

//bit count for 8 bit
#define BITMAP_8BIT 8

//Bit count for 24 Bit
#define BITMAP_24BIT 24

//Info Header Bi Compression true
#define COMPRESSION_RLE 1

//Info Header Bi Compression false
#define COMPRESSION_NONE 0

//size of WORD for aligning in RLE absolute mode
#define WORD_SIZE 2

//@brief file header of bitmap
typedef struct {
	WORD bfType;
	DWORD bfSize;
	WORD bfReserved1;
	WORD bfReserved2;
	DWORD bfOffBits;
} BitmapFileHeader;

//@brief info header of the bitmap
PACKED( 
	typedef struct  {
		DWORD biSize;
		LONG biWidth;
		LONG biHeight;
		WORD biPlanes;
		WORD biBitCount;
		DWORD biCompression;
		DWORD biSizeImage;
		LONG biXPelsPerMeter;
		LONG biYPelsPerMeter;
		DWORD biClrUsed;
		DWORD biClrImportant;
} BitmapInfoHeader;
)


//@brief data (RGB values) of a single pixel
PACKED (
	typedef struct {
	unsigned char blue;
	unsigned char green;
	unsigned char red;
} PixelData;
)

//@brief wraps pointer to memory containing data and array size
typedef struct {
	int width;
	int height;
	PixelData* data;
} vlArray;

//representation of a bitmap
typedef struct {
	BitmapFileHeader fileHeader;
	BitmapInfoHeader infoHeader;
	vlArray imageData;
} Bitmap;

//@brief data in color table
PACKED(
	typedef struct {
	unsigned char blue;
	unsigned char green;
	unsigned char red;
	unsigned char reserved;
} ColorTableEntry;
)

#endif // !_BITMAP_TYPES_H

