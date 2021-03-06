/*Implementation of the sensor module*/

#include "sensor.h"
#include "bus.h"
#include "error.h"

#define SCRATCHPAD_SATA_SIZE 8
#define TEMP_RESOLUTION 0.0625

/**
 * @brief Reads and converts the temperature from the scratchpad
 *
 * @param[out] temp pointer, to where temperature will be stored
 * @return error code
 * @retval 0 no error
 * @retval E_CRC_FAILED crc checksum failed
 */
 static int read_temp_from_scratchpad(double *temp) {
			uint64_t data = 0;
			BYTE crc = 0;
			BYTE b = 0;
	 
			//read data
			for(int i = 0; i < SCRATCHPAD_SATA_SIZE; i++) {
						bus_read_byte(&b);
						uint64_t dummes_c = b;
						data |= dummes_c << (i * BYTE_SIZE);
			}
			//read crc
			bus_read_byte(&crc);
			
			//CRC verify
			if(bus_check_crc(&data, crc)) {
						int16_t temp_raw = (int16_t) data;
						*temp = temp_raw * TEMP_RESOLUTION;
						return 0;
			}  else {
						return E_CRC_FAILED;
			}
 }
 
 /*measure data*/
 int sensor_measure(uint64_t romcode, double *temp) {
			if(bus_reset()) {
						bus_send_command(BUS_MATCH_ROM_CMD);
						bus_send_romcode(romcode);
						bus_send_command(BUS_MEASURE_CMD);
						bus_provide_power();
				
						if(bus_reset()) {
									bus_send_command(BUS_MATCH_ROM_CMD);
									bus_send_romcode(romcode);
									bus_send_command(BUS_READ_SCRATCHPAD_CMD);
									return read_temp_from_scratchpad(temp);
						}  else {
									return E_NO_SENSOR;
						}
			}  else {
						return E_NO_SENSOR;
			}
 }
 
 //EOF
 