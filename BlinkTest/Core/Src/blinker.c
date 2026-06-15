/*
 * blinker.c
 *
 *  Created on: 08.06.2026
 *      Author: nalle
 */

#include "blinker.h"
#include "outputs.h"

#include "main.h"

#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <outputs.h>

typedef enum
{
    BLINK_MODE_3,
    BLINK_MODE_30,
    BLINK_MODE_CONTINUOUS
} BlinkMode_t;

typedef enum
{
    BLINKER_OFF,
    BLINKER_LEFT,
    BLINKER_RIGHT,
    BLINKER_HAZARD
} BlinkerState_t;

static BlinkerState_t blinkerState;
static BlinkMode_t blinkMode;

static int lastMode = -1;

static uint32_t blinkTimer;
static uint32_t blinkPressStart;

static uint16_t blinkCount;

static bool blinkButtonPressed;
static bool blinkOutputState;
static bool lastBlinkOutputState;

void Blinker_Init(void)
{
    blinkerState = BLINKER_OFF;
    blinkMode = BLINK_MODE_3;
}

static void Blinker_Stop(void)
	{
	    blinkerState = BLINKER_OFF;

	    blinkButtonPressed = false;
	    blinkCount = 0;

	    blinkMode = BLINK_MODE_3;

	    blinkOutputState = false;
	    lastBlinkOutputState = false;

	}

static void Blinker_Start(
	    BlinkerState_t direction)
	{
	    blinkButtonPressed = true;

	    blinkPressStart = HAL_GetTick();

	    blinkerState = direction;

	    blinkTimer = HAL_GetTick();

	    blinkOutputState = true;
	    lastBlinkOutputState = false;

	    switch(direction)
	    {
	        case BLINKER_LEFT:

	        	outputs.leftBlinker = true;
	        	outputs.rightBlinker = false;
	            break;

	        case BLINKER_RIGHT:

	        	outputs.leftBlinker = false;
	        	outputs.rightBlinker = true;
	            break;

	        case BLINKER_HAZARD:

	        	outputs.leftBlinker = true;
	        	outputs.rightBlinker = true;
	            break;

	        default:
	            break;
	    }

	    Outputs_Update();

	}

static void Blinker_ButtonRelease(void)
{
    blinkButtonPressed = false;

    switch(blinkMode)
    {
        case BLINK_MODE_3:
            blinkCount = 3;
            break;

        case BLINK_MODE_30:
            blinkCount = 30;
            break;

        case BLINK_MODE_CONTINUOUS:
            blinkCount = 0xFFFF;
            break;
    }
}

 void Blinker_Update(void)
	{
		if(blinkerState == BLINKER_OFF)
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
				        blinkMode =
				            BLINK_MODE_CONTINUOUS;
				    }
				    else if(holdTime > 500)
				    {
				        blinkMode =
				            BLINK_MODE_30;
				    }
				    else
				    {
				        blinkMode =
				            BLINK_MODE_3;
				    }
				}


				if(blinkMode != lastMode)
				{
				    printf("Mode = %d\r\n", blinkMode);

				    lastMode = blinkMode;
				}

			    if((HAL_GetTick() - blinkTimer) > 500)
			    {
			        blinkTimer = HAL_GetTick();

			        blinkOutputState = !blinkOutputState;

			        if(lastBlinkOutputState == true &&
			        		blinkOutputState == false)
			        {
			            if(blinkCount != 0xFFFF &&
			               blinkCount > 0)
			            {
			                blinkCount--;

			                printf("Remaining: %u\r\n",
			                       blinkCount);

			                if(blinkCount == 0)
			                {
			                	Blinker_Stop();
			                }
			            }
			        }

			        lastBlinkOutputState = blinkOutputState;

			        switch(blinkerState)
			        {
			            case BLINKER_LEFT:

			                printf("LEFT CASE\n\r");

			                outputs.leftBlinker = blinkOutputState;
			                outputs.rightBlinker = false;
			                break;

			            case BLINKER_RIGHT:

			                printf("RIGHT CASE\n\r");

			                outputs.leftBlinker = false;
			                outputs.rightBlinker = blinkOutputState;
			                break;

			            case BLINKER_HAZARD:

			                printf("HAZARD CASE\n\r");

			                outputs.leftBlinker = blinkOutputState;
			                outputs.rightBlinker = blinkOutputState;
			                break;

			            default:

			            	outputs.leftBlinker = false;
			            	outputs.rightBlinker = false;
			            	break;
			        }
			    }
	}

void Blinker_LeftDown(void)
{
	if(blinkerState == BLINKER_HAZARD)
	{
	    Blinker_Stop();
	    return;
	}
	if(blinkerState == BLINKER_LEFT)
    {
		Blinker_Stop();
        return;
    }

	Blinker_Start(BLINKER_LEFT);


}

void Blinker_LeftUp(void)
{
	Blinker_ButtonRelease();

}

void Blinker_RightDown(void)
{
	if(blinkerState == BLINKER_HAZARD)
	{
	    Blinker_Stop();
	    return;
	}
    if(blinkerState == BLINKER_RIGHT)
    {
    	Blinker_Stop();
        return;
    }

    Blinker_Start(BLINKER_RIGHT);
}

void Blinker_RightUp(void)
{
	Blinker_ButtonRelease();
    }

void Blinker_HazardToggle(void)
{
    if(blinkerState == BLINKER_HAZARD)
    {
        Blinker_Stop();
        return;
    }

    Blinker_Start(BLINKER_HAZARD);

    blinkButtonPressed = false;

    blinkMode = BLINK_MODE_CONTINUOUS;
    blinkCount = 0xFFFF;

    printf("HAZARD START\n");
}
