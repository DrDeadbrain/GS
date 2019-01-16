#ifndef hardware_IO_H
#define hardware_IO_H

/**
 * @brief Sets the pin to open-drain mode
 *
 * @param pin the pin to set
 */
void hwIO_set_pin_opendrain(int pin);

/**
 * @brief Sets the pin to push-pull mode
 *
 * @param pin the pin to set
 */
void hwIO_set_pin_pushpull(int pin);

/**
 * @brief Sets the pin to low (0V)
 *
 * @param pin the pin to set
 */
void hwIO_set_pin_low(int pin);

/**
 * @brief Sets the pin to high (3,3V)
 *
 * @param pin the pin to set
 */
void hwIO_set_pin_high(int pin);

/**
 * @brief Reads the pin
 *
 * @param pin the pin to read
 * @retval 0 pin is low
 * @retval 1 pin is high
 */
int hwIO_get_pin(int pin);

#endif /* HARDWARE_IO_H */

//EOF

