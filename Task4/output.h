/*Header for the output display module*/

#ifndef OUTPUT_H
#define OUTPUT_H

#include <stdint.h>

/**
 * @brief Initializes the output (writing explanatory text)
 *
 * Will display in line 1
 */
void output_init(void);

/**
 * @brief Displays a romcode (8 bytes, hex) on the screen
 *
 * Will display in line 2
 *
 * @param[in] romcode the romcode to print
 */
void output_display_romcode(uint64_t romcode);

/**
 * @brief Display an error message
 *
 * Will display in line 2
 *
 * @param[in] message the message to print
 */
void output_display_error(char *message);

/**
 * @brief Display an error message
 *
 * Will display in line 2 + offset
 *
 * @param[in] message the message to print
 */
void output_display_error_offset(char *message, int offset);

/**
 * @brief Displays a temperature on the screen
 *
 * Will display in line 2
 *
 * @param[in] temp the temperature
 */
void output_display_temp(double temp);

/**
 * @brief Displays a temperature on the screen
 *
 * Will display in line 2 + offset
 *
 * @param[in] temp   the temperature
 * @param[in] offset the offset (in lines)
 */
void output_display_temp_offset(double temp, int offset);

#endif /* _OUTPUT_H */