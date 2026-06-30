/*
 * can_protocol.h
 *
 *  Created on: 30.06.2026
 *      Author: nalle
 */

#ifndef INC_CAN_PROTOCOL_H_
#define INC_CAN_PROTOCOL_H_

#include <stdint.h>

/* CAN IDs */

#define CAN_ID_EVENT     0x101
#define CAN_ID_STATUS    0x100


/* Status Frame */

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


#endif /* INC_CAN_PROTOCOL_H_ */
