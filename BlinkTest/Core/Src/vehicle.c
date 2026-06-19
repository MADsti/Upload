#include "vehicle.h"
#include "events.h"
#include <stdio.h>

static VehicleState_t vehicleState;

void Vehicle_Init(void)
{
	vehicleState.light = LIGHT_OFF;

    vehicleState.blinkLeft = false;
    vehicleState.blinkRight = false;

    vehicleState.hazard = false;

    vehicleState.horn = false;
}

VehicleState_t* Vehicle_GetState(void)
{
    return &vehicleState;
}

void Vehicle_HandleEvent(Event_t event)
{
    switch(event)
    {
    case EVENT_LIGHT_PRESS:

        switch(vehicleState.light)
        {
            case LIGHT_OFF:
                vehicleState.light = LIGHT_LOW;
                break;

            case LIGHT_LOW:
                vehicleState.light = LIGHT_HIGH;
                break;

            case LIGHT_HIGH:
                vehicleState.light = LIGHT_LOW;
                break;
        }
        printf("LIGHT=%d\r\n", vehicleState.light);
        break;

    case EVENT_LIGHT_LONG_PRESS:

        vehicleState.light = LIGHT_OFF;
        break;

        case EVENT_BLINK_LEFT_DOWN:
            vehicleState.blinkLeft = true;
            break;

        case EVENT_BLINK_LEFT_UP:
            vehicleState.blinkLeft = false;
            break;

        case EVENT_BLINK_RIGHT_DOWN:
            vehicleState.blinkRight = true;
            break;

        case EVENT_BLINK_RIGHT_UP:
            vehicleState.blinkRight = false;
            break;

        case EVENT_HAZARD_ON:
            vehicleState.hazard = true;
            break;

        case EVENT_HAZARD_OFF:
            vehicleState.hazard = false;
            break;

        case EVENT_HAZARD_TOGGLE:
            vehicleState.hazard = !vehicleState.hazard;
            break;

        case EVENT_HORN_ON:
            vehicleState.horn = true;
            break;

        case EVENT_HORN_OFF:
            vehicleState.horn = false;
            break;

        default:
            break;
    }
}

