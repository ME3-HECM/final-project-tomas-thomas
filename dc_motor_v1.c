#include <xc.h>
#include "dc_motor_v1.h"

// function initialise T2 and CCP for DC motor control
void initDCmotorsPWM(unsigned int PWMperiod){
    //initialise your TRIS and LAT registers for PWM  
    TRISEbits.TRISE2 = 0;
    TRISEbits.TRISE4 = 0;
    TRISCbits.TRISC7 = 0;
    TRISGbits.TRISG6 = 0;

    LATEbits.LATE2 = 0;
    LATEbits.LATE4 = 0;
    LATCbits.LATC7 = 0;
    LATGbits.LATG6 = 0;
    
    //configure PPS to map CCP modules to pins
    RE2PPS=0x05; //CCP1 on RE2
    RE4PPS=0x06; //CCP2 on RE4
    RC7PPS=0x07; //CCP3 on RC7
    RG6PPS=0x08; //CCP4 on RG6

    // timer 2 config
    T2CONbits.CKPS=100; // 1:16 prescaler
    T2HLTbits.MODE=0b00000; // Free Running Mode, software gate only
    T2CLKCONbits.CS=0b0001; // Fosc/4

    // Tpwm*(Fosc/4)/prescaler - 1 = PTPER
    // 0.0001s*16MHz/16 -1 = 99
    T2PR=PWMperiod; // 99 for us Period reg 10kHz base period
    T2CONbits.ON=1;
    
    //setup CCP modules to output PMW signals
    //initial duty cycles 
    CCPR1H=0; 
    CCPR2H=0; 
    CCPR3H=0; 
    CCPR4H=0; 
    
    //use tmr2 for all CCP modules used
    CCPTMRS0bits.C1TSEL=0;
    CCPTMRS0bits.C2TSEL=0;
    CCPTMRS0bits.C3TSEL=0;
    CCPTMRS0bits.C4TSEL=0;
    
    //configure each CCP
    CCP1CONbits.FMT=1; // left aligned duty cycle (we can just use high byte)
    CCP1CONbits.CCP1MODE=0b1100; //PWM mode  
    CCP1CONbits.EN=1; //turn on
    
    CCP2CONbits.FMT=1; // left aligned
    CCP2CONbits.CCP2MODE=0b1100; //PWM mode  
    CCP2CONbits.EN=1; //turn on
    
    CCP3CONbits.FMT=1; // left aligned
    CCP3CONbits.CCP3MODE=0b1100; //PWM mode  
    CCP3CONbits.EN=1; //turn on
    
    CCP4CONbits.FMT=1; // left aligned
    CCP4CONbits.CCP4MODE=0b1100; //PWM mode  
    CCP4CONbits.EN=1; //turn on
    
    TRISHbits.TRISH3 = 0;
    LATHbits.LATH3 = 0;
}

void delay_ms_function(unsigned int milliseconds) {     //simple delay command to create a variable that changes delay length
    while (milliseconds > 0) {
        __delay_ms(1);
        milliseconds--;
    }
}

// function to set CCP PWM output from the values in the motor structure
void setMotorPWM(DC_motor *m)
{
    unsigned char posDuty, negDuty; //duty cycle values for different sides of the motor
    
    if(m->brakemode) {
        posDuty=m->PWMperiod - ((unsigned int)(m->power)*(m->PWMperiod))/100; //inverted PWM duty
        negDuty=m->PWMperiod; //other side of motor is high all the time
    }
    else {
        posDuty=0; //other side of motor is low all the time
		negDuty=((unsigned int)(m->power)*(m->PWMperiod))/100; // PWM duty
    }
    
    if (m->direction) {
        *(m->posDutyHighByte)=posDuty;  //assign values to the CCP duty cycle registers
        *(m->negDutyHighByte)=negDuty;       
    } else {
        *(m->posDutyHighByte)=negDuty;  //do it the other way around to change direction
        *(m->negDutyHighByte)=posDuty;
    }
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

void stop(DC_motor *mL, DC_motor *mR){ //function to decrease the power of the motor slowly 
    // Car enters brake mode - motors resist forward momentum
    mL->brakemode = 1;
    mR->brakemode = 1;
    
    //now we need to turn the power down to stop the car going forward
    while(mL->power || mR->power > 0){ //if either motor is on (with power))
        if(mL->power > 0 ){ //decrease power until power level is 0
            mL->power--;
            setMotorPWM(mL);
            
        }
        if(mR->power > 0 ){ //decrease power until power level is 0
            mR->power--;
            setMotorPWM(mR);
        }
    __delay_us(800); //controls slow down speed of the car 
    }
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

void forward(char Distance_Calibration, DC_motor *mL, DC_motor *mR){
    //function to move the buggy forward a set distance "distance_calibration"
    //accelerates in a given time, moves forward for distance_calibration time, deaccelerates
    
    //set the motors to move forwards
    mL->direction = 1;
    mR->direction = 1; 
    
    //Speed control variables
    char max_power = 40;                             //tunable value for higher speed increase
    char acceleration_time = 100;                    //[mirco-s]    
    char delay_time = acceleration_time/max_power;   //calculates the delay needed to increase the power in the given acceleration_time
                                                     // note that this delay_time value will only be accurate to the nearest ms since it isn't a float
    
    //acceleration period
    for(char i=0; i< max_power; i++){                //until we reach max_power increment left and right motors
        mL->power = mL->power + 1;                  //increment right motor power
        mR->power = mR->power + 1 ;                 //increment left motor power
        setMotorPWM(mR);                            //send new power reading to the setMotorPWM function
        setMotorPWM(mL);                            //send new power reading to the setMotorPWM function
        delay_ms_function(delay_time);              //calls delay function to make sure the acceleration for power happens in the correct amount of time 
    }
        //potential to add a bias correcting function to adjust for motor left and motor right imbalance
        //however testing showed that the car drove straight - if an issue increase the power of the weak side to correct for drift
    
    //constant velocity period   
    for(char j=0; j<Distance_Calibration; j++){  //hold the motors at a constant velocity for 10ms * distance calibration time
        __delay_ms(10);                         //can be reduced to refine the accuracy of distance travelled however 10ms worked for us
    }
    
    //deceleration period
    while(mL->power || mR->power > 0){              //while left or right motor has power
        if(mR->power > 0 ){                         //if non zero reduce power until it is non zero 
            mR->power--;
        }
        if(mL->power > 0 ){                         //if non zero reduce power until it is non zero 
            mL->power--;
        }
        setMotorPWM(mR);                            //send new power reading to the setMotorPWM function
        setMotorPWM(mL);                            //send new power reading to the setMotorPWM function
        delay_ms_function(delay_time);              //calls the delay function to make sure the de-acceleration for the correct amount of time  
    }     
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void backward(char Distance_Calibration, DC_motor *mL, DC_motor *mR){
    //function to move the buggy backwards a set distance "distance_calibration"
    //accelerates in a given time, moves forward for distance_calibration time, deaccelerates
    //same as forward function except motors are driven in reverse. It still is still seperate in case left or right tunning is necessary to correct drift
    
    //set the motors to move backwards
    mL->direction = 0;
    mR->direction = 0; 
    
    //Speed control variables
    char max_power = 40;                             //tunable value for higher speed increase
    char acceleration_time = 100;                    //[mirco-s]    
    char delay_time = acceleration_time/max_power;   //calculates the delay needed to increase the power in the given acceleration_time
    
    //acceleration period
    for(int i=0; i< max_power; i++){                //until we reach max_power increment left and right motors
        mL->power = mL->power + 1;                  //increment right motor power
        mR->power = mR->power + 1 ;                 //increment left motor power
        setMotorPWM(mR);                            //send new power reading to the setMotorPWM function
        setMotorPWM(mL);                            //send new power reading to the setMotorPWM function
        delay_ms_function(delay_time);              //calls delay function to make sure the acceleration for power happens in the correct amount of time 
    }
        //potential to add a bias correcting function to adjust for motor left and motor right imbalance
        //however testing showed that the car drove straight - if an issue increase the power of the weak side to correct for drift
    
    //constant velocity period   
    for(int j=0; j<Distance_Calibration; j++){  //hold the motors at a constant velocity for 10ms * distance calibration time
        __delay_ms(10);                         //can be reduced to refine the accuracy of distance travelled however 10ms worked for us
    }
    
    //deceleration period
    while(mL->power || mR->power > 0){              //while left or right motor has power
        if(mR->power > 0 ){                         //if non zero reduce power until it is non zero 
            mR->power--;
        }
        if(mL->power > 0 ){                         //if non zero reduce power until it is non zero 
            mL->power--;
        }
        setMotorPWM(mR);                            //send new power reading to the setMotorPWM function
        setMotorPWM(mL);                            //send new power reading to the setMotorPWM function
        delay_ms_function(delay_time);              //calls the delay function to make sure the de-acceleration for the correct amount of time  
    }   
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~


void leftTURN(char rotation_calibration, DC_motor *mL, DC_motor *mR){
    //motor direction control 
    mL->direction = 0;  //spin left motors backwards
    mR->direction = 1;  //spin right motors forwards
    
    char max_power = 40;                                     //tunable values
    char acceleration_time = 100;                         //[mirco-s]    //check that value is accepted by the delay function
    char delay_time = acceleration_time/max_power;
    
    //acceleration period
    for(char i=0; i< max_power; i++){                //until we reach max_power increment left and right motors
        mL->power = mL->power + 1;                  //increment right motor power
        mR->power = mR->power + 1 ;                 //increment left motor power
        setMotorPWM(mR);                            //send new power reading to the setMotorPWM function
        setMotorPWM(mL);                            //send new power reading to the setMotorPWM function
        delay_ms_function(delay_time);              //calls delay function to make sure the acceleration for power happens in the correct amount of time 
    }
        //potential to add a bias correcting function to adjust for motor left and motor right imbalance
        //however testing showed that the car drove straight - if an issue increase the power of the weak side to correct for drift
    
    //constant velocity period   
    for(char j=0; j<rotation_calibration; j++){  //hold the motors at a constant velocity for 10ms * distance calibration time
        __delay_ms(10);                         //can be reduced to refine the accuracy of distance travelled however 10ms worked for us
    }
    
    //deceleration period
    while(mL->power || mR->power > 0){              //while left or right motor has power
        if(mR->power > 0 ){                         //if non zero reduce power until it is non zero 
            mR->power--;
        }
        if(mL->power > 0 ){                         //if non zero reduce power until it is non zero 
            mL->power--;
        }
        setMotorPWM(mR);                            //send new power reading to the setMotorPWM function
        setMotorPWM(mL);                            //send new power reading to the setMotorPWM function
        delay_ms_function(delay_time);              //calls the delay function to make sure the de-acceleration for the correct amount of time  
    }  
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

void rightTURN(char rotation_calibration, DC_motor *mL, DC_motor *mR){
    mL->direction = 1;
    mR->direction = 0; 
    
    char max_power = 40;         //tunable values
    char acceleration_time = 100;    //[mirco-s]    //check that value is accepted by the delay function
    char delay_time = acceleration_time/max_power;
    
    //acceleration period
    for(char i=0; i< max_power; i++){                //until we reach max_power increment left and right motors
        mL->power = mL->power + 1;                  //increment right motor power
        mR->power = mR->power + 1 ;                 //increment left motor power
        setMotorPWM(mR);                            //send new power reading to the setMotorPWM function
        setMotorPWM(mL);                            //send new power reading to the setMotorPWM function
        delay_ms_function(delay_time);              //calls delay function to make sure the acceleration for power happens in the correct amount of time 
    }
        //potential to add a bias correcting function to adjust for motor left and motor right imbalance
        //however testing showed that the car drove straight - if an issue increase the power of the weak side to correct for drift
    
    //constant velocity period   
    for(char j=0; j<rotation_calibration; j++){  //hold the motors at a constant velocity for 10ms * distance calibration time
        __delay_ms(10);                         //can be reduced to refine the accuracy of distance travelled however 10ms worked for us
    }
    
    //deceleration period
    while(mL->power || mR->power > 0){              //while left or right motor has power
        if(mR->power > 0 ){                         //if non zero reduce power until it is non zero 
            mR->power--;
        }
        if(mL->power > 0 ){                         //if non zero reduce power until it is non zero 
            mL->power--;
        }
        setMotorPWM(mR);                            //send new power reading to the setMotorPWM function
        setMotorPWM(mL);                            //send new power reading to the setMotorPWM function
        delay_ms_function(delay_time);              //calls the delay function to make sure the de-acceleration for the correct amount of time  
    } 
}

