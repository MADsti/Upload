#include "can.h"
#include "main.h"
#include "vehicle.h"
#include "outputs.h"
#include "can_protocol.h"

extern FDCAN_HandleTypeDef hfdcan1;
static uint8_t heartbeatCounter = 0;

void Can_Init(void)
{
}

bool Can_GetEvent(Event_t *event)
{
    (void)event;

    return false;
}

bool Can_SendStatusFrame(void)
{
    FDCAN_TxHeaderTypeDef txHeader;
    CanStatusFrame_t frame = {0};

    frame.lowBeam = outputs.lowBeam;
    frame.highBeam = outputs.highBeam;
    frame.leftBlinker = outputs.leftBlinker;
    frame.rightBlinker = outputs.rightBlinker;
    frame.horn = outputs.horn;
    frame.speed = 0;

    txHeader.Identifier = CAN_ID_STATUS;
    txHeader.IdType = FDCAN_STANDARD_ID;
    txHeader.TxFrameType = FDCAN_DATA_FRAME;
    txHeader.DataLength = FDCAN_DLC_BYTES_8;
    txHeader.ErrorStateIndicator = FDCAN_ESI_ACTIVE;
    txHeader.BitRateSwitch = FDCAN_BRS_OFF;
    txHeader.FDFormat = FDCAN_CLASSIC_CAN;
    txHeader.TxEventFifoControl = FDCAN_NO_TX_EVENTS;
    txHeader.MessageMarker = 0;

    return HAL_FDCAN_AddMessageToTxFifoQ(
        &hfdcan1,
        &txHeader,
        (uint8_t *)&frame
    ) == HAL_OK;
}


bool Can_SendHeartbeat(void)
{
    FDCAN_TxHeaderTypeDef txHeader = {0};
    uint8_t data[2];

    data[0] = 0x01;
    data[1] = heartbeatCounter++;

    txHeader.Identifier = 0x101;
    txHeader.IdType = FDCAN_STANDARD_ID;
    txHeader.TxFrameType = FDCAN_DATA_FRAME;
    txHeader.DataLength = FDCAN_DLC_BYTES_2;
    txHeader.ErrorStateIndicator = FDCAN_ESI_ACTIVE;
    txHeader.BitRateSwitch = FDCAN_BRS_OFF;
    txHeader.FDFormat = FDCAN_CLASSIC_CAN;
    txHeader.TxEventFifoControl = FDCAN_NO_TX_EVENTS;
    txHeader.MessageMarker = 0;

    HAL_StatusTypeDef result =
        HAL_FDCAN_AddMessageToTxFifoQ(
            &hfdcan1,
            &txHeader,
            data
        );

    return result == HAL_OK;
}
