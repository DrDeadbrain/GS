/*main module and program entry point*/


#include "error.h"
#include "timer.h"
#include "TI_Lib.h"
#include "hardware_io.h"
#include "output.h"
#include "rotary_encoder.h"

/**
 * how often should speed be recalculated (in iterations of super loop)
 */
#define SPEED_UPDATE_INTERVAL 30

/*program entry point*/
int main (void) {
			//local variable
			int counter = 0;
	
			//timer
			uint32_t last_time = getTimeStamp();
			uint32_t new_line = last_time;
			int last_pos = 0;
			int new_pos = 0;
			double speed = 0.0;
			int new_time = 0;
	
			//initialize everything
			Init_TI_Board();
			encoder_init();
			initTimer();
			output_init();
			hwio_init_interupts();
	
			while(1) {
						if (io_is_reset_pressed()) {
									//reset everything
									encoder_reset();
									io_clear_error();
									
									//reset timer
									last_pos = 0;
									last_time = getTimeStamp();
									speed = 0.0;
						}
						
						if (counter > SPEED_UPDATE_INTERVAL) {
									//update timer
									counter = 0;
									new_time = getTimeStamp();
									new_pos = encoder_get_position();
							
									uint64_t time_diff = timerDiffToNsec(last_time, new_time);
									int pos_diff = new_pos - last_pos;
							
									// (tenth of degree / ns) * 1 * 10^9 ns/s / 10 tenth of degree per degree
									speed = (pos_diff * 1e9 / time_diff / 10);
							
									last_time = new_time;
									last_pos = new_pos;
						}
						
						// make sure that the number LEDS always display a positive number
						int pulses = encoder_get_position_raw();
						if (pulses < 0) {
								pulses *= -1;
            }
						
						//display on LED
						io_display_data((char) pulses, encoder_get_direction());
						
						//display on display
						output_update_data(encoder_get_position() / 10.0, speed);
						
						counter++;
			}
}