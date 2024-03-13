#ifndef _calibration_H
#define _calibration_H

#include <xc.h>

#define _XTAL_FREQ 64000000 //note intrinsic _delay function is 62.5ns at 64,000,000Hz  

typedef struct calibration_structure { //definition of DC_motor structure
    char index;
    char over;
    char left_90;
    char right_90;
    char left_135;
    char right_135;
    char forward;
    char backward;
    char forward_motorL;
    char forward_motorR;
} calibration_structure;

struct calibration_structure calibration;

void adjust_calibration(int *calibration_label);
void switch_calibration(int *calibration_index);


#endif
