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

    uint32_t pressStartTime;
    bool longPressTriggered;

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
        buttons[i].pressStartTime = 0;
        buttons[i].longPressTriggered = false;
    }
}


void Buttons_Update(void)
{
    uint32_t now = HAL_GetTick();

    for (int button = 0; button < BUTTON_COUNT; button++)
    {
        Button_t *b = &buttons[button];

        bool raw =
            (HAL_GPIO_ReadPin(b->port, b->pin) == GPIO_PIN_RESET);

        // ---------- Entprellen ----------
        if (raw != b->lastRawState)
        {
            b->lastRawState = raw;
            b->debounceTime = now;
        }

        if ((now - b->debounceTime) < 20)
            continue;

        b->pressed = raw;

        // ---------- Flanke erkannt ----------
        if (b->pressed != b->lastPressed)
        {
            if (b->pressed)
            {
                // Taste wurde gedrückt
                b->pressStartTime = now;
                b->longPressTriggered = false;

                switch(button)
                {
                    case BUTTON_BLINK_LEFT:
                        Event_Push(EVENT_BLINK_LEFT_DOWN);
                        break;

                    case BUTTON_BLINK_RIGHT:
                        Event_Push(EVENT_BLINK_RIGHT_DOWN);
                        break;

                    case BUTTON_HORN:
                        Event_Push(EVENT_HORN_ON);
                        break;

                    case BUTTON_LIGHT:
                        // Nur Timer starten
                        break;

                    default:
                        break;
                }
            }
            else
            {
                // Taste wurde losgelassen
                switch(button)
                {
                    case BUTTON_BLINK_LEFT:
                        Event_Push(EVENT_BLINK_LEFT_UP);
                        break;

                    case BUTTON_BLINK_RIGHT:
                        Event_Push(EVENT_BLINK_RIGHT_UP);
                        break;

                    case BUTTON_HORN:
                        Event_Push(EVENT_HORN_OFF);
                        break;

                    case BUTTON_LIGHT:

                        if (!b->longPressTriggered)
                        {
                            Event_Push(EVENT_LIGHT_PRESS);
                        }

                        b->pressStartTime = 0;
                        b->longPressTriggered = false;

                        break;

                    default:
                        break;
                }
            }

            b->lastPressed = b->pressed;
        }

        // ---------- Long Press ----------
        if (button == BUTTON_LIGHT)
        {
            if (b->pressed &&
                !b->longPressTriggered &&
                b->pressStartTime != 0 &&
                (now - b->pressStartTime >= 1500))
            {
                b->longPressTriggered = true;
                Event_Push(EVENT_LIGHT_LONG_PRESS);
            }
        }
    }

    // ---------- Warnblinker ----------
    bool leftPressed  = buttons[BUTTON_BLINK_LEFT].pressed;
    bool rightPressed = buttons[BUTTON_BLINK_RIGHT].pressed;

    static bool hazardTriggered = false;

    if (leftPressed && rightPressed)
    {
        if (!hazardCombinationActive)
        {
            hazardCombinationActive = true;
            hazardStartTime = now;
        }

        if ((now - hazardStartTime) >= 2000)
        {
            if (!hazardTriggered)
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
