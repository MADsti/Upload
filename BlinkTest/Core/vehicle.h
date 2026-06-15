/*
 * vehicle.h
 *
 *  Created on: 15.06.2026
 *      Author: nalle
 */

#ifndef VEHICLE_H_
#define VEHICLE_H_

#include <stdbool.h>
#include "events.h"

typedef struct
{
    bool light;

    bool blinkLeft;
    bool blinkRight;

    bool hazard;

    bool horn;

} VehicleState_t;

void Vehicle_Init(void);

void Vehicle_HandleEvent(Event_t event);

VehicleState_t* Vehicle_GetState(void);


#endif /* VEHICLE_H_ */
