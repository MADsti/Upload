/*
 * outputs.c
 *
 *  Created on: 08.06.2026
 *      Author: nalle
 */

#include "outputs.h"
#include "main.h"
#include "vehicle.h"
#include "blinker.h"
#include <stdbool.h>

Outputs_t outputs =
{
    0
};

void Outputs_Update(void)
{

	VehicleState_t *state = Vehicle_GetState();
	bool blink = Blinker_GetPhase();

	switch(state->blinkMode)
	{
	    case BLINK_LEFT:

	        outputs.leftBlinker  = blink;
	        outputs.rightBlinker = false;
	        break;

	    case BLINK_RIGHT:

	        outputs.leftBlinker  = false;
	        outputs.rightBlinker = blink;
	        break;

	    case BLINK_HAZARD:

	        outputs.leftBlinker  = blink;
	        outputs.rightBlinker = blink;
	        break;

	    default:

	        outputs.leftBlinker  = false;
	        outputs.rightBlinker = false;
	        break;


	}

	outputs.lowBeam =
	    (state->light != LIGHT_OFF);

	outputs.highBeam =
	    (state->light == LIGHT_HIGH);

	outputs.horn =
	    state->horn;


    HAL_GPIO_WritePin(
        LEFT_OUT_GPIO_Port,
        LEFT_OUT_Pin,
        outputs.leftBlinker ?
            GPIO_PIN_SET :
            GPIO_PIN_RESET);

    HAL_GPIO_WritePin(
        RIGHT_OUT_GPIO_Port,
        RIGHT_OUT_Pin,
        outputs.highBeam ?
            GPIO_PIN_SET :
            GPIO_PIN_RESET);

    HAL_GPIO_WritePin(
        LD2_GPIO_Port,
        LD2_Pin,
        outputs.lowBeam ?
            GPIO_PIN_SET :
            GPIO_PIN_RESET);
}
