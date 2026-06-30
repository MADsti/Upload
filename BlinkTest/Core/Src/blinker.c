/*
 * blinker.c
 *
 *  Created on: 08.06.2026
 *      Author: nalle
 */

#include "blinker.h"
#include "outputs.h"
#include "events.h"

#include "main.h"

#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <outputs.h>
#include "vehicle.h"

typedef enum
{
    COMFORT_BLINK_3,
    COMFORT_BLINK_30,
    COMFORT_BLINK_CONTINUOUS

} ComfortBlinkMode_t;


static ComfortBlinkMode_t comfortBlinkMode;

static uint32_t blinkTimer;
static uint32_t blinkPressStart;

static uint16_t blinkCount;

static bool blinkButtonPressed;

static bool blinkPhase;

bool Blinker_GetPhase(void)
{
    return blinkPhase;
}

void Blinker_Init(void)
{
    comfortBlinkMode = COMFORT_BLINK_3;
}

static void Blinker_Stop(void)
{
    blinkButtonPressed = false;
    blinkCount = 0;

    comfortBlinkMode = COMFORT_BLINK_3;

    blinkPhase = false;

    Event_Push(EVENT_BLINK_STOP);
}

static void Blinker_Start(void)
	{
	    blinkButtonPressed = true;

	    blinkPressStart = HAL_GetTick();

	    blinkTimer = HAL_GetTick();

	    blinkPhase = true;

	}

static void Blinker_ButtonRelease(void)
{
    blinkButtonPressed = false;

    switch(comfortBlinkMode)
    {
        case COMFORT_BLINK_3:
            blinkCount = 3;
            break;

        case COMFORT_BLINK_30:
            blinkCount = 30;
            break;

        case COMFORT_BLINK_CONTINUOUS:
            blinkCount = 0xFFFF;
            break;
    }
}

 void Blinker_Update(void)
	{
	 VehicleState_t *state = Vehicle_GetState();

	 if(state->blinkMode == BLINK_OFF)
	 {
	     blinkPhase = false;
	     return;
	 }

				if(blinkButtonPressed)
				{
				    uint32_t holdTime =
				        HAL_GetTick() - blinkPressStart;

				    if(holdTime > 1000)
				    {
				        comfortBlinkMode =
				            COMFORT_BLINK_CONTINUOUS;
				    }
				    else if(holdTime > 500)
				    {
				        comfortBlinkMode =
				            COMFORT_BLINK_30;
				    }
				    else
				    {
				        comfortBlinkMode =
				            COMFORT_BLINK_3;
				    }
				}

			    if((HAL_GetTick() - blinkTimer) > 500)
			    {
			        blinkTimer = HAL_GetTick();

			        blinkPhase = !blinkPhase;

			        blinkPhase = !blinkPhase;

			        if(!blinkPhase)
			        {
			            if(blinkCount != 0xFFFF &&
			               blinkCount > 0)
			            {
			                blinkCount--;

			                if(blinkCount == 0)
			                {
			                    Blinker_Stop();
			                }
			            }
			        }
			    }
	}

void Blinker_LeftDown(void)
{
	Blinker_Start();
}

void Blinker_LeftUp(void)
{
	Blinker_ButtonRelease();

}

void Blinker_RightDown(void)
{
	Blinker_Start();
}

void Blinker_RightUp(void)
{
	Blinker_ButtonRelease();
    }

void Blinker_HazardToggle(void)
{
    Blinker_Start();

    blinkButtonPressed = false;
    comfortBlinkMode = COMFORT_BLINK_CONTINUOUS;
    blinkCount = 0xFFFF;
}
