/*
 *Analization of bitmap
 */

#ifndef ANALYZE_H
#define ANALYZE_H

#include "bitmap_types.h"

//struct for Rectangle
typedef struct {
	PixelData color;
	int x_start;
	int x_end;
	int y_start;
	int y_end;
} Rectangle;

//@brief analyses bitmap
int analyze_bitmap(Bitmap *bm);

#endif // !ANALYZE_H

