/*
 * Functions.c
 *
 *  Created on: Nov 5, 2022
 *      Author: mustafa
 */
#include"Functions.h"

state_Type state;
void wait_CTC()
{
	ticks++;
}

void Ready_To_Send()
{
	//mcu2 is ready to send
	UART_sendByte(MCU1_Ready);

	while(UART_recieveByte()!=MCU2_Ready)                       //wait until mcu1 is ready
	{ }
}

void Ready_To_Recieve()
{
	while(UART_recieveByte()!=MCU2_Ready)                       //wait until mcu1 is ready to send
	{ }

	//mcu2 is ready to recieve
	UART_sendByte(MCU1_Ready);
}

void initalize_Drivers() {
	/* Enable Global Interrupt I-Bit */
	SREG |= (1 << 7);

	/* Write normal state to EEPROM address 0x00 initially */
	//EEPROM_writeByte(memory_Address,normal_State);
	state = normal_State;

	/* Initialize the DC Motor */
	DcMotor_Init();

	/* initialize ADC driver */
	ADC_ConfigType Config_Ptr;
	Config_Ptr.prescaler = 0X03;
	Config_Ptr.ref_volt = 0X03;
	ADC_init(&Config_Ptr);

	temp = 0;

	/* initialize UART driver */
	UART_ConfigType UART_Ptr;
	UART_Ptr.bit_data = 0X03;
	UART_Ptr.baud_rate = 9600;
	UART_Ptr.parity = 0;
	UART_Ptr.stop_bit = 0;
	UART_init(&UART_Ptr);

	/* initialize Timer driver */
	timer_Ptr.mode = CTC_Mode;
	timer_Ptr.prescaler = 0X05;     //F_CPU/1024
	timer_Ptr.initial_value = 0;
	timer_Ptr.compare_value = 7813;   //one second
	Timer1_setCallBack(wait_CTC);
	ticks = 0;

	/* initialize the Button */
	GPIO_setupPinDirection(PORTB_ID, PIN0_ID, PIN_INPUT);
	GPIO_writePin(PORTB_ID, PIN0_ID, 1);
	stop_Machine=FALSE;
}



void fan_Controller()
{
    //state=EEPROM_readByte(memory_Address);
	temp = LM35_getTemperature();

	//Send temperature through UART
	Ready_To_Send();
	UART_sendByte(temp);

	if (temp < 20)
	{
		//EEPROM_writeByte(memory_Address,normal_State);
		state=normal_State;
		DcMotor_Rotate(0);
	}
	else if (temp < 40)
	{
		//EEPROM_writeByte(memory_Address,normal_State);
		state=normal_State;
		uint8 speed = (temp - 20) * 5;
		DcMotor_Rotate(speed);
	}
	else if (temp <= 50)
	{
		//EEPROM_writeByte(memory_Address,normal_State);
		state=normal_State;
		DcMotor_Rotate(100);
		if(!GPIO_readPin(PORTB_ID, PIN0_ID))
			stop_Machine=TRUE;
		else
			stop_Machine=FALSE;

		//Send stop_Machine through UART
		Ready_To_Send();
		UART_sendByte(stop_Machine);
	}
	else
	{
		//EEPROM_writeByte(memory_Address,emergency_State);
		state = emergency_State;
		DcMotor_Rotate(100);
		Timer1_init(&timer_Ptr);
		ticks=0;
		while (temp > 50)
		{
			temp = LM35_getTemperature();
			//Send temperature through UART
	     	Ready_To_Send();
			UART_sendByte(temp);
			if (ticks >= 7)
			{
		    //EEPROM_writeByte(memory_Address,abnormal_State);
			state = abnormal_State;
			WDT_ON();
			}
		}
		ticks = 0;
		Timer1_deInit();
	}
}
