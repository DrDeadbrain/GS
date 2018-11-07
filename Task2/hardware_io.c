/** ********************************************************************
 *@file 				aufgabe2/hardware_io.c
 *@author 	Christian Caus (christiancaus@gmail.com)
 *@version	1.0
 *@date 			06.11.2018
 *@brief 			Implementation of the Hardware IO Module
 **********************************************************************
*/

#include "TI_Lib.h"
#include "error.h"
#include "hardware_io.h"
#include <stdbool.h>

/**
	* Creates a mask for a pin, assuming 1 bit per pin
*/
#define MASK_ONE_BIT_PIN(i) (0x01U << (i))

// Pin Configuration section
/**
	*the pin numer where rotary_ecoder channel A is connected to 
*/

#define ENCODER_CHANNEL_A  0

/**
	*the pin numer where rotary_ecoder channel A is connected to 
*/

#define ENCODER_CHANNEL_B 1

/**
	*the pin numer of the LED used to indicate a error
*/

#define ERROR_LED 5

/**
	*the pin numer of the LED used to indicate a backward rotation
*/
#define BACKWARD_LED 6

/**
	*the pin numer of the LED used to indicate a forward rotation
*/

#define FORWARD_LED 7


//Function section ----------------------------------------------------------------------------------
/**
	*@brief Reads a single input from the board
	*
	*@param [in] GPIOx the port to read from
	*@param [in] pin the pin number
	*
	*@return wheter the pin is high
*/
	static bool is_Pin_High(GPIO_TypeDef* GPIOx , int pin) {
		return (GPIOx->IDR & MASK_ONE_BIT_PIN(pin)) == MASK_ONE_BIT_PIN(pin); 
	}
	
/** 
	*Reads Channel A from encoder
*/
	bool get_encoder_channel_A(void) {
		return is_Pin_High(GPIOE, ENCODER_CHANNEL_A);
	}
	
/** 
	*Reads Channel B from encoder
*/
	bool get_encoder_channel_B(void) {
		return is_Pin_High(GPIOE, ENCODER_CHANNEL_B);
	}
	
/** 
	*Function that displays data on LEDs
	*/
	
	void display_data(
	
	
	