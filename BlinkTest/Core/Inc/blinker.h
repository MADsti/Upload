/*
 * blinker.h
 *
 *  Created on: 08.06.2026
 *      Author: nalle
 */
#include <stdbool.h>
#include "events.h"

#ifndef INC_BLINKER_H_
#define INC_BLINKER_H_

void Blinker_HandleEvent(Event_t event);

void Blinker_Init(void);
void Blinker_Update(void);

void Blinker_HazardToggle(void);
bool Blinker_GetPhase(void);



#endif /* INC_BLINKER_H_ */
