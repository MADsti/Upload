/*
 * events.h
 *
 *  Created on: 05.06.2026
 *      Author: nalle
 */

#ifndef INC_EVENTS_H_
#define INC_EVENTS_H_

#include <stdbool.h>

typedef enum
{
    EVENT_NONE = 0,

    EVENT_BLINK_LEFT_DOWN,
	EVENT_BLINK_LEFT_UP,

    EVENT_BLINK_RIGHT_DOWN,
	EVENT_BLINK_RIGHT_UP,

	EVENT_HAZARD_OFF,
	EVENT_HAZARD_ON,

	EVENT_HAZARD_TOGGLE,

    EVENT_LIGHT_TOGGLE,
	EVENT_LIGHT_DOWN,
	EVENT_LIGHT_UP,

    EVENT_HORN_ON,
    EVENT_HORN_OFF

} Event_t;

void Event_Push(Event_t event);

bool Event_Get(Event_t* event);


#endif /* INC_EVENTS_H_ */
