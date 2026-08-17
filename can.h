#ifndef INC_CAN_H_
#define INC_CAN_H_
#include "events.h"
#include <stdbool.h>

#define CAN_ID_HEARTBEAT 0x101
#define CAN_ID_STATUS 0x100

void Can_Init(void);
bool Can_SendEvent(Event_t event);
bool Can_GetEvent(Event_t* event);
bool Can_SendStatusFrame(void);
bool Can_SendHeartbeat(void);

#endif
