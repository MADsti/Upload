#include "can.h"
#include "main.h"
#include "vehicle.h"
#include "outputs.h"
#include "can_protocol.h"

extern FDCAN_HandleTypeDef hfdcan1;

void Can_Init(void)
{
}


bool Can_SendEvent(Event_t event)
{
    FDCAN_TxHeaderTypeDef txHeader;

    uint8_t data[1];

    data[0] = (uint8_t)event;

    txHeader.Identifier = CAN_ID_EVENT;
    txHeader.IdType = FDCAN_STANDARD_ID;
    txHeader.TxFrameType = FDCAN_DATA_FRAME;
    txHeader.DataLength = FDCAN_DLC_BYTES_1;
    txHeader.ErrorStateIndicator = FDCAN_ESI_ACTIVE;
    txHeader.BitRateSwitch = FDCAN_BRS_OFF;
    txHeader.FDFormat = FDCAN_CLASSIC_CAN;
    txHeader.TxEventFifoControl = FDCAN_NO_TX_EVENTS;
    txHeader.MessageMarker = 0;

    return
        HAL_FDCAN_AddMessageToTxFifoQ(
            &hfdcan1,
            &txHeader,
            data)
        == HAL_OK;
}

bool Can_GetEvent(Event_t* event)
{
    return false;
}

bool Can_SendStatusFrame(void)
{

    FDCAN_TxHeaderTypeDef txHeader;

    CanStatusFrame_t frame = {0};

    frame.lowBeam      = outputs.lowBeam;
    frame.highBeam     = outputs.highBeam;

    frame.leftBlinker  = outputs.leftBlinker;
    frame.rightBlinker = outputs.rightBlinker;

    frame.horn         = outputs.horn;

    frame.speed        = 0;

    txHeader.Identifier = CAN_ID_STATUS;
    txHeader.IdType = FDCAN_STANDARD_ID;
    txHeader.TxFrameType = FDCAN_DATA_FRAME;
    txHeader.DataLength = FDCAN_DLC_BYTES_8;

    txHeader.ErrorStateIndicator = FDCAN_ESI_ACTIVE;
    txHeader.BitRateSwitch = FDCAN_BRS_OFF;
    txHeader.FDFormat = FDCAN_CLASSIC_CAN;

    txHeader.TxEventFifoControl = FDCAN_NO_TX_EVENTS;
    txHeader.MessageMarker = 0;

    bool ok =
        HAL_FDCAN_AddMessageToTxFifoQ(
            &hfdcan1,
            &txHeader,
			(uint8_t *)&frame) == HAL_OK;


    return ok;

}
