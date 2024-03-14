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
        // 1 = forward,     -1 = backward
        // 2 = right,       -2 = left
    
        // read the array backwardss, with a -1 sign, extra zeros in the array get skipped 
    
    calibration.right_90 = 78;
    calibration.left_90 = 77;
    calibration.left_135 = 60;
    calibration.right_135 = 10;
    calibration.forward = 70;
    calibration.backward = 70;
    
    calibration.forward_motorL = 20;
    calibration.forward_motorR = 20;
    
  
//~~~~~~~~~~~~~~~ Calibration Code ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    
    while(1){ //calibration selecting loop
//        while (calibration.over < 2){
        
        //RIGHT
        if(calibration.index == 1){
            adjust_calibration(&calibration.right_90);
            turnRIGHT(calibration.right_90, &motorL, &motorR);  
            switch_calibration(&calibration.index);
        }

        //LEFT
        if(calibration.index == 2){
            adjust_calibration(&calibration.left_90);
            turnLEFT(calibration.left_90, &motorL, &motorR);
            switch_calibration(&calibration.index);
        }
        
        //forward 
        if(calibration.index == 4){
            adjust_calibration(&calibration.forward);
            forward(calibration.forward, &motorL, &motorR);
            switch_calibration(&calibration.index);
        }
        
        if(calibration.index == 5){
            adjust_calibration(&calibration.backward);
            backward(calibration.backward, &motorL, &motorR);
            switch_calibration(&calibration.index);
        }
        
        // to add later
        //180
        //135 right
        //135 left
        
        if(calibration.index == 3){
//            calibration.index = 1; 
            break; //quits us out of the calibration routine
        }     
    }
      
                    
//~~~~~~~~~~~~~~~~~~~~~~ Maze Finding Code ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    char Operation_Count = 0;
    char Forward_Count = 0;
    char length = 20;       //should be changed
    char Operation_History[20] = {0}; //20 slots
    int Color_Value;
    
    color_click_init();
    initUSART4();
    
        //look into pointers and arrays if need requires
    
//    __delay_ms(1000); //delay between calibration and operation
    
    // there's some way to input a calibration struture 
    
    while(1){
        LATHbits.LATH3 = 1;
        LATDbits.LATD7 = 1; 

        forward(calibration.forward, &motorL, &motorR);
//        __delay_ms(10);
        Forward_Count++;
        Color_Value = color_cardCheck(); //reutrns 1 to 8 integer value
        
        LATHbits.LATH3 = 0;
        LATDbits.LATD7 = 0;
//        __delay_ms(10);
        
        if(Color_Value != 0){//detects a colour  
            Operation_History[Operation_Count] = Forward_Count + 10;    //10 offset for reasons
            Forward_Count = 0;
            Operation_Count++;
                    
            if(Color_Value == 1){ //detects that it is red - turn right 90
                Operation_History[Operation_Count] = Color_Value;    //1 = red value 
                Operation_Count++;
                backward(calibration.backward, &motorL, &motorR);
                turnRIGHT(calibration.right_90, &motorL, &motorR); 
            }
            
            
            else if(Color_Value == 2){ //detects that it is green - turn left 90
                Operation_History[Operation_Count] = Color_Value;    //1 = red value 
                Operation_Count++;
                backward(calibration.backward, &motorL, &motorR);
              
                turnLEFT(calibration.left_90, &motorL, &motorR); 
            }
            
            
            else if(Color_Value == 3){ //detects that it is blue - turn 180                     some calibration issues
                Operation_History[Operation_Count] = Color_Value;    //1 = red value 
                Operation_Count++;
                backward(calibration.backward, &motorL, &motorR);
                turnLEFT(calibration.left_90, &motorL, &motorR);
                turnLEFT(calibration.left_90, &motorL, &motorR);
            }
            
            else if(Color_Value == 8){ //detects that it is white - return home            
                
                backward(calibration.backward, &motorL, &motorR);
                // 180
                turnLEFT(calibration.left_90, &motorL, &motorR);
                turnLEFT(calibration.left_90, &motorL, &motorR);
                backward(calibration.backward, &motorL, &motorR);
                backward(calibration.backward, &motorL, &motorR);
                
                for (int i = (length-1); i >= 0; i--) {
                    if(Operation_History[i] == 1){  //opposite of right = left
                        turnLEFT(calibration.left_90, &motorL, &motorR); 
                        backward(calibration.backward, &motorL, &motorR);
                    }
                    else if(Operation_History[i] == 2){ //opposite of left = right
                        turnRIGHT(calibration.right_90, &motorL, &motorR);
                        backward(calibration.backward, &motorL, &motorR);
                    }
                    
                    else if(Operation_History[i] == 3){
                        turnLEFT(calibration.left_90, &motorL, &motorR); 
                        turnLEFT(calibration.left_90, &motorL, &motorR); 
                    }
//                        
//                    }
//                    else if(Operation_History[i] == 4){
//                        //opposite of 1
//                    }
//                    else if(Operation_History[i] == 5){
//                        //opposite of 1
//                    }
//                    else if(Operation_History[i] == 6){
//                        //opposite of 1
//                    }
//                    else if(Operation_History[i] == 7){
//                        //opposite of 1
//                    }
//                    else if(Operation_History[i] == 8){
//                        //opposite of 1
//                    }
                    else if(Operation_History[i] > 10){  
                        unsigned char distance_back = Operation_History[i] - 10;
                        for (int j = 0; j < distance_back; j++) {forward(calibration.forward, &motorL, &motorR);}
                        // need to have a go back by a half
                    }
                    else if(Operation_History[i] == 1){
                        //opposite of 1
                    }
                 
                    
                }
                
                //read reverse of list 
                //adding 
                
                
            }
        }  
    
            //--------------------- SERIAL COMMUNICATION ------------------------------------------
         char senddata[20]; //Empty char to hold string data
         
         for (int i = 0; i < 10; i++) {
            sprintf(senddata, "%u, ", Operation_History[i]);
            sendStringSerial4(senddata);
            __delay_ms(50); // Required delay
        }
         
        //--------------------------------------
    }        
}
