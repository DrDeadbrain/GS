/*
 *header for vla module
 */

#ifndef _vlArray_H
#define _vlArray_H

#include "bitmap_types.h"

/**
 * gets value from vlarray
 *
 * @param x the x position
 * @param y the y postition
 * @param vla the VLA to read from
 * @param ref pointer to where the result should be stored
 * @return error code
 * @retval 0 no error
 * @retval E_INDEX_OUT_OF_BOUNDS the index (x/y) is out of bounds
 */
int vlArray_get(int x, int y, vlArray *vla, PixelData *ref);

/**
 * Sets a value in the VLA
 *
 * @param x the x position
 * @param y the y position
 * @param vla the VLA to read from
 * @param data the data to insert
 * @return error code
 * @retval 0 no error
 * @retval E_INDEX_OUT_OF_BOUNDS the index (x/y) is out of bounds
 */
int vlArray_set(int x, int y, vlArray *vla, PixelData data);
























#endif // !_vlArray_H

