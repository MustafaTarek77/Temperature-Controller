/*
 * Functions.c
 *
 *  Created on: Nov 5, 2022
 *      Author: mustafa
 */
#include"Functions.h"

void wait_CTC()
{
	ticks++;
}

void Ready_To_Send()
{
	//mcu2 is ready to send
	UART_sendByte(MCU2_Ready);

	while(UART_recieveByte()!=MCU1_Ready)                       //wait until mcu1 is ready
	{ }
}

void Ready_To_Recieve()
{
	while(UART_recieveByte()!=MCU1_Ready)                       //wait until mcu1 is ready to send
	{ }

	//mcu2 is ready to recieve
	UART_sendByte(MCU2_Ready);
}


void initalize_Drivers() {
	/* Enable Global Interrupt I-Bit */
	SREG |= (1 << 7);


	/* Initialize the DC Motor */
	DcMotor_Init();

	/* Initialize the Servo Motor */
	GPIO_setupPinDirection(PORTD_ID, PIN5_ID, PIN_OUTPUT);
	rotate_Servo(124);	/* Set Servo shaft at 0° position by 1 ms pulse */

	/* initialize Buzzer driver */
	Buzzer_init();

	temp = 0;

	/* initialize UART driver */
	UART_ConfigType UART_Ptr;
	UART_Ptr.bit_data = 0X03;
	UART_Ptr.baud_rate = 9600;
	UART_Ptr.parity = 0;
	UART_Ptr.stop_bit = 0;
	UART_init(&UART_Ptr);

	/* initialize ADC driver */
	ADC_ConfigType Config_Ptr;
	Config_Ptr.prescaler = 0X03;
	Config_Ptr.ref_volt = 0X01;
	ADC_init(&Config_Ptr);

	/* initialize Timer driver */
	timer_Ptr.mode = CTC_Mode;
	timer_Ptr.prescaler = 0X05;     //F_CPU/1024
	timer_Ptr.initial_value = 0;
	timer_Ptr.compare_value = 7813;   //one second
	Timer1_setCallBack(wait_CTC);
	ticks = 0;

	/* initialize the Leds */
	GPIO_setupPinDirection(PORTB_ID, PIN5_ID, PIN_OUTPUT);
	GPIO_writePin(PORTB_ID, PIN5_ID, 1);
	GPIO_setupPinDirection(PORTB_ID, PIN6_ID, PIN_OUTPUT);
	GPIO_writePin(PORTB_ID, PIN6_ID, 1);
	GPIO_setupPinDirection(PORTB_ID, PIN7_ID, PIN_OUTPUT);
    GPIO_writePin(PORTB_ID, PIN7_ID, 1);

    stop_Machine=FALSE;
}



void machine_Controller()
{
	potentiometer_Value=ADC_readChannel(2);
	potentiometer_Value=potentiometer_Value*(255.0/1023.0);
	if(stop_Machine==FALSE || temp<=40 || temp>50)
    	DcMotor_Rotate(potentiometer_Value);

	//Receive temperature through UART
   	Ready_To_Recieve();
	temp=UART_recieveByte();

	/**************************Normal State**************************/
	if(temp<20)
	{
		GPIO_writePin(PORTB_ID, PIN5_ID, 0);
		GPIO_writePin(PORTB_ID, PIN6_ID, 1);
		GPIO_writePin(PORTB_ID, PIN7_ID, 1);
	}
	else if(temp<40)
	{
		GPIO_writePin(PORTB_ID, PIN6_ID, 0);
		GPIO_writePin(PORTB_ID, PIN5_ID, 1);
		GPIO_writePin(PORTB_ID, PIN7_ID, 1);
	}
	else if(temp<=50)
	{
		GPIO_writePin(PORTB_ID, PIN7_ID, 0);
		GPIO_writePin(PORTB_ID, PIN5_ID, 1);
		GPIO_writePin(PORTB_ID, PIN6_ID, 1);

		//Receive stop_Machine through UART
		Ready_To_Recieve();
		stop_Machine=UART_recieveByte();
		if(stop_Machine==TRUE)
			DcMotor_Rotate(0);

	}
	else
	{
		/**************************Emergency State**************************/
		GPIO_writePin(PORTB_ID, PIN7_ID, 0);
		GPIO_writePin(PORTB_ID, PIN5_ID, 1);
		GPIO_writePin(PORTB_ID, PIN6_ID, 1);
		Buzzer_on();
		Timer1_init(&timer_Ptr);
		ticks=0;
		while (temp > 50)
		{
			//Receive temperature through UART
		   	Ready_To_Recieve();
			temp=UART_recieveByte();
			if (ticks >= 7)
			{
				/**************************Abnormal State**************************/
				rotate_Servo(187);	/* Set Servo shaft at 90° position by 1.5 ms pulse */
				DcMotor_Rotate(0);
				while(ticks<13)
				{
				}
				break;
			}
		}
		ticks = 0;
		Buzzer_off();
		Timer1_deInit();
		rotate_Servo(124);
	}
}
