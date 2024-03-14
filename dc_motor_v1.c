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
void delay_ms_function(unsigned int milliseconds) {
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

//function to stop the robot gradually 
void stop(DC_motor *mL, DC_motor *mR){ //decrement the power slowly
    // Car enters brake mode - motors resist forward momentum
    mL->brakemode = 1;
    mR->brakemode = 1;
    
    //now we need to turn the power down to stop the car going forward
    while(mL->power || mR->power > 0){ //if either motor is on
        if(mL->power > 0 ){
            mL->power--;
            setMotorPWM(mL);
            
        }
        if(mR->power > 0 ){
            mR->power--;
            setMotorPWM(mR);
        }
    __delay_us(800); //controls slow down speed - whats a good slow down time?
    }
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

void forward(char Distance_Calibration, DC_motor *mL, DC_motor *mR){
    mL->direction = 1;
    mR->direction = 1; 
    
    // The plan 
        //Get left and right motor to go at the same speed 
        // making the intital assumption that the controllers might be different
    
    //Speed control variables
    int max_power = 20;         //tunable values
    int acceleration_time = 100;    //[mirco-s]    //check that value is accepted by the delay function
    int delay_time = acceleration_time/max_power;
    
    //acceleration period
    for(int i=0; i< max_power; i++){
        mL->power = mL->power + 1;
        mR->power = mR->power + 1 ; //can I increase this by more than one
        setMotorPWM(mR);
        setMotorPWM(mL);
        delay_ms_function(delay_time); //function to make sure the acceleration for power happens in the correct amount of time 
    }
        //potential to add a bias correcting function to adjust for motor left and motor right imbalance
        //Probs best to increase the power of the week side such that the same RPM is achieved

    //constant velocity    
    for(int j=0; j<Distance_Calibration; j++){
        __delay_ms(10);            //can be reduced to refine the accuracy of distance travelled
    }
    
    //deceleration period
    while(mL->power || mR->power > 0){
        if(mR->power> 0 ){
            mR->power--;
        }
        if(mL->power> 0 ){
            mL->power--;
        }
        setMotorPWM(mR);
        setMotorPWM(mL);
        delay_ms_function(delay_time);    // this could make it more stable if more time?  
    }     
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void backward(char Distance_Calibration, DC_motor *mL, DC_motor *mR){
    mL->direction = 0;
    mR->direction = 0; 
    
    //Speed control variables
    int max_power = 20;         //tunable values
    int acceleration_time = 100;    //[mirco-s]    //check that value is accepted by the delay function
    int delay_time = acceleration_time/max_power;
    
    //acceleration period
    for(int i=0; i< max_power; i++){
        mL->power = mL->power + 1;
        mR->power = mR->power + 1 ; //can I increase this by more than one
        setMotorPWM(mR);
        setMotorPWM(mL);
        delay_ms_function(delay_time); //function to make sure the acceleration for power happens in the correct amount of time 
    }

    //constant velocity    
    for(int j=0; j<Distance_Calibration; j++){
        __delay_ms(10);            //can be reduced to refine the accuracy of distance travelled
    }
    
    //deceleration period
    while(mL->power || mR->power > 0){
        if(mR->power> 0 ){
            mR->power--;
        }
        if(mL->power> 0 ){
            mL->power--;
        }
        setMotorPWM(mR);
        setMotorPWM(mL);
        delay_ms_function(delay_time);    // this could make it more stable if more time?  
    }     
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

//check max pulse I can deliver to motors - thats the max speed
void leftTURN(char rotation_calibration, DC_motor *mL, DC_motor *mR){
    mL->direction = 0;
    mR->direction = 1; 
    
    int max_power = 20;         //tunable values
    int acceleration_time = 100;    //[mirco-s]    //check that value is accepted by the delay function
    int delay_time = acceleration_time/max_power;
    
    //acceleration period
    for(int i=0; i< max_power; i++){
        mL->power = mL->power + 1;
        mR->power = mR->power + 1 ; //can I increase this by more than one
        setMotorPWM(mR);
        setMotorPWM(mL);
        delay_ms_function(delay_time); //function to make sure the acceleration for power happens in the correct amount of time 
    }
        
    //constant velocity    
    for(int j=0; j<rotation_calibration; j++){
        __delay_ms(10);            //can be reduced to refine the accuracy of distance travelled
    }
    
    //deceleration period
    while(mL->power || mR->power > 0){
        if(mR->power> 0 ){
            mR->power--;
        }
        if(mL->power> 0 ){
            mL->power--;
        }
        setMotorPWM(mR);
        setMotorPWM(mL);
        delay_ms_function(delay_time);    // this could make it more stable if more time?  
    }     
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

void rightTURN(char rotation_calibration, DC_motor *mL, DC_motor *mR){
    mL->direction = 1;
    mR->direction = 0; 
    
   int max_power = 20;         //tunable values
    int acceleration_time = 100;    //[mirco-s]    //check that value is accepted by the delay function
    int delay_time = acceleration_time/max_power;
    
    //acceleration period
    for(int i=0; i< max_power; i++){
        mL->power = mL->power + 1;
        mR->power = mR->power + 1 ; //can I increase this by more than one
        setMotorPWM(mR);
        setMotorPWM(mL);
        delay_ms_function(delay_time); //function to make sure the acceleration for power happens in the correct amount of time 
    }
        
    //constant velocity    
    for(int j=0; j<rotation_calibration; j++){
        __delay_ms(10);            //can be reduced to refine the accuracy of distance travelled
    }
    
    //deceleration period
    while(mL->power || mR->power > 0){
        if(mR->power> 0 ){
            mR->power--;
        }
        if(mL->power> 0 ){
            mL->power--;
        }
        setMotorPWM(mR);
        setMotorPWM(mL);
        delay_ms_function(delay_time);    // this could make it more stable if more time?  
    }     
}

