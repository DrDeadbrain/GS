/** ****************************************************************
 * @file    aufgabe2/output.h
 * @author  Christian Caus (christiancaus@gmail.com
 * @version 1.0
 * @date    21.11.2018
 * @brief   Header for the Output Display module
 ******************************************************************
 */
#ifndef OUTPUT_H
#define OUTPUT_H

/**
 * @brief Updates the angle and speed of rotation on the screen.
 *
 * @param[in] angle the angle in degrees
 * @param[in] speed the speed in degrees per second
 */
void output_update_data(double angle, double speed);

/**
 * @brief Initializes the output (writing explanatory text)
 */
void output_init(void);

#endif /* _OUTPUT_H */



