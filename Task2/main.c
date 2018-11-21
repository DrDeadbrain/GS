/** ****************************************************************
 * @file    aufgabe2/main.c
 * @author  Christian Caus (christiancaus@gmail.com)
 * @version 1.0
 * @date    21.11.2018
 * @brief   Main module and program entry point
 ******************************************************************
 */
 
 #include "error.h"
 #include <timer.h>
 #include "TI_Lib.h"
 #include "hardware_io.h"
 #include "output.h"
 #include "rotary_encoder.h"
 
 /**
  *how often should speed be recalculated (in iterations of super loop)
 */
 #define SPEED_UPDATE_INTERVAL 40
 
 /**
 * @brief for debugging with oscilloscope
 *
 * could be put in Hardware IO module, but direct access is faster
 */
 #define OSCI_HIGH GPIOG -> BSRRH = 1
 
 /**
 * @brief for debugging with oscilloscope
 *
 * could be put in Hardware IO module, but direct access is faster
 */
 #define OSCI_LOW GPIOG->BSRRH = 1
 
 /** ****************************************************************
 * @brief   Program entry point
 ******************************************************************
 */
 
int main (void) {
	//local variables
	int res;
	int counter = 0;
	
	//timer
	uint32_t last_time = getTimeStamp();
	uint32_t new_time = last_time;
	int last_pos = 0;
	int new_pos = 0;
	double speed = 0.0;
	
	//initialize everything
	Init_TI_Board();
	encoder_init();
	initTimer();
	output_init();
	
	while (1)  {
		if (is_reset_pressed()) {
			//reset everything
			encoder_reset();
			clear_error();
			
			//reset timer
			last_pos = 0;
			last_time = getTimeStamp();
			speed = 0;
		}
		
		//update encoder
		res = encoder_update();
		if (res == E_INVALID_STATE) {
			//skipped a step => show error and stop output
			show_error();
		} else {
			//continue with output
			
			if (counter > SPEED_UPDATE_INTERVAL) {
				//update timer
				counter = 0;
				new_time = getTimeStamp();
				new_pos = encoder_get_pos();
			
				uint64_t time_diff = timerDiffToNsec(last_time, new_time);
				int pos_diff = new_pos - last_pos; 
				
				// ((tenth of degree /ns) * 1 * 10^9 ns/s) / 10 (tenth of degree per degree)
				speed = ((double) pos_diff / time_diff ) * 1e9 /10;
				
				last_time = new_time; 
				last_pos = new_pos; 
			}
			
			//make sure that the number LEDs always display a positive number 
			int pulses = encoder_raw_pos();
			if (pulses < 0) {
				pulses *= -1;
			}
			
			//display on LED
			display_data((char) pulses, encoder_get_dir());
			
			//display on TFT
			OSCI_HIGH;
			output_update_data (encoder_get_pos() / 10.0, speed);
			OSCI_LOW;
			
		}
		counter++;
	}
}

//EOF
