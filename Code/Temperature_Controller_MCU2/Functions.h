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
#include"servo_motor.h"
#include"uart.h"
#include"adc.h"
#include"timer1.h"
#include"buzzer.h"
#include"std_types.h"
#include <avr/io.h>
#include <util/delay.h>

/*******************************************************************************
 *                        Definitions & Variables                              *
 *******************************************************************************/
#define MCU1_Ready 1
#define MCU2_Ready 2
boolean stop_Machine;
uint8 temp;
Timer1_ConfigType timer_Ptr;
uint8 ticks;
uint16 potentiometer_Value;


/*******************************************************************************
 *                              Functions Prototypes                           *
 *******************************************************************************/
void initalize_Drivers();
void machine_Controller();





#endif /* FUNCTIONS_H_ */
