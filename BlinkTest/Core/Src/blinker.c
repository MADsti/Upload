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

typedef enum
{
    BLINKER_OFF,
    BLINKER_LEFT,
    BLINKER_RIGHT,
    BLINKER_HAZARD

} BlinkerState_t;

static BlinkerState_t blinkerState;
static ComfortBlinkMode_t comfortBlinkMode;

static int lastMode = -1;

static uint32_t blinkTimer;
static uint32_t blinkPressStart;

static uint16_t blinkCount;

static bool blinkButtonPressed;
static bool lastBlinkPhase;

static bool blinkPhase;

bool Blinker_GetPhase(void)
{
    return blinkPhase;
}

void Blinker_Init(void)
{
    blinkerState = BLINKER_OFF;
    comfortBlinkMode = COMFORT_BLINK_3;
}

static void Blinker_Stop(void)
	{
	    blinkerState = BLINKER_OFF;

	    blinkButtonPressed = false;
	    blinkCount = 0;

	    comfortBlinkMode = COMFORT_BLINK_3;

	    blinkPhase = false;
	    lastBlinkPhase = false;

	}

static void Blinker_Start(void)
	{
	    blinkButtonPressed = true;

	    blinkPressStart = HAL_GetTick();

	    blinkTimer = HAL_GetTick();

	    blinkPhase = true;
	    lastBlinkPhase = false;

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
					outputs.leftBlinker = false;
					outputs.rightBlinker = false;

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


				if(comfortBlinkMode != lastMode)
				{

				    lastMode = comfortBlinkMode;
				}

			    if((HAL_GetTick() - blinkTimer) > 500)
			    {
			        blinkTimer = HAL_GetTick();

			        blinkPhase = !blinkPhase;

			        if(lastBlinkPhase == true &&
			        		blinkPhase == false)
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

			        lastBlinkPhase = blinkPhase;
			    }
	}

void Blinker_LeftDown(void)
{

	VehicleState_t *state = Vehicle_GetState();
	if(state->blinkMode == BLINK_HAZARD)
	{
		Event_Push(EVENT_HAZARD_OFF);
	    Blinker_Stop();
	    return;
	}
	if(state->blinkMode == BLINK_LEFT)
    {
		Blinker_Stop();
        return;
    }

	Blinker_Start();


}

void Blinker_LeftUp(void)
{
	Blinker_ButtonRelease();

}

void Blinker_RightDown(void)
{
	VehicleState_t *state = Vehicle_GetState();
	if(state->blinkMode == BLINK_HAZARD)
	{
		Event_Push(EVENT_HAZARD_OFF);
	    Blinker_Stop();
	    return;
	}
	if(state->blinkMode == BLINK_RIGHT)
    {
    	Blinker_Stop();
        return;
    }

	Blinker_Start();
}

void Blinker_RightUp(void)
{
	Blinker_ButtonRelease();
    }

void Blinker_HazardToggle(void)
{
	VehicleState_t *state = Vehicle_GetState();

	if(state->blinkMode == BLINK_HAZARD)
    {
        Blinker_Stop();
        return;
    }

	Blinker_Start();

    blinkButtonPressed = false;

    comfortBlinkMode = COMFORT_BLINK_CONTINUOUS;
    blinkCount = 0xFFFF;

}
