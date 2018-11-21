
/**
  ******************************************************************************
  * @file    	timer.h 
  * @author  	Schoenher, Nils
  * @version  V1.0
  * @date     10.10.2016
  * @brief    Timer for calculation
  ******************************************************************************
  */

#ifndef TIMER_H
#define TIMER_H

#include <stdint.h>

/**
 * @brief  Diese Funktion initialisiert das Timer-Modul
 *         Das Modul verwendet den Timer TIM2
 * @param  None
 * @retval None
 */
void initTimer(void);

/**
 * @brief  Diese Funktion liefert einen Zeitstempel in der Einheit Timer Ticks
 *         84 Timer Ticks entsprechen 1 us
 * @param  None
 * @retval Aktuelle TimeStamp
 */
uint32_t getTimeStamp(void);

/**
 * @brief  Diese Funktion berechnet die Zeitspanne zwischen zwei
 *         Zeitstempeln
 * @param  firstTimeStamp ist der erste (frühere) Zeitstempel
 * @param  secondTimeStamp ist der zweite (spätere) Zeitstempel
 * @retval Die Zeit (in ns), die zwischen den beiden Zeitstempeln vergangen ist.
 */
uint64_t timerDiffToNsec(uint32_t firstTimeStamp, uint32_t secondTimeStamp);

#endif
