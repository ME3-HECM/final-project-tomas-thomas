#ifndef _calibration_H
#define _calibration_H

#include <xc.h>
#include "dc_motor_v1.h"

#define _XTAL_FREQ 64000000 //note intrinsic _delay function is 62.5ns at 64,000,000Hz  

typedef struct calibration_structure { //definition of DC_motor structure
    char index;
    char right_90;
    char left_90;
    char right_135;
    char left_135;
    char forward_one;
    char backward_one;
    char forward_half;
    char backward_half;

} calibration_structure;

struct calibration_structure calibration;

void pause_until_RF2_pressed();
void adjust_calibration(int *calibration_label);
void switch_calibration(int *calibration_index);
void calibration_routine(calibration_structure *c, DC_motor *mL, DC_motor *mR );


#endif
