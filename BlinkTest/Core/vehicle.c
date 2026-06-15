#include "vehicle.h"

static VehicleState_t vehicleState;

void Vehicle_Init(void)
{
    vehicleState.light = false;

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
        case EVENT_LIGHT_TOGGLE:
            vehicleState.light = !vehicleState.light;
            break;

        default:
            break;
    }
}
