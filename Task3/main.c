
 //Main program (starting point)

#define _CRT_SECURE_NO_DEPRECATE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "input.h"
#include "output.h"
#include "vlarray.h"
#include "error.h"
#include "analyze.h"





 // I want Pixeldata output
#define DEBUG_PRINT_PIXELDATA 0

 /**
 * program entry
 *
 * @return exit code
 */
int main(void) {
	Bitmap bm;
	int res;

	char* path = "C:\Users\Christian\Desktop\Uni\Uni_neu\GS\Praktikum\Aufgabe3\testBilder\\";

	// TestBilder
	char* filename = "6x10_8_bit_nicht_komprimiert_mit_padding_bytes.bmp";
	//char* filename = "22x14_8_bit_komprimiert_mit_padding_bytes_sehr_kleines_testbild.bmp";
	//char* filename = "22x14_8_bit_nicht_komprimiert_mit_padding_bytes_sehr_kleines_testbild.bmp"; 
	//char* filename = "60x50_8_bit_komprimiert_ohne_padding_bytes_kleines_testbild.bmp";
	//char* filename = "60x50_8_bit_nicht_komprimiert_ohne_padding_bytes_kleines_testbild.bmp";
	//char* filename = "128x480_8_bit_komprimiert_ohne_padding_bytes_zeile_zu_lang.bmp";
	//char* filename = "640x480_8_bit_komprimiert_ohne_padding_bytes_amrandrle.bmp";
	//char* filename = "640x480_8_bit_komprimiert_ohne_padding_bytes_einfaches_testbild.bmp";
	//char* filename = "640x480_8_bit_komprimiert_ohne_padding_bytes_mehr_farben.bmp"; 
	//char* filename = "640x480_8_bit_komprimiet_ohne_padding_bytes_eine_zeile_zu_lang_amrandrleMod.bmp";
	//char* filename = "640x480_8_bit_komprimiet_ohne_padding_bytes_rechtecke_am_bildrand.bmp"; 
	//char* filename = "640x480_8_bit_komprimiet_ohne_padding_bytes_rotes_rechteck_fehlt_pixel_am_rand.bmp";
	//char* filename = "640x480_8_bit_nicht_komprimiert_ohne_padding_bytes_einfaches_testbild.bmp";
	//char* filename = "640x480_24_bit_nicht_komprimiert_ohne_padding_bytes_blauer_rahmen.bmp";
	//char* filename = "640x480_IM_HEADER_biWidth_viel_zu_goss_8_bit_komprimiet_ohne_padding_bytes.bmp";
	//char* filename = "640x480_IM_HEADER_biWidth_zu_goss_8_bit_komprimiet_ohne_padding_bytes.bmp";
	//char* filename = "720_400_8_bit_komprimiert_ohne_padding_bytes.bmp";
	//char* filename = "BildSignaturDefekt.bmp";

	//superloop
	while (1) {
		printf("Please select a File:\n");
		printf("[1] amrandrle.bmp              [2] amrandrle_NO_BLUE_LINES.bmp \n");
		printf("[3] amrandrleMod.bmp           [4] anderegroesse.bmp \n");
		printf("[5] aufgabe3_bild1.bmp         [6] aufgabe3_bild2.bmp \n");
		printf("[7] rotmitlochamrand.bmp       [8] rotmitlochrle.bmp \n");
		printf("[9] rotmitlochrlemod1.bmp      [10] rotmitlochrlemod2.bmp \n");
		printf("[11] rotmitlochrlemod3.bmp     [12] rotmitlochrlemod4.bmp \n");
		printf("[13] rotmitlochrleundpunkt.bmp \n");

		printf("File with Index: ");
		int menuIndex;
		scanf_s("%i", &menuIndex);
		fflush(stdin);
		char * filename;

		switch (menuIndex) {
		case 1: filename = "6x10_8_bit_nicht_komprimiert_mit_padding_bytes.bmp";
			break;
		case 2: filename = "anrandrle_NO_BLUE_LINES.bmp";
			break;
		case 3: filename = "amrandrleMod.bmp";
			break;
		case 4: filename = "anderegroesse.bmp";
			break;
		case 5: filename = "aufgabe3_bild1.bmp";
			break;
		case 6: filename = "aufgabe3_bild2.bmp";
			break;
		case 7: filename = "rotmitlochamrand.bmp";
			break;
		case 8: filename = "rotmitlochrle.bmp";
			break;
		case 9: filename = "rotmitlochrlemod1.bmp";
			break;
		case 10: filename = "rotmitlochrlemod2.bmp";
			break;
		case 11: filename = "rotmitlochrlemod3.bmp";
			break;
		case 12: filename = "rotmitlochrlemod4.bmp";
			break;
		case 13: filename = "rotmitlochrleundpunkt.bmp";
			break;

		default: return EXIT_SUCCESS;
		}

		char filepath[200] = { 0 };
		strcat(filepath, path);
		strcat(filepath, filename);

		res = input_read_file(filepath, &bm);

		printf("W=%d, H=%d\n", bm.infoHeader.biWidth, bm.infoHeader.biHeight);
		printf("Mode=%d, Compression=%d\n", bm.infoHeader.biBitCount, bm.infoHeader.biCompression);

		if (res == 0) {
#if DEBUG_PRINT_PIXELDATA
			printf("\nPixel Data\n");
			for (int y = 0; y < bm.imageData.width; y++) {
				for (int x = 0; x < bm.imageData.width; x++) {
					PixelData p;
					if (vlArray.get(x, y, &bm.imageData, &p) != 0) {
						printf("You failed!\n");
						break;
					}
					printf("(%3d %3d %3d)", p.red, p.green, p.blue);
				}
				printf("\n");
			}
#endif // DEBUG_PRINT_PIXELDATA
			printf("Analyse:\n");
			analyze_bitmap(&bm);

			char saveFilePath[200] = { 0 };
			strcat(saveFilePath, path);
			strcat(saveFilePath, "saved_");
			strcat(saveFilePath, filename);

			res = output_write_files(saveFilePath, &bm);

			if (res == E_FAILED_TO_WRITE) {
				printf("Failed to write");
			}
			else if (res == E_INDEX_OUT_OF_BOUNDS) {
				printf("Index out of bounds");
			}
			free(bm.imageData.data);
		}
		else {
			if (res == E_INVALID_BMP) {
				printf("Bitmap Invalid");
			}
			else if (res == E_FAILED_TO_READ) {
				printf("Failed to read");
				perror("cannot be read");
			}
			else if (res == E_INDEX_OUT_OF_BOUNDS) {
				printf("Index otu of bounds");
			}
			else if (res == E_NO_MEMORY) {
				printf("Out of memory");
			}
			else {
				printf("Magic Failure");
			}
		}
		printf("\n------------------------------------------------------------\n\n");
	}
	return EXIT_SUCCESS;
}