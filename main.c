// CONFIG1L
#pragma config FEXTOSC = HS     // External Oscillator mode Selection bits (HS (crystal oscillator) above 8 MHz; PFM set to high power)
#pragma config RSTOSC = EXTOSC_4PLL// Power-up default value for COSC bits (EXTOSC with 4x PLL, with EXTOSC operating per FEXTOSC bits)

// CONFIG3L
#pragma config WDTCPS = WDTCPS_31// WDT Period Select bits (Divider ratio 1:65536; software control of WDTPS)
#pragma config WDTE = OFF        // WDT operating mode (WDT enabled regardless of sleep)

#include <xc.h>
#include "dc_motor_v1.h"
#include "calibration.h"
#include "color.h"
#include <stdio.h>
#include "serial.h"
#include "pathfinder_file.h"

#define _XTAL_FREQ 64000000 //note intrinsic _delay function is 62.5ns at 64,000,000Hz  

void main(void){
    unsigned int PWMperiod = 99;
    initDCmotorsPWM(PWMperiod);
//~~~~~~~~~~~~~~~~ Left and Right Motor initial conditions ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    //fast decay = coast, slow decay=resisting current
    motorL.power=0;                                     //zero power to start
    motorL.direction=1;                                 //set default motor direction
    motorL.brakemode=1;                                 // brake mode (slow decay)
    motorL.posDutyHighByte=(unsigned char *)(&CCPR1H);  //store address of CCP1 duty high byte
    motorL.negDutyHighByte=(unsigned char *)(&CCPR2H);  //store address of CCP2 duty high byte
    motorL.PWMperiod=PWMperiod;                        //store PWMperiod for motor (value of T2PR in this case)

    motorR.power=0;                                     //zero power to start
    motorR.direction=1;                                 //set default motor direction
    motorR.brakemode=1;                                 // brake mode (slow decay)
    motorR.posDutyHighByte=(unsigned char *)(&CCPR3H);  //store address of CCP3 duty high byte
    motorR.negDutyHighByte=(unsigned char *)(&CCPR4H);  //store address of CCP4 duty high byte
    motorR.PWMperiod=PWMperiod;                         //store PWMperiod for motor (value of T2PR in this case)

         
//~~~~~~~~~~~~~~~ setting up LEDs, buttons ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
        //note: LEFT and right is now from the perspective of the buggy
    // LED Right clicker board 
    TRISDbits.TRISD7 = 0;
    LATDbits.LATD7 = 0;
    
    // LED left clicker board
    TRISHbits.TRISH3 = 0;
    LATHbits.LATH3 = 0;

    // button right clicker board 
    TRISFbits.TRISF2=1; //set TRIS value for pin (input)
    ANSELFbits.ANSELF2=0; //turn off analogue input on pin  
    
    // button left clicker board
    TRISFbits.TRISF3=1; //set TRIS value for pin (input)
    ANSELFbits.ANSELF3=0; //turn off analogue input on pin  
    
 //~~~~~~~~~~~~~~ Calibration variables ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
   
    calibration.index = 1;  // tracking function set up commands the be 1, 2, 3, 4, 5, 6 etc. and then its reverse to be the negative value of it.
       
    calibration.right_90 = 25;
    calibration.left_90 = 25;
    
    calibration.right_135 = 40;
    calibration.left_135 = 40;
    
    calibration.forward_one = 100;
    calibration.backward_one = 100;
    
    calibration.forward_half = 33;
    calibration.backward_half = 33;

  
//~~~~~~~~~~~~~~~ Executable code for the buggy ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    
    while(1){ //calibration selecting loop

        pause_until_RF2_pressed();

        calibration_routine(&calibration, &motorL, &motorR);

        maze_search(&calibration, &motorL, &motorR);
//
        maze_return(&calibration, &motorL, &motorR);

    }
}


// if 20 forward 
// half step back
// 180 rotation 
// execute return command
       
                    

////            //--------------------- SERIAL COMMUNICATION ------------------------------------------
////         char senddata[20]; //Empty char to hold string data
////         
////         for (int i = 0; i < 10; i++) {
////            sprintf(senddata, "%u, ", Operation_History[i]);
////            sendStringSerial4(senddata);
////            __delay_ms(50); // Required delay
////        }
//         
//        //--------------------------------------
//
