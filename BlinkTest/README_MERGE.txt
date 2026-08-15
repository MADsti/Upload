Merged project: STM32G431KBTx own-board basis + application code from MADsti/Upload/BlinkTest.

Hardware/CubeMX basis was preserved from the newly generated project:
- STM32G431KBTx
- HSI + PLL: 170 MHz
- SysTick timebase
- SWD debug
- PA11/PA12 FDCAN1
- own-board GPIO labels from Core/Inc/main.h

Application files integrated from GitHub BlinkTest:
- buttons.c/.h
- blinker.c/.h
- can.c/.h
- can_protocol.h
- events.c/.h
- lights.c/.h
- outputs.c/.h
- vehicle.c/.h

Integration changes required for the own board:
- main.c keeps the newly generated CubeMX hardware initialization.
- GitHub application logic is inserted into main.c.
- GitHub button pin assignments are mapped to the CubeMX-generated GPIO labels.
- GitHub output handling is mapped to all six own-board outputs.
- Nucleo BSP/COM/LED code is not imported.
- FDCAN timing uses the GitHub working values: prescaler 20, SJW 2, seg1 14, seg2 2, classic CAN, auto retransmission enabled.

The original basis main.c/main.h/STM32.ioc are stored in BACKUP_BASIS and are not compiled.
