/*Implementation of hardware IO module*/

#include "TI_Lib.h"
#include "error.h"
#include "hardware_io.h"

/*Creates a mask for a pin, assuming 1 bit per pin.*/
#define MASK_ONE_BIT_PIN(i)    (0x01U << (i))

// Pin Configuration ----------------------------------------------------------

/*the pin number where encoder channel A is connected to*/
#define ENCODER_CHANNEL_A 2

/*the pin number where encoder channel B is connected to*/
#define ENCODER_CHANNEL_B 3

/* the pin number of the LED used to indicate an error*/
#define ERROR_LED 5

/**
 * the pin number of the LED used to indicate forward turning
 */
#define FORWARD_LED 6

/**
 * the pin number of the LED used to indicate backward turning
 */
#define BACKWARD_LED 7

// Functions ------------------------------------------------------------------
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

/*Reads channel A from encoder*/
bool io_get_encoder_channel_A(void) {
    return is_pin_high(GPIOF, ENCODER_CHANNEL_A);
}

/*Reads channel B from encoder*/
bool io_get_encoder_channel_B(void) {
    return is_pin_high(GPIOF, ENCODER_CHANNEL_B);
}

/*Displays data on LEDs*/
void io_display_data(char number, Direction dir) {
    // create reset mask
    int reset_mask = 0xFF00; // upper 8 bits get reset
    reset_mask |= MASK_ONE_BIT_PIN(FORWARD_LED); // forward LED gets reset
    reset_mask |= MASK_ONE_BIT_PIN(BACKWARD_LED); // backwards LED gets reset

    // reset our pins to 0
    GPIOG->BSRRH = reset_mask;

    // display number on upper 8 bits
    GPIOG->BSRRL = number << 8;

    // display direction
    if (dir == FORWARD) {
        GPIOG->BSRRL = MASK_ONE_BIT_PIN(FORWARD_LED);
    } else if (dir == BACKWARD) {
        GPIOG->BSRRL = MASK_ONE_BIT_PIN(BACKWARD_LED);
    }
}

/* Checks reset switches S6 + S7 */
bool io_is_reset_pressed(void) {
    return !is_pin_high(GPIOE, 6) || !is_pin_high(GPIOE, 7);
}

/*Displays an error by illuminating D18*/
void io_show_error(void) {
    GPIOG->BSRRL = MASK_ONE_BIT_PIN(ERROR_LED);
}

/*Clears the error LED D18*/
void io_clear_error(void) {
    GPIOG->BSRRH = MASK_ONE_BIT_PIN(ERROR_LED);
}


void io_init_interrupts() {
			//Clocks
			RCC->AHB1ENR |= RCC_AHB1ENR_GPIOFEN; //clock for GPIO Port F
			RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN; //Digital Interface clock
			//Routing Pin 2 of Port F -> EXTI2
			SYSCFG->EXTICR[0] &= ~(0x0f << (4*2)); //Remove old selection
			SYSCFG->EXTICR[0] |=  0x05 << (4*2); //0x05 : select Port F
			//Routing Pin 3 of Port F -> EXTI2
			SYSCFG->EXTICR[0] &= ~(0x0f << (4*3)); //Remove old selection
			SYSCFG->EXTICR[0] |=  0x05 << (4*3); //0x05 : select Port F
	
			//Select Triggers (rising/falling) for both pins
			EXTI->RTSR |= (1<<2); //rising trigger for int 2
			EXTI->FTSR |= (1<<2); //falling trigger for int 2
	
			EXTI->RTSR |= (1<<3); //for int 3
			EXTI->FTSR |= (1<<3); //for int 3
	
			//unmask
			EXTI->IMR |= (1<<2); //int2
			EXTI->IMR |= (1<<3); //int3
	
			//Configuring Interrupot Controller
			NVIC_SetPriorityGrouping(2); //setup interrupt controller
																								 //4 subpriority for each priority
		 NVIC_SetPriority(EXTI2_IRQn, 8); //Setup EXTI2:
																											//priority = 2, subpriority = 0
		 NVIC_SetPriority(EXTI3_IRQn, 8); //setup priority for EXTI3
		 
		 //Enable
		 NVIC_EnableIRQ(EXTI2_IRQn);
		 NVIC_EnableIRQ(EXTI3_IRQn);
}

