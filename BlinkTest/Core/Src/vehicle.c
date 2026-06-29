#include "vehicle.h"
#include "events.h"
#include <stdio.h>

static VehicleState_t vehicleState;

void Vehicle_Init(void)
{
	vehicleState.light = LIGHT_OFF;
	vehicleState.blinkMode = BLINK_OFF;
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
        break;

    case EVENT_LIGHT_LONG_PRESS:

        vehicleState.light = LIGHT_OFF;
        break;


    case EVENT_BLINK_LEFT_DOWN:

        if(vehicleState.blinkMode == BLINK_LEFT)
            vehicleState.blinkMode = BLINK_OFF;
        else
            vehicleState.blinkMode = BLINK_LEFT;

        break;

    case EVENT_BLINK_RIGHT_DOWN:

        if(vehicleState.blinkMode == BLINK_RIGHT)
            vehicleState.blinkMode = BLINK_OFF;
        else
            vehicleState.blinkMode = BLINK_RIGHT;

        break;

    case EVENT_HAZARD_TOGGLE:

        if(vehicleState.blinkMode == BLINK_HAZARD)
            vehicleState.blinkMode = BLINK_OFF;
        else
            vehicleState.blinkMode = BLINK_HAZARD;

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

