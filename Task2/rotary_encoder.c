/** 
 * @file    aufgabe2/rotary_encoder.c
 * @author  Christian Caus(christiancaus@gmail.com)
 * @version 1.0
 * @date    07.11.2018
 * @brief   Implementation of the Rotary Encoder module
 */
#include "rotary_encoder.h"
#include "error.h"
#include "hardware_io.h"
#include <stdbool.h>
#include <stdio.h>

//Constants 
/**
	*Cycles per revolution
	*/

#define CPR 300

/**
	*Pulses per revolution
	*/

#define PPR (CPR*4)

/**
	* How many degrees per revolution
	*/

#define DPR 360

//Global Variables
/**
	* direction of encoder rotation
	*/
	static Direction direction;
	
/**
  *position of encoder in pulses relative to latest reset
	*/
 static int position;
 
 /**
	 *boolean if encoder is in valid state
	*/
  static bool valid_state;
	
//Functions

/**
	*resets encoder position
	*/
	void encoder_reset(void) {
		direction = UNKNOWN;
		valid_state = true;
		position = 0;
}

/**
	*func that returns direction of encoder
	*/
	Direction encoder_get_dir() {
		return direction;
	}
	
/**
	*func that returns pos in pulses
	*/
	int encoder_raw_pos() {
		return position;
	}
	
/**
	*func that returns pos in 0.1 degrees
	*/
	int encoder_get_pos() {
		//x10 for returning 10th of deg.
		return position * DPR *10 / PPR;
	}
	
/**
	*reads inputs and calculates phase
	*@return phase
	*/
	static int get_phase() {
		bool channel_a;
		bool channel_b;
		
		//read from io
		channel_a = get_encoder_channel_A();
		channel_b = get_encoder_channel_B();
		
		int phase;
		if (channel_a) {
				if (channel_b)  {
						//both on high => c
						phase = 2;
			} else {
						//a on high, b on low => b
						phase = 1;
			}
			} else {
						if(channel_b) {
								//a on low, b on high => d
								phase = 3;
					} else {
						//both on low => a
						phase = 0;
			}
		}
		return phase;
	}
	
/**
	*updates encoder
	*/
	int encoder_update() {
		static int current_phase;
		
		//reads inputs
		int next_phase = (int) get_phase;
		
		if (!valid_state) {
				//remember current_phase for next time
				current_phase = next_phase;
				
				return E_INVALID_STATE;
		}
		
		if (current_phase == next_phase) {
				//stationary => do nothing
		} else if  (((current_phase +1 ) % 4) == next_phase)  {
					 //forwards turnin
					direction = FORWARD;
					position++;
		} else if (((current_phase + 3) % 4) == next_phase) {
					//backwards turnin
					direction = BACKWARD;
					position--;
		} else {
					//invalid (step was skipped)
				  valid_state = false;
					
				 //remember current phase 
				 current_phase = next_phase;
				
				return E_INVALID_STATE;
		}
		
			//remember current phase
		  current_phase = next_phase;
		return 0;
	}
	
/**
	*init encoder
	*/
	void encoder_init () {
			encoder_reset();
			//update once to init static var current_phase
			encoder_update();
			encoder_reset();
	}

//EOF
