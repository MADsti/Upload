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

typedef enum
{
    LIGHT_OFF,
    LIGHT_LOWBEAM,
    LIGHT_LOWBEAM_HIGHBEAM

} LightState_t;


void Outputs_Update(void);

static bool lightButtonPressed = false;
static uint32_t lightPressStart = 0;
static bool lightLongPressHandled = false;

static LightState_t lightState = LIGHT_OFF;

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
		if(lightButtonPressed &&
		   !lightLongPressHandled)
		{
		    if((HAL_GetTick() - lightPressStart) > 1500)
		    {
		        lightState = LIGHT_OFF;

		        lightLongPressHandled = true;
		    }
		}
		switch(lightState)
		{
		    case LIGHT_OFF:

		    	outputs.lowBeam = false;
		    	outputs.highBeam = false;
		        break;

		    case LIGHT_LOWBEAM:

		    	outputs.lowBeam = true;
		    	outputs.highBeam = false;
		        break;

		    case LIGHT_LOWBEAM_HIGHBEAM:

		    	outputs.lowBeam = true;
		    	outputs.highBeam = true;
		        break;
		}
	}

	void Lights_LightDown(void)
	{
	    lightButtonPressed = true;

	    lightPressStart = HAL_GetTick();

	    lightLongPressHandled = false;
	}

	void Lights_LightUp(void)
	{
	    lightButtonPressed = false;

	    if(lightLongPressHandled)
	    {
	        return;
	    }

	    switch(lightState)
	    {
	        case LIGHT_OFF:

	            lightState = LIGHT_LOWBEAM;
	            break;

	        case LIGHT_LOWBEAM:

	            lightState = LIGHT_LOWBEAM_HIGHBEAM;
	            break;

	        case LIGHT_LOWBEAM_HIGHBEAM:

	            lightState = LIGHT_LOWBEAM;
	            break;
	    }
	}




