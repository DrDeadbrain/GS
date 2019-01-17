/*Header for hardware IO module*/

#ifndef HARDWARE_IO_H
#define HAL_H

#include <stdbool.h>
#include "rotary_encoder.h"

/**
 * @brief Reads channel A from encoder
 *
 * @retval true channel is high
 * @retval false channel is low
 */
bool io_get_encoder_channel_A(void);

/**
 * @brief Reads channel B from encoder
*
 * @retval true channel is high
 * @retval false channel is low
 */
bool io_get_encoder_channel_B(void);

/**
 * @brief Displays data on LEDs
 *
 * @param[in] number an 8 bit number to be displayed on LEDS D21-D28
 * @param[in] dir displays the direction on LEDS D20 / D19
 */
void io_display_data(char number, Direction dir);


/**
 * @brief Displays an error by illuminating D18
 */
void io_show_error(void);

/**
 * @brief Clears the error LED D18
 */
void io_clear_error(void);

/**
 * @brief Checks reset switches S7 and S6
 * 
 * @return true when either is pressed
 */
bool io_is_reset_pressed(void);

/**
 * 
 */
void hwio_init_interupts( void );

#endif /* HARDWARE_IO_H */