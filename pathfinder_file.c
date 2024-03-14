#include <xc.h>
#include "pathfinder_file.h"
#include "color.h"
#include "dc_motor_v1.h"


// function initialise T2 and CCP for DC motor control
void maze_search(calibration_structure *c, DC_motor *mL, DC_motor *mR){
    color_click_init();
    
    while(1){
        LATHbits.LATH3 = 1;
        LATDbits.LATD7 = 1; 

        forward(c->forward, mL, mR);
        Forward_Count++;
        Color_Value = color_cardCheck(); //reutrns 1 to 8 integer value
        
        LATHbits.LATH3 = 0;
        LATDbits.LATD7 = 0;

        
        if(Color_Value != 0){//detects a colour  
            Operation_History[Operation_Count] = Forward_Count + 10;    //10 offset for reasons
            Forward_Count = 0;
            Operation_Count++;
                    
            if(Color_Value == 1){ //detects that it is red - turn right 90
                Operation_History[Operation_Count] = Color_Value;    //1 = red value 
                Operation_Count++;
                backward(c->backward, mL, mR);
                rightTURN(c->right_90, mL, mR);
            }
            
            else if(Color_Value == 2){ //detects that it is green - turn left 90
                Operation_History[Operation_Count] = Color_Value;    //1 = red value 
                Operation_Count++;
                backward(c->backward, mL, mR);
                leftTURN(c->left_90, mL, mR);  
            }
            
            
            else if(Color_Value == 3){ //detects that it is blue - turn 180                     some calibration issues
                Operation_History[Operation_Count] = Color_Value;    //1 = red value 
                Operation_Count++;
                backward(c->backward, mL, mR);
                rightTURN(c->right_90, mL, mR);
                rightTURN(c->right_90, mL, mR);
            }
            
            else if(Color_Value == 8){ //detects that it is white - return home            
                
                backward(c->backward, mL, mR);
                // 180
                rightTURN(c->right_90, mL, mR);
                rightTURN(c->right_90, mL, mR);
                backward(c->backward, mL, mR);
                backward(c->backward, mL, mR);
                break;
            }
        }
    }
}

void maze_return(calibration_structure *c, DC_motor *mL, DC_motor *mR){
    while(1){
        
        for (int i = length; i >= 0; i--) {
//            forward(c->forward, mL, mR);
            
            if(Operation_History[i] > 10){  
                unsigned char distance_back = Operation_History[i] - 10;
                for (int j = 0; j < distance_back; j++) {
                    forward(c->forward, mL, mR);
                }
                // need to have a go back by a half  
            }
//
            else if(Operation_History[i] == 1){  //opposite of right = left
                leftTURN(c->left_90, mL, mR);
                backward(c->backward, mL, mR);   
            }

            else if(Operation_History[i] == 2){ //opposite of left = right
                rightTURN(c->right_90, mL, mR);
                backward(c->backward, mL, mR);
            }

            else if(Operation_History[i] == 3){
                rightTURN(c->right_90, mL, mR);
                rightTURN(c->right_90, mL, mR);
            }
                     
            else if(Operation_History[i] == 4){
                //opposite of 1
            }

            else if(Operation_History[i] == 5){
                //opposite of 1
            }

            else if(Operation_History[i] == 6){
                //opposite of 1
            }

            else if(Operation_History[i] == 7){
                //opposite of 1
            }

            else if(Operation_History[i] == 8){
                //opposite of 1
            }
            
            else if(Operation_History[i] == 0){
            }
        }
        Operation_Count = 0;
        for (int i = 0; i < 50; ++i) {
            Operation_History[i] = 0;
        }

        break; //quits the return home function when the history is complete
    }
}