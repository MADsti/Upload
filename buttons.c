#include "buttons.h"
#include "main.h"
#include "events.h"
#include <stdint.h>

typedef struct
{
    GPIO_TypeDef *port;
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
        .port = BlinkLinksIn_GPIO_Port,
        .pin = BlinkLinksIn_Pin
    },

    [BUTTON_BLINK_RIGHT] =
    {
        .port = BlinkRechtsIn_GPIO_Port,
        .pin = BlinkRechtsIn_Pin
    },

    [BUTTON_LIGHT] =
    {
        .port = LichttasterIn_GPIO_Port,
        .pin = LichttasterIn_Pin
    },

    [BUTTON_HORN] =
    {
        .port = HupeIn_GPIO_Port,
        .pin = HupeIn_Pin
    },

    [BUTTON_DISPLAY] =
    {
        .port = Reserve_GPIO_Port,
        .pin = Reserve_Pin
    }
};


void Buttons_Init(void)
{
    for (int i = 0; i < BUTTON_COUNT; i++)
    {
        bool state =
            (HAL_GPIO_ReadPin(
                buttons[i].port,
                buttons[i].pin
            ) == GPIO_PIN_RESET);

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
            (HAL_GPIO_ReadPin(
                b->port,
                b->pin
            ) == GPIO_PIN_RESET);

        /*
         * Entprellung:
         * Wenn sich der Rohzustand ändert,
         * startet die Entprellzeit neu.
         */
        if (raw != b->lastRawState)
        {
            b->lastRawState = raw;
            b->debounceTime = now;
        }

        if ((now - b->debounceTime) < 20)
        {
            continue;
        }

        b->pressed = raw;

        /*
         * Zustandsänderung erkannt
         */
        if (b->pressed != b->lastPressed)
        {
            /*
             * Taste gedrückt
             */
            if (b->pressed)
            {
                b->pressStartTime = now;
                b->longPressTriggered = false;

                switch (button)
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

                    default:
                        break;
                }
            }

            /*
             * Taste losgelassen
             */
            else
            {
                switch (button)
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

        /*
         * Langer Druck auf Lichttaster
         */
        if (button == BUTTON_LIGHT &&
            b->pressed &&
            !b->longPressTriggered &&
            b->pressStartTime != 0 &&
            (now - b->pressStartTime >= 1500))
        {
            b->longPressTriggered = true;

            Event_Push(EVENT_LIGHT_LONG_PRESS);
        }
    }

    /*
     * Warnblinker:
     * Beide Blinker gleichzeitig für mindestens 2 Sekunden halten.
     */
    static bool hazardTriggered = false;

    bool leftPressed =
        buttons[BUTTON_BLINK_LEFT].pressed;

    bool rightPressed =
        buttons[BUTTON_BLINK_RIGHT].pressed;

    if (leftPressed && rightPressed)
    {
        if (!hazardCombinationActive)
        {
            hazardCombinationActive = true;
            hazardStartTime = now;
        }

        if ((now - hazardStartTime) >= 2000 &&
            !hazardTriggered)
        {
            Event_Push(EVENT_HAZARD_TOGGLE);
            hazardTriggered = true;
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
