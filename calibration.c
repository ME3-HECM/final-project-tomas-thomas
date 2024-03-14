
#include "calibration.h"
#include "dc_motor_v1.h"
#include <xc.h>

void adjust_calibration(int *calibration_label){ //take the calibration variable and if right button pressed increases, if left pressed decreases,
                                                 // if both pressed at same time then exits calibration loop  
    
    while(1){ //calibration adjustment loop            
        
        if(!PORTFbits.RF3 || !PORTFbits.RF2){   //check to see if user has pressed a button
            __delay_ms(100);                    //wait 100ms to account for user input delay between pressing both buttons
                    
            if(!PORTFbits.RF3 && !PORTFbits.RF2 ){ //if both buttons were pressed together for 100ms then exit the calibration lop
                //flash lights then exit the loop
                LATHbits.LATH3 = 1;
                LATDbits.LATD7 = 1; 
                __delay_ms(1000);
                LATHbits.LATH3 = 0;
                LATDbits.LATD7 = 0;
                break;
            }
            else{
                if(!PORTFbits.RF2){     //if Right LED is pressed
                    __delay_ms(200);
                    *calibration_label = *calibration_label + 2;
                    LATDbits.LATD7 = 1;
                    __delay_ms(200);
                    LATDbits.LATD7 = 0;
                }

                if(!PORTFbits.RF3){     //if Left LED is pressed
                    __delay_ms(200);
                    *calibration_label = *calibration_label - 2;
                    LATHbits.LATH3 = 1;
                    __delay_ms(200);
                    LATHbits.LATH3 = 0;
                }
            }
        }
    }
}

void switch_calibration(int *calibration_index){
    while(1){
        if(!PORTFbits.RF2){     //if Right LED is pressed
            __delay_ms(200);
            *calibration_index = *calibration_index + 1;
            LATDbits.LATD7 = 1;
            __delay_ms(200);
            LATDbits.LATD7 = 0;
            break;
        }

        if(!PORTFbits.RF3){     //if Left LED is pressed
            __delay_ms(200);
            LATHbits.LATH3 = 1;
            __delay_ms(200);
            LATHbits.LATH3 = 0;
            break;
        }
    }
}