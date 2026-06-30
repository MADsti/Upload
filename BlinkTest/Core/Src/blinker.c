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

				    if(holdTime > 1500)
				    {
				        comfortBlinkMode =
				            COMFORT_BLINK_CONTINUOUS;
				    }
				    else if(holdTime > 780)
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

void Blinker_HandleEvent(Event_t event)
{
    switch(event)
    {
        case EVENT_BLINK_LEFT_DOWN:
        case EVENT_BLINK_RIGHT_DOWN:

            Blinker_Start();
            break;

        case EVENT_BLINK_LEFT_UP:
        case EVENT_BLINK_RIGHT_UP:

            Blinker_ButtonRelease();
            break;

        default:
            break;
    }
}
