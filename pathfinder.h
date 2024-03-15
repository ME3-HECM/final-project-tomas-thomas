#ifndef _pathfinder_H
#define _pathfinder_H

#include <xc.h>
#include "dc_motor_v1.h"
#include "color.h"
#include "calibration.h"

#define _XTAL_FREQ 64000000 //note intrinsic _delay function is 62.5ns at 64,000,000Hz  


void maze_search(calibration_structure *c, DC_motor *mL, DC_motor *mR);
void maze_return(calibration_structure *c, DC_motor *mL, DC_motor *mR);

char Operation_Count = 0;
char Forward_Count = 0;
char length = 50;       //should be changed
char Operation_History[50] = {0}; //20 slots
char forward_reset_threshold = 10;  //20 forward moves without function to reset

int Color_Value;

#endif