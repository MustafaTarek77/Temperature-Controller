/*
 * main.c
 *
 *  Created on: Nov 3, 2022
 *      Author: mustafa
 */
#include "Functions.h"

int main()
{
	initalize_Drivers();

	while (1)
	{
		machine_Controller();
	}
}
