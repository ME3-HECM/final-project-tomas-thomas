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
   
    calibration.index = 5;  // tracking function set up commands the be 1, 2, 3, 4, 5, 6 etc. and then its reverse to be the negative value of it.
       
    calibration.right_90 = 78;
    calibration.left_90 = 77;
    
    calibration.right_135 = 140;
    calibration.left_135 = 140;
    
    calibration.forward = 70;
    calibration.backward = 40;
  
//~~~~~~~~~~~~~~~ Executable code for the buggy ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    
    while(1){ //calibration selecting loop

        pause_until_RF2_pressed();

        calibration_routine(&calibration, &motorL, &motorR);

        maze_search(&calibration, &motorL, &motorR);
//
        maze_return(&calibration, &motorL, &motorR);

    }
}
       
                    
//~~~~~~~~~~~~~~~~~~~~~~ Maze Finding Code ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//    
//    
//    
//        //look into pointers and arrays if need requires
//    
////    __delay_ms(1000); //delay between calibration and operation
//    
//    // there's some way to input a calibration struture 
//    
//    while(1){
//        LATHbits.LATH3 = 1;
//        LATDbits.LATD7 = 1; 
//
//        forward(calibration.forward, &motorL, &motorR);
////        __delay_ms(10);
//        Forward_Count++;
//        Color_Value = color_cardCheck(); //reutrns 1 to 8 integer value
//        
//        LATHbits.LATH3 = 0;
//        LATDbits.LATD7 = 0;
////        __delay_ms(10);
//        
//        if(Color_Value != 0){//detects a colour  
//            Operation_History[Operation_Count] = Forward_Count + 10;    //10 offset for reasons
//            Forward_Count = 0;
//            Operation_Count++;
//                    
//            if(Color_Value == 1){ //detects that it is red - turn right 90
//                Operation_History[Operation_Count] = Color_Value;    //1 = red value 
//                Operation_Count++;
//                backward(calibration.backward, &motorL, &motorR);
//                turnRIGHT(calibration.right_90, &motorL, &motorR); 
//            }
//            
//            
//            else if(Color_Value == 2){ //detects that it is green - turn left 90
//                Operation_History[Operation_Count] = Color_Value;    //1 = red value 
//                Operation_Count++;
//                backward(calibration.backward, &motorL, &motorR);
//              
//                turnLEFT(calibration.left_90, &motorL, &motorR); 
//            }
//            
//            
//            else if(Color_Value == 3){ //detects that it is blue - turn 180                     some calibration issues
//                Operation_History[Operation_Count] = Color_Value;    //1 = red value 
//                Operation_Count++;
//                backward(calibration.backward, &motorL, &motorR);
//                turnLEFT(calibration.left_90, &motorL, &motorR);
//                turnLEFT(calibration.left_90, &motorL, &motorR);
//            }
//            
//            else if(Color_Value == 8){ //detects that it is white - return home            
//                
//                backward(calibration.backward, &motorL, &motorR);
//                // 180
//                turnLEFT(calibration.left_90, &motorL, &motorR);
//                turnLEFT(calibration.left_90, &motorL, &motorR);
//                backward(calibration.backward, &motorL, &motorR);
//                backward(calibration.backward, &motorL, &motorR);
//                
//                for (int i = (length-1); i >= 0; i--) {
//                    if(Operation_History[i] == 1){  //opposite of right = left
//                        turnLEFT(calibration.left_90, &motorL, &motorR); 
//                        backward(calibration.backward, &motorL, &motorR);
//                    }
//                    else if(Operation_History[i] == 2){ //opposite of left = right
//                        turnRIGHT(calibration.right_90, &motorL, &motorR);
//                        backward(calibration.backward, &motorL, &motorR);
//                    }
//                    
//                    else if(Operation_History[i] == 3){
//                        turnLEFT(calibration.left_90, &motorL, &motorR); 
//                        turnLEFT(calibration.left_90, &motorL, &motorR); 
//                    }
////                        
////                    }
////                    else if(Operation_History[i] == 4){
////                        //opposite of 1
////                    }
////                    else if(Operation_History[i] == 5){
////                        //opposite of 1
////                    }
////                    else if(Operation_History[i] == 6){
////                        //opposite of 1
////                    }
////                    else if(Operation_History[i] == 7){
////                        //opposite of 1
////                    }
////                    else if(Operation_History[i] == 8){
////                        //opposite of 1
////                    }
//                    else if(Operation_History[i] > 10){  
//                        unsigned char distance_back = Operation_History[i] - 10;
//                        for (int j = 0; j < distance_back-1; j++) {forward(calibration.forward, &motorL, &motorR);}
//                        
//                        // need to have a go back by a half
//                    }
//                    else if(Operation_History[i] == 1){
//                        //opposite of 1
//                    }
//                 
//                    
//                }
//                
//                //read reverse of list 
//                //adding 
//                
//                
//            }
//        }  
//    
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
