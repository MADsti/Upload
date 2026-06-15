

#ifndef OUTPUTS_H_
#define OUTPUTS_H_

#include <stdbool.h>

typedef struct
{
    bool leftBlinker;
    bool rightBlinker;

    bool lowBeam;
    bool highBeam;

} Outputs_t;

extern Outputs_t outputs;

void Outputs_Update(void);

#endif
