// CONFIG1L
#pragma config FEXTOSC = HS     // External Oscillator mode Selection bits (HS (crystal oscillator) above 8 MHz; PFM set to high power)
#pragma config RSTOSC = EXTOSC_4PLL// Power-up default value for COSC bits (EXTOSC with 4x PLL, with EXTOSC operating per FEXTOSC bits)

// CONFIG3L
#pragma config WDTCPS = WDTCPS_31// WDT Period Select bits (Divider ratio 1:65536; software control of WDTPS)
#pragma config WDTE = OFF        // WDT operating mode (WDT enabled regardless of sleep)

#include <xc.h>
#include "dc_motor_v1.h"
#include "calibration.h"
#include <stdio.h>
#include "color.h"
#include "serial.h"

#define _XTAL_FREQ 64000000 //note intrinsic _delay function is 62.5ns at 64,000,000Hz  

void main(void){
//    unsigned int PWMperiod = 99;
//    initDCmotorsPWM(PWMperiod);
    
    
    color_click_init();
    initUSART4();
    __delay_ms(1000);
    
    while(1) {
        unsigned int a = color_cardCheck();
         __delay_ms(1000);
    }
    
//~~~~~~~~~~~~~~~~ Left and Right Motor initial conditions ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    //fast decay = coast, slow decay=resisting current
//    motorL.power=0;                                     //zero power to start
//    motorL.direction=1;                                 //set default motor direction
//    motorL.brakemode=1;                                 // brake mode (slow decay)
//    motorL.posDutyHighByte=(unsigned char *)(&CCPR1H);  //store address of CCP1 duty high byte
//    motorL.negDutyHighByte=(unsigned char *)(&CCPR2H);  //store address of CCP2 duty high byte
//    motorL.PWMperiod=PWMperiod;                        //store PWMperiod for motor (value of T2PR in this case)
//
//    motorR.power=0;                                     //zero power to start
//    motorR.direction=1;                                 //set default motor direction
//    motorR.brakemode=1;                                 // brake mode (slow decay)
//    motorR.posDutyHighByte=(unsigned char *)(&CCPR3H);  //store address of CCP3 duty high byte
//    motorR.negDutyHighByte=(unsigned char *)(&CCPR4H);  //store address of CCP4 duty high byte
//    motorR.PWMperiod=PWMperiod;                         //store PWMperiod for motor (value of T2PR in this case)
//
//         
////~~~~~~~~~~~~~~~ setting up LEDs, buttons ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//        //note: LEFT and right is now from the perspective of the buggy
//    // LED Right clicker board 
//    TRISDbits.TRISD7 = 0;
//    LATDbits.LATD7 = 0;
//    
//    // LED left clicker board
//    TRISHbits.TRISH3 = 0;
//    LATHbits.LATH3 = 0;
//
//    // button right clicker board 
//    TRISFbits.TRISF2=1; //set TRIS value for pin (input)
//    ANSELFbits.ANSELF2=0; //turn off analogue input on pin  
//    
//    // button left clicker board
//    TRISFbits.TRISF3=1; //set TRIS value for pin (input)
//    ANSELFbits.ANSELF3=0; //turn off analogue input on pin  
//    
// //~~~~~~~~~~~~~~ Calibration variables ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//   
//    calibration.index = 1;  // tracking function set up commands the be 1, 2, 3, 4, 5, 6 etc. and then its reverse to be the negative value of it.
//    calibration.over = 0;
//        // 1 = forward,     -1 = backward
//        // 2 = right,       -2 = left
//    
//        // read the array backwardss, with a -1 sign, extra zeros in the array get skipped 
//    
//    calibration.left_90 = 60;
//    calibration.right_90 = 10;
//    calibration.left_135 = 60;
//    calibration.right_135 = 10;
//    calibration.forward = 10;
//    calibration.forward_motorL = 20;
//    calibration.forward_motorR = 20;
//    
//    
//   
//    
////~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//    
//    while(1){ //calibration selecting loop
////        while (calibration.over < 2){
//        
//        //RIGHT
//        if(calibration.index == 1){
//            adjust_calibration(&calibration.right_90);
//            turnRIGHT(calibration.right_90, &motorL, &motorR);  
//            switch_calibration(&calibration.index);
//        }
//
//        //LEFT
//        if(calibration.index == 2){
//            adjust_calibration(&calibration.left_90);
//            turnLEFT(calibration.left_90, &motorL, &motorR);
//            switch_calibration(&calibration.index);
//        }
//        
//        //forward 
//        if(calibration.index == 3){
//            adjust_calibration(&calibration.forward);
//            forward(calibration.forward, &motorL, &motorR);
//            switch_calibration(&calibration.index);
//        }
//        
//        // to add later
//        //180
//        //135 right
//        //135 left
//        
//        if(calibration.index == 4){
////            calibration.index = 1; 
//            break; //quits us out of the calibration routine
//        }
//        
//    }
}      
                    
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~


