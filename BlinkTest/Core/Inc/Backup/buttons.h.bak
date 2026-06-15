/*
 * buttons.h
 *
 *  Created on: 05.06.2026
 *      Author: nalle
 */

#ifndef INC_BUTTONS_H_
#define INC_BUTTONS_H_

#include <stdbool.h>

void Buttons_Init(void);
void Buttons_Update(void);

typedef enum
{
    BUTTON_BLINK_LEFT,
    BUTTON_BLINK_RIGHT,
    BUTTON_LIGHT,
    BUTTON_HORN,
	BUTTON_DISPLAY,

    BUTTON_COUNT
} ButtonId_t;

bool Button_IsPressed(ButtonId_t button);

#endif /* INC_BUTTONS_H_ */
