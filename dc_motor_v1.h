#ifndef _DC_MOTOR_V1_H
#define _DC_MOTOR_V1_H

#include <xc.h>

#define _XTAL_FREQ 64000000

typedef struct DC_motor { //definition of DC_motor structure
    char power;         //motor power, out of 100
    char direction;     //motor direction, forward(1), reverse(0)
    char brakemode;		// short or fast decay (brake or coast)
    unsigned int PWMperiod; //base period of PWM cycle
    unsigned char *posDutyHighByte; //PWM duty address for motor +ve side
    unsigned char *negDutyHighByte; //PWM duty address for motor -ve side
} DC_motor;

struct DC_motor motorL, motorR;

char temp = 7;
//function prototypes
void initDCmotorsPWM(unsigned int PWMperiod); // function to setup PWM
void setMotorPWM(DC_motor *m);
void stop(DC_motor *mL, DC_motor *mR);
void turnRIGHT(char rotation_calibration, DC_motor *mL, DC_motor *mR);
void turnLEFT(char rotation_calibration, DC_motor *mL, DC_motor *mR);
void fullSpeedAhead(DC_motor *mL, DC_motor *mR);

//void turn (char direction, char rotation, DC_motor *mL, DC_motor *mR);
void forward(char Distance_Calibration, DC_motor *mL, DC_motor *mR);
void backward(char Distance_Calibration, DC_motor *mL, DC_motor *mR);
void delay_ms_function(unsigned int milliseconds);

#endif
