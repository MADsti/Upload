/*
 * can.h
 *
 *  Created on: 13.06.2026
 *      Author: nalle
 */

#ifndef INC_CAN_H_
#define INC_CAN_H_

#include "events.h"
#include <stdbool.h>

#define CAN_ID_EVENT 0x100

void Can_Init(void);

bool Can_SendEvent(Event_t event);

bool Can_GetEvent(Event_t* event);

#endif


