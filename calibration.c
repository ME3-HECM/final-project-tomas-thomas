
#include "calibration.h"
#include "dc_motor_v1.h"
#include <xc.h>

void pause_until_RF2_pressed(){
    while(1){ //press to start button
        // flashing to indicate press this button to start the sequence
        LATDbits.LATD7 = 1;
        __delay_ms(100);
        LATDbits.LATD7 = 0;
        __delay_ms(100);
        
        if(!PORTFbits.RF2){     //if Right LED is pressed
        __delay_ms(200);
        LATDbits.LATD7 = 0;
        break;
        }
    }
}

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
                    *calibration_label = *calibration_label -2;
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

void calibration_routine(calibration_structure *c, DC_motor *mL, DC_motor *mR){
    while(1){
        //////////////////////////////////
  
        if(c->index == 1){ //right 90
            adjust_calibration(&(c->right_90));
            rightTURN(c->right_90, mL, mR);  
            switch_calibration(&(c->index));
        }
        
        if(c->index == 2){ //left 90
            adjust_calibration(&(c->left_90));
            leftTURN(c->left_90, mL, mR);  
            switch_calibration(&(c->index));
        }
    
        if(c->index == 3){ //right 135
            adjust_calibration(&(c->right_135));
            rightTURN(c->right_135, mL, mR);  
            switch_calibration(&(c->index));
        }
        if(c->index == 4){ //left 135
            adjust_calibration(&(c->left_135));
            leftTURN(c->left_135, mL, mR);  
            switch_calibration(&(c->index));
        }
        
        if(c->index == 5 ){ //forward
            adjust_calibration(&(c->forward));
            forward(c->forward, mL, mR);  
            switch_calibration(&(c->index));
        }
        
        if(c->index == 6){ //backward
            adjust_calibration(&(c->backward));
            backward(c->backward, mL, mR);  
            switch_calibration(&(c->index));
        }
        
        if(c->index == 7){//this value should be 7 - currently 3 for testing
            c->index = 1; 
            break; //quits us out of the calibration routine
        }
    }
}