
#include "calibration.h"
#include "dc_motor_v1.h"
#include <xc.h>

void pause_until_RF2_pressed(){
    //function that when called requires button RF2 to be pressed to continue
    //in the code this is used to enter the calibration state
    
    while(1){ //continous loop unless broken by if statement
        // flashing LED D7 to indicate this is the button to press
        LATDbits.LATD7 = 1;
        __delay_ms(100);
        LATDbits.LATD7 = 0;
        __delay_ms(100);
        
        if(!PORTFbits.RF2){     //if Right LED is pressed
        __delay_ms(200);        //delay to prevent a double click
        LATDbits.LATD7 = 0;     //turn LED light off
        break;                  //exit while loop
        }
    }
}

void adjust_calibration(int *calibration_label){ 
    //take the calibration variable and if right button pressed increases, if left pressed decreases,
    // if both pressed at same time then exits calibration loop  
    
    while(1){ //calibration adjustment loop          
        
        if(!PORTFbits.RF3 || !PORTFbits.RF2){   //check to see if user has pressed a button
            __delay_ms(100);                    //wait 100ms to account for user input delay between pressing both buttons
                    
            if(!PORTFbits.RF3 && !PORTFbits.RF2 ){ //if both buttons were pressed together within 100ms then exit the calibration lop
                //flash lights to signal that loop has been broken
                LATHbits.LATH3 = 1;
                LATDbits.LATD7 = 1; 
                __delay_ms(1000);
                LATHbits.LATH3 = 0;
                LATDbits.LATD7 = 0;
                break;
            }
            else{
                if(!PORTFbits.RF2){     //if Right button is pressed
                    __delay_ms(200);                                //delay to prevent accidental double click
                    *calibration_label = *calibration_label + 1;    //add 1 to the calibration value input to the function
                    LATDbits.LATD7 = 1;                             //LED flash to indicate the the button has been pressed
                    __delay_ms(200);
                    LATDbits.LATD7 = 0;
                }

                if(!PORTFbits.RF3){ //if Left button is pressed
                    __delay_ms(200);                                //delay to prevent accidental double click
                    *calibration_label = *calibration_label - 1;    //subtract 1 to the calibration value input to the function
                    LATHbits.LATH3 = 1;                             //LED flash to indicate the the button has been pressed
                    __delay_ms(200);
                    LATHbits.LATH3 = 0;
                }
            }
        }
    }
}

void switch_calibration(int *calibration_index){    
    //function switch between the calibration index to move onto the next motor function
    //right button moves on to the next calibration 
    //left button remains on the current calibration 
    
    while(1){
        if(!PORTFbits.RF2){ //if Right button is pressed
            __delay_ms(200);                                        //delay to prevent accidental double click
            *calibration_index = *calibration_index + 1;            //add 1 to the calibration index moving on to the next index
            LATDbits.LATD7 = 1;                                     //LED flash to indicate the the button has been pressed
            __delay_ms(200);
            LATDbits.LATD7 = 0;
            break;                                                  //exit the calibration index selector
        }

        if(!PORTFbits.RF3){ //if Left button is pressed stay on current setting
            __delay_ms(200);                                        //delay to prevent accidental double click
            LATHbits.LATH3 = 1;                                     //LED flash to indicate the the button has been pressed
            __delay_ms(200);
            LATHbits.LATH3 = 0;
            break;                                                  //exit the calibration index selector
        }
    }
}

void calibration_routine(calibration_structure *c, DC_motor *mL, DC_motor *mR){
    //combination of the previous functions to calibrate the 8 movements in a single callable function
    //takes inputs of left motor, right motor, calibration values
    
    while(1){
  
        if(c->index == 1){ //calibrate right 90 turn
            adjust_calibration(&(c->right_90));         //use the left and right buttons to increase or decrease the calibration angle (turn value)
            rightTURN(c->right_90, mL, mR);             //check the turn angle
            switch_calibration(&(c->index));            //use left button to recalibrate turn angle or right button to increase index and move to next movement
        }
        
        if(c->index == 2){ //left 90
            adjust_calibration(&(c->left_90));          //adjust left 90 turn angle
            leftTURN(c->left_90, mL, mR);  
            switch_calibration(&(c->index));
        }
    
        if(c->index == 3){ //right 135
            adjust_calibration(&(c->right_135));        //adjust right 135 turn angle
            rightTURN(c->right_135, mL, mR);  
            switch_calibration(&(c->index));
        }
        if(c->index == 4){ //left 135
            adjust_calibration(&(c->left_135));         //adjust left 135 turn angle
            leftTURN(c->left_135, mL, mR);  
            switch_calibration(&(c->index));
        }
        
        if(c->index == 5 ){ //forward one unit
            adjust_calibration(&(c->forward_one));      //adjust distance for the forward  by 1 unit movement
            forward(c->forward_one, mL, mR);  
            switch_calibration(&(c->index));
        }
        
        if(c->index == 6){ //backward one unit
            adjust_calibration(&(c->backward_one));     //adjust distance for the backward  by 1 unit movement
            backward(c->backward_one, mL, mR);  
            switch_calibration(&(c->index));
        }
        
        if(c->index == 7 ){ //forward half a unit
            adjust_calibration(&(c->forward_half));     //adjust distance for the forward  by 0.5 unit movement
            forward(c->forward_half, mL, mR);  
            switch_calibration(&(c->index));
        }
        
        if(c->index == 8){ //backward half a unit
            adjust_calibration(&(c->backward_half));    //adjust distance for the backward  by 0.5 unit movement
            backward(c->backward_half, mL, mR);  
            switch_calibration(&(c->index));
        }
        
        if(c->index >= 9){                             //when index has completed all 8 functions reset back to 1 to recalibrate back from start
            c->index = 1;                              //set index to 1
            break;                                     //quits us out of the calibration routine
        }
    }
}