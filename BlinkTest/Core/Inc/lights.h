/*
 * licht.h
 *
 *  Created on: 05.06.2026
 *      Author: nalle
 */

#ifndef INC_LIGHTS_H_
#define INC_LIGHTS_H_

#include <stdbool.h>

void Lights_Init(void);
void Lights_Update(void);

void Lights_ToggleBlinkLeft(void);
void Lights_BlinkLeftDown(void);
void Lights_BlinkLeftUp(void);
void Lights_BlinkRightDown(void);
void Lights_BlinkRightUp(void);
void Lights_HazardToggle(void);

void Lights_LightDown(void);
void Lights_LightUp(void);

void Lights_ToggleLowBeam(void);

bool Lights_IsBlinkLeftActive(void);

#endif /* INC_LIGHTS_H_ */
