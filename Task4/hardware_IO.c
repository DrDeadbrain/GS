/*Implementation of the Hardware Abstraction Module*/

#include "hardware_IO.h"
#include "TI_Lib.h"
#include "error.h"
#include <stdbool.h>

/*port to use*/
#define PORT GPIOG

/*Creates a mask for a pin, assuming 1 bit per pin*/
#define MASK_ONE_BIT_PIN(i)    (1 << (i))

//Functions

/**
 * @brief Reads a single input from the board
 *
 * @param[in] port the port to read from
 * @param[in] pin the pin number
 * 
 * @return whether the pin is high
 */
 static bool is_pin_high(GPIO_TypeDef *port, int pin) {
			return (port->IDR & MASK_ONE_BIT_PIN(pin)) == MASK_ONE_BIT_PIN(pin);
 }
 
 /*sets pin to open drain mode*/
 void hwIO_set_pin_opendrain(int pin) {
			PORT->OTYPER |= MASK_ONE_BIT_PIN(pin);
 }
 
 /*sets pin to push-pull mode*/
 void hwIO_set_pin_pushpull(int pin) {
	 PORT->OTYPER &= ~MASK_ONE_BIT_PIN(pin);
 }
 
 /*Sets pin to low*/
 void hwIO_set_pin_low(int pin) {
			PORT->BSRRH = MASK_ONE_BIT_PIN(pin);
 }
 
 /*sets pin to high*/
 void hwIO_set_pin_high(int pin) {
			PORT->BSRRL = MASK_ONE_BIT_PIN(pin);
 }
 
 /*reads the pin*/
 int hwIO_get_pin(int pin) {
		return is_pin_high(PORT, pin) ? 1: 0;
 }