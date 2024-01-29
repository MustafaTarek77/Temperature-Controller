/*
 * main.c
 *
 *  Created on: Nov 3, 2022
 *      Author: mustafa
 */
#include"Functions.h"

int main()
{
	initalize_Drivers();


	while (1)
	{
		fan_Controller();
	}
}













/*//initalize_Drivers();
	EEPROM_writeByte(memory_Address,normal_State);
	DcMotor_Init();

	while (1)
	{
		//fan_Controller();
		uint8 state=EEPROM_readByte(memory_Address);

		switch(state)
		{
		case(normal_State):
		            DcMotor_Rotate(30);
		            _delay_ms(2000);
		            EEPROM_writeByte(memory_Address,emergency_State);
				    break;
		case(emergency_State):
			        DcMotor_Rotate(70);
		            _delay_ms(2000);
		            EEPROM_writeByte(memory_Address,abnormal_State);
					break;
		case(abnormal_State):
			        DcMotor_Rotate(100);
	                _delay_ms(2000);
	                EEPROM_writeByte(memory_Address,normal_State);
					break;
			}

	}*/
