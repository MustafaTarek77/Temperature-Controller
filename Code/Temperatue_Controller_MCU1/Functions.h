/*
 * Functions.h
 *
 *  Created on: Nov 5, 2022
 *      Author: mustafa
 */
#ifndef FUNCTIONS_H_
#define FUNCTIONS_H_

/*******************************************************************************
 *                                  Includes                                   *
 *******************************************************************************/
#include"gpio.h"
#include"PWM_timer.h"
#include"DC_motor.h"
#include"Internal_EEPROM.h"
#include "adc.h"
#include "lm35_sensor.h"
#include"uart.h"
#include"watchDog.h"
#include"timer1.h"
#include"std_types.h"
#include <avr/io.h>
#include <util/delay.h>

/*******************************************************************************
 *                        Definitions & Variables                              *
 *******************************************************************************/
#define MCU1_Ready 1
#define MCU2_Ready 2
boolean stop_Machine;
#define memory_Address 0x00
uint8 temp;
Timer1_ConfigType timer_Ptr;
uint8 ticks;

/*******************************************************************************
 *                               Types Declaration                             *
 *******************************************************************************/
typedef enum
{
	normal_State,emergency_State,abnormal_State
}state_Type;


/*******************************************************************************
 *                              Functions Prototypes                           *
 *******************************************************************************/
void initalize_Drivers();
void fan_Controller();





#endif /* FUNCTIONS_H_ */
