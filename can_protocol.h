#ifndef INC_CAN_PROTOCOL_H_
#define INC_CAN_PROTOCOL_H_

#include <stdint.h>

#define CAN_ID_STATUS      0x100
#define CAN_ID_HEARTBEAT   0x101

#define CAN_NODE_STM32     0x01
#define CAN_NODE_ESP32     0x02

typedef struct
{
    uint8_t lowBeam;
    uint8_t highBeam;
    uint8_t leftBlinker;
    uint8_t rightBlinker;
    uint8_t horn;
    uint8_t speed;
    uint8_t reserved1;
    uint8_t reserved2;
} CanStatusFrame_t;

typedef struct
{
    uint8_t node;
    uint8_t counter;
} CanHeartbeatFrame_t;

#endif
