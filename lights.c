#include "lights.h"
#include "blinker.h"
#include "outputs.h"
#include "vehicle.h"
void Lights_Init(void){}
void Lights_Update(void){Blinker_Update(); Outputs_Update();}
