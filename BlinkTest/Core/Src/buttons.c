/*
 * buttons.c
 *
 *  Created on: 05.06.2026
 *      Author: nalle
 */

#include "buttons.h"
#include "main.h"
#include "events.h"


typedef struct

{
	GPIO_TypeDef* port;
	uint16_t pin;

    bool pressed;
    bool lastPressed;

    bool lastRawState;

    uint32_t debounceTime;
} Button_t;

static bool hazardCombinationActive = false;
static uint32_t hazardStartTime = 0;


static Button_t buttons[BUTTON_COUNT] =
{
    [BUTTON_BLINK_LEFT] =
    {
        .port = GPIOA,
        .pin  = GPIO_PIN_10
    },

    [BUTTON_BLINK_RIGHT] =
    {
        .port = GPIOA,
        .pin  = GPIO_PIN_9
    },

    [BUTTON_LIGHT] =
    {
        .port = GPIOA,
        .pin  = GPIO_PIN_15
    },

    [BUTTON_HORN] =
    {
        .port = GPIOA,
        .pin  = GPIO_PIN_13
    },
    [BUTTON_DISPLAY] =
    {
        .port = GPIOA,
        .pin  = GPIO_PIN_8
    }

};

void Buttons_Init(void)
{
    for(int i = 0; i < BUTTON_COUNT; i++)
    {
        bool state =
            (HAL_GPIO_ReadPin(
                buttons[i].port,
                buttons[i].pin)
             == GPIO_PIN_RESET);

        buttons[i].pressed = state;
        buttons[i].lastPressed = state;
        buttons[i].lastRawState = state;
        buttons[i].debounceTime = HAL_GetTick();
    }
}
void Buttons_Update(void)
{
	for(int button = 0;
	    button < BUTTON_COUNT;
	    button++)
	{
	    bool rawState =
	        (HAL_GPIO_ReadPin(
	            buttons[button].port,
	            buttons[button].pin)
	         == GPIO_PIN_RESET);

	    if(rawState != buttons[button].lastRawState)
	    {
	        buttons[button].debounceTime =
	            HAL_GetTick();

	        buttons[button].lastRawState =
	            rawState;
	    }

	    if((HAL_GetTick() -
	        buttons[button].debounceTime) > 20)
	    {
	        buttons[button].pressed = rawState;

	        if(buttons[button].pressed != buttons[button].lastPressed)
	        {

	            switch(button)
	            {
	                case BUTTON_BLINK_LEFT:

	                    if(buttons[button].pressed)
	                    {
	                        Event_Push(
	                            EVENT_BLINK_LEFT_DOWN);
	                    }
	                    else
	                    {
	                        Event_Push(
	                            EVENT_BLINK_LEFT_UP);
	                    }

	                    break;

	                case BUTTON_BLINK_RIGHT:


	                    if(buttons[button].pressed)
	                    {
	                        Event_Push(
	                            EVENT_BLINK_RIGHT_DOWN);
	                    }
	                    else
	                    {
	                        Event_Push(
	                            EVENT_BLINK_RIGHT_UP);
	                    }

	                    break;

	                case BUTTON_LIGHT:

	                    if(buttons[button].pressed)
	                    {
	                        Event_Push(
	                            EVENT_LIGHT_DOWN);
	                    }
	                    else
	                    {
	                        Event_Push(
	                            EVENT_LIGHT_UP);
	                    }

	                    break;

	                case BUTTON_HORN:

	                    if(buttons[button].pressed)
	                    {
	                        Event_Push(
	                            EVENT_HORN_ON);
	                    }

	                    break;

	                default:
	                    break;
	            }

	            buttons[button].lastPressed =
	                buttons[button].pressed;
	        }
	}
}
	bool leftPressed =
	    buttons[BUTTON_BLINK_LEFT].pressed;

	bool rightPressed =
	    buttons[BUTTON_BLINK_RIGHT].pressed;

	static bool hazardTriggered = false;

	if(leftPressed && rightPressed)
	{
	    if(!hazardCombinationActive)
	    {
	        hazardCombinationActive = true;

	        hazardStartTime =
	            HAL_GetTick();
	    }

	    if((HAL_GetTick() - hazardStartTime) > 2000)
	    {
	        if(!hazardTriggered)
	        {
	            Event_Push(EVENT_HAZARD_TOGGLE);

	            hazardTriggered = true;
	        }
	    }
	}
	else
	{
	    hazardCombinationActive = false;

	    hazardTriggered = false;
	}
}

bool Button_IsPressed(ButtonId_t button)
{
    return buttons[button].pressed;
}
