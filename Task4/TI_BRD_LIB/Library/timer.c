/**
  ******************************************************************************
  * @file    	timer.c 
  * @author  	Schoenher, Nils
  * @version  V1.0
  * @date     10.10.2016
  * @brief    Timer related functions
  ******************************************************************************
  */

#include "TI_memory_map.h"
#include "timer.h"


#define TIM2_CLOCK      84  /* Mhz */
// 84 Timer Ticks == 1 us

void timerinit(void){
    RCC->APB1ENR |= RCC_APB1ENR_TIM2EN; /* Takt fuer Timer 2 einschalten */
    TIM2->CR1 = 0;                      /* Timer disabled                */
    TIM2->CR2 = 0;                      /*                               */
    TIM2->PSC = 0;                      /* Prescaler   (84MHz)           */
#ifndef SIMULATION
    TIM2->ARR = 0xffffffff;             /* Auto reload register          */
#else
    TIM2->ARR = 0xffff;             		/* Auto reload register          */
#endif
    TIM2->DIER  = 0;                    /* Interrupt ausschalten         */
    TIM2->CR1 = TIM_CR1_CEN;            /* Enable Timer                  */
}

uint32_t getTimeStamp(){
	return (TIM2->CNT);
}

uint64_t timerDiffToNsec(uint32_t firstValue, uint32_t secondValue) {
	 uint32_t v = secondValue - firstValue; // unsigned, no problem with overflow
	 // Berechne das Ergebnis, da ein Timer Tick 1/(84MHz) = ca. 12 ns und eine Breite von 32 Bit hat.
	 uint64_t erg = ((uint64_t) v) * 1000; 
	 erg = erg/TIM2_CLOCK;
	 return erg;
}


