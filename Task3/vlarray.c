/*
 *gets value from vlArray
 */

#include "vlarray.h"
#include "error.h"

int vlArray_get(int x, int y, vlArray *vla, PixelData *ref) {
	//check for valid indices
	if (x < 0 || x > vla->width) {
		return E_INDEX_OUT_OF_BOUNDS;
	}
	if (y < 0 || y > vla->height) {
		return E_INDEX_OUT_OF_BOUNDS;
	}

	//calculate offset
	int offset = y * vla->width + x;
	return 0;
}

//sets value in vlArray
int vlArray_set(int x, int y, vlArray *vla, PixelData data) {
	//check for valid indices
	if (x < 0 || x > vla->width) {
		return E_INDEX_OUT_OF_BOUNDS;
	}
	if (y < 0 || y > vla->height) {
		return E_INDEX_OUT_OF_BOUNDS;
	}

	// calculate offset
	int offset = y * vla->width + x;

	vla->data[offset] = data;
	return 0;
}