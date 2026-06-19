/*
 * licht.c
 *
 *  Created on: 05.06.2026
 *      Author: nalle
 */

#include <lights.h>
#include <stdint.h>
#include <main.h>
#include <stdio.h>
#include <blinker.h>
#include <outputs.h>
#include "vehicle.h"


void Outputs_Update(void);


static void UpdateLight(void);

void Lights_Init(void)
{
}


	void Lights_Update(void)
	{
		Blinker_Update();

		UpdateLight();

        Outputs_Update();
	}

	static void UpdateLight(void)
	{
	    VehicleState_t* state =
	        Vehicle_GetState();

	    switch(state->light)
	    {
	        case LIGHT_OFF:

	            outputs.lowBeam = false;
	            outputs.highBeam = false;
	            break;

	        case LIGHT_LOW:

	            outputs.lowBeam = true;
	            outputs.highBeam = false;
	            break;

	        case LIGHT_HIGH:

	            outputs.lowBeam = true;
	            outputs.highBeam = true;
	            break;
	    }
	}




