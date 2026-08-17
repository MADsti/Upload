#ifndef INC_BLINKER_H_
#define INC_BLINKER_H_
#include <stdbool.h>
#include "events.h"
void Blinker_HandleEvent(Event_t event);
void Blinker_Init(void);
void Blinker_Update(void);
void Blinker_HazardToggle(void);
bool Blinker_GetPhase(void);
#endif
