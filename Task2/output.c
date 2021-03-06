/** ****************************************************************
 * @file    aufgabe2/output.c
 * @author  Christian Caus (christiancaus@gmail.com)
 * @version 1.0
 * @date    21.11.2018
 * @brief   Implementation of the Output Display module
 ******************************************************************
 */
#include <string.h>
#include <stdio.h>
#include "tft.h"
#include "output.h"

#define BUFFER_SIZE 40

#define START_X 2
#define DATA_X 19

#define LINE_1_Y 2
#define LINE_2_Y 3

/* 
 * Updates the angle and speed of rotation on the screen.
 */
void output_update_data(double angle, double speed) {
    char buffer[BUFFER_SIZE + 1] = { 0 };

    // write angle to string
    sprintf(buffer, "%6.1f", angle);
    TFT_gotoxy(DATA_X, LINE_1_Y);
    TFT_puts(buffer);

    //write speed to string
    sprintf(buffer, "%6.2f", speed);
    TFT_gotoxy(DATA_X, LINE_2_Y);
    TFT_puts(buffer);
}

/* 
 * Initializes the output (writing explanatory text
 */
void output_init() {
    TFT_gotoxy(START_X, LINE_1_Y);
    TFT_puts("Winkel:                 Grad");

    TFT_gotoxy(START_X, LINE_2_Y);
    TFT_puts("Geschwindigkeit:        Grad/s");
}

