/** ****************************************************************
 * @file    aufgabe2/rotary_encoder.h
 * @author  Christian Caus(christiancaus@gmail.com)
 * @version 1.0
 * @date    07.11.2018
 * @brief   Header for the Rotary Encoder module
 ******************************************************************
 */

#ifndef ROTARY_ENCODER_H
#define ROTARY_ENCODER_H
/**
	*@biref enum of possible rotation directions
*/
	typedef enum {
			FORWARD, //!<encoder is turning forwards
			BACKWARD, //!< encoder is turning backwards
			UNKNOWN //!< encoder is stationary or direction is unknown
	} Direction;
	
/**
	* @brief returns direction of encoder
	*
	* @return direction
 */
	Direction encoder_get_direction(void);

/**
	* pos is relative to last reset, negative values indicate that
	* encoder turned backwards
	*
	* @return position in 0.1 degrees
 */
	int encoder_get_position(void);

/**
	* pos is relative to last reset, negative values indicate that 
	* encoder turned backwards
	*
	* @return position in pulses
 */
	int encoder_get_position_raw(void);

/**
 * @brief resets encoder position
 */
	void encoder_reset(void);

/**
 * @brief initializes encoder
 */
void encoder_init(void);

/**
 * @brief updates encoder
 *
 * This method will read the two inputs calculating the current phase. Then
 * updates the position and direction variables accordingly. If invalid
 * phase transition is detected, an error is returned.
 *
 * @returns error code
 * @retval 0 no error
 * @retval E_INVALID_STATE the encoder has moved too much - state is now unknown
 */
int encoder_update(void);
	
	
	
	
	#endif /*ROTARY_ENCODER_H*/