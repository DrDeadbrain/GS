/*
 *Analization of the bitmap
 */

#include <stdbool.h>
#include <stdio.h>
#include <math.h>
#include "vlarray.h"
#include "error.h"
#include "list.h"

#define MIN_SIZE 3

//color red
const PixelData RED = { 0, 0, 255 };

//color green
const PixelData GREEN = { 0, 255, 0 };

/*
*checks wheter two PixelData structs are equal
*
*@param[in] p1 PixelData 1
*@param[in] p2 PixelData 2
*@return wheter they are equal or not
*/
static bool equal(PixelData p1, PixelData p2) {
	return p1.red == p2.red && p1.green && p2.green && p1.blue == p2.blue;
}

/*
*checks wheter any of the potential rectangles can be added to the "real" rectangle list
*
*@param[in] current:  the current Rectangle
*@param[in, out] pot: the List of all potential rectangles 
*@param[in, out] act: the list of actual(real) rectangles
*@param[in] current_x: the current x value
*@param[in] current_y: the current y value
*/
void check_rects(Rectangle current, List *pot, List *act, int current_x, int current_y) {
	if (current.x_end - current.x_start >= MIN_SIZE) {
		//rectangle is big enough to count 
		//check potential rects:
		Iterator it = list_get_iterator(pot);
		Rectangle r; 
		bool already_there = false;
		while (iterator_has_next(&it)) {
			iterator_next(&it, &r);
			if (equal(r.color, current.color) && r.x_start == current.x_start && r.x_end == current.x_end) {
				//element is present, update y_end
				already_there = true;
				r.y_end = current_y;
				iterator_update(&it, r);
				break;
			}
			else {
				if (r.y_end != current_y && r.x_start < current_x - 1) {
					//the rectangle has passed
					if (r.y_end - r.y_start >= MIN_SIZE) {
						//rectangle big enough to count
						list_append(act, r);
					}
					iterator_remove(&it);
				}
			}
		}
		if (!already_there) {
			list_append(pot, current);
		}
	}
}

//analizes the bitmap
int analyze_bitmap(Bitmap *bm) {
	//create 2 Lists
	List rectangles;
	List *pRects = &rectangles;
	List potential_rectangles;
	List *pPot_Rects = &potential_rectangles;
	list_init(pRects);
	list_init(pPot_Rects);

	bool is_red = false;
	bool is_green = false;
	Rectangle rect;
	for (int y = 0; y < bm->imageData.height; y++) {
		for (int x = 0; x < bm->imageData.width; x++) {
			PixelData p;
			if (vlArray_get(x, y, &bm->imageData, &p) != 0) {
				printf("verkackt!\n");
				break;
			}

			//find red rectangles
			if (equal(p, RED) && !is_red) {
				is_red = true;
				rect.color = RED;
				rect.x_start = x;
				rect.y_start = y;
			} else if (is_red && !equal(p, RED)) {
				is_red = false;
				rect.x_end = x - 1;
				rect.y_end = y;

				check_rects(rect, pRects, pPot_Rects, x, y);
			}

			//find green rectangles
			if (equal(p, GREEN) && !is_green) {
				is_green = true;
				rect.color = GREEN;
				rect.x_start = x;
				rect.y_start = y;
			} else if (is_green && !equal(p, GREEN)) {
				is_green = false;
				rect.x_end = x - 1;
				rect.y_end = y;

				check_rects(rect, pRects, pPot_Rects, x, y);
			}
		}
		if (is_red) {
			is_red = false;
			rect.x_end = bm->imageData.width - 1;
			rect.y_end = y;

			check_rects(rect, pRects, pPot_Rects, bm->imageData.width - 1, y);
		}
		if (is_green) {
			is_green = false;
			rect.x_end = bm->imageData.width - 1;
			rect.y_end = y;

			check_rects(rect, pRects, pPot_Rects, bm->imageData.width - 1, y);
		}
		//finish all pending rectangles
		Iterator it = list_get_iterator(pPot_Rects);
		Rectangle r;
		while (iterator_has_next(&it)) {
			iterator_next(&it, &r);
			if (r.y_end - r.y_start >= MIN_SIZE) {
				//rectangle is big enough to count
				list_append(pRects, r);
			}
			iterator_remove(&it);
		}

		it = list_get_iterator(pRects);
		while (iterator_has_next(&it)) {
			iterator_next(&it, &r);
			char *color = equal(r.color, RED) ? "RED" : "GREEN";
			printf("Rectangle [%s from (%d|%d) to (%d|%d)]\n", color, r.x_start, r.y_start, r.x_end, r.y_end);
		}

		PixelData blue = {255, 0, 0};
		Rectangle border = {blue, bm->imageData.width - 1, 0, bm->imageData.height - 1, 0};

		it = list_get_iterator(pRects);
		while (iterator_has_next(&it)) {
			iterator_next(&it, &r);

			border.x_start = fmin(border.x_start, r.x_start);
			border.x_end = fmax(border.x_end, r.x_end);
			border.y_start = fmin(border.y_start, r.y_start);
			border.y_end = fmax(border.y_end, r.y_end);
		}

		printf("Border: (%d|%d) to (%d|%d)\n", border.x_start, border.y_start, border.x_end, border.y_end);

		//draw border
		for (int i = border.x_start; i <= border.x_end; i++) {
			if (vlArray_set(i, border.y_start, &bm->imageData, border.color)) {
				list_clear(pRects);
				list_clear(pPot_Rects);
				return E_INDEX_OUT_OF_BOUNDS;
			}
			if (vlArray_set(i, border.y_end, &bm->imageData, border.color)) {
				list_clear(pRects);
				list_clear(pPot_Rects);
				return E_INDEX_OUT_OF_BOUNDS;
			}
			if (vlArray_set(i, border.y_start, &bm->imageData, border.color)) {
				list_clear(pRects);
				list_clear(pPot_Rects);
				return E_INDEX_OUT_OF_BOUNDS;
			}
			if (vlArray_set(i, border.x_end, &bm->imageData, border.color)) {
				list_clear(pRects);
				list_clear(pPot_Rects);
				return E_INDEX_OUT_OF_BOUNDS;
			}
		}

		list_clear(pRects);
		list_clear(pPot_Rects);
		return 0;
	}
}
