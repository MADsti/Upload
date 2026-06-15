/*
 * outputs.c
 *
 *  Created on: 08.06.2026
 *      Author: nalle
 */

#include "outputs.h"
#include "main.h"

Outputs_t outputs =
{
    0
};

void Outputs_Update(void)
{
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
