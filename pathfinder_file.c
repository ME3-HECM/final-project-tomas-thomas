#include <xc.h>
#include "pathfinder_file.h"
#include "color.h"
#include "dc_motor_v1.h"


// function initialise T2 and CCP for DC motor control
void maze_search(calibration_structure *c, DC_motor *mL, DC_motor *mR){
    color_click_init();
    
    while(1){
        __delay_ms(500);
        LATHbits.LATH3 = 1;
        LATDbits.LATD7 = 1; 

        forward(c->forward_one, mL, mR);
        Forward_Count++;
        Color_Value = color_cardCheck(); //reutrns 1 to 8 integer value
        
        LATHbits.LATH3 = 0;
        LATDbits.LATD7 = 0;
        
        //UNABLE TO READ A SET COLOUR 
        //Determines that the buggy is lost in the maze and exist the maze search to then begin return home
        
        if(Forward_Count > forward_reset_threshold){
            backward(c->backward_half, mL, mR); // back off from wall you've just hit
            rightTURN(c->right_90, mL, mR);     //do a 180* rotation
            rightTURN(c->right_90, mL, mR);
            backward(c->backward_one, mL, mR);  //contact the wall to allign rotation  
            for (int i = 0; i < forward_reset_threshold; i++) {
                 forward(c->forward_one, mL, mR);  //moves forward by forward_reset_threshold units
            }
            backward(c->backward_half, mL, mR);     //return to centre of the unit square space
            break;
        }
        
        if(Color_Value != 0){//detects a colour  
            //log the number of times forward
            Operation_History[Operation_Count] = Forward_Count + 10;    //10 offset for reasons
            Forward_Count = 0;
            Operation_Count++;
                    
            if(Color_Value == 1){ //detects that it is red - turn right 90
                Operation_History[Operation_Count] = Color_Value;    //1 = red value 
                Operation_Count++;
                backward(c->backward_half, mL, mR);
                rightTURN(c->right_90, mL, mR);
            }
            
            else if(Color_Value == 2){ //detects that it is green - turn left 90
                Operation_History[Operation_Count] = Color_Value;    //1 = red value 
                Operation_Count++;
                backward(c->backward_half, mL, mR);
                leftTURN(c->left_90, mL, mR);  
            }
           
            else if(Color_Value == 3){ //detects that it is blue - turn 180                     some calibration issues
                Operation_History[Operation_Count] = Color_Value;    //1 = red value 
                Operation_Count++;
                backward(c->backward_half, mL, mR);
                rightTURN(c->right_90, mL, mR);
                rightTURN(c->right_90, mL, mR);
            }
            
            else if(Color_Value == 4){ //detects that it is yellow - reverse 1 square turn right 90                     some calibration issues
                Operation_History[Operation_Count] = Color_Value;    
                Operation_Count++;
                backward(c->backward_half, mL, mR); // half back off from wall
                backward(c->backward_one, mL, mR); //reverse for 1 square
                rightTURN(c->right_90, mL, mR); //turn 90 left
            }
            
            else if(Color_Value == 5){ //detects that it is pink - reverse 1 square turn left 90                     some calibration issues
                Operation_History[Operation_Count] = Color_Value;   
                Operation_Count++;
                backward(c->backward_half, mL, mR); // half back off from wall
                backward(c->backward_one, mL, mR); //reverse for 1 square
                leftTURN(c->left_90, mL, mR);  
            }
            
            else if(Color_Value == 6){ //detects that it is orange - turn right 135                  some calibration issues
                Operation_History[Operation_Count] = Color_Value;    
                Operation_Count++;
                backward(c->backward_half, mL, mR); // half back off from wall
                rightTURN(c->right_135, mL, mR);
            }
            
            else if(Color_Value == 7){ //detects that it is light blue - turn Left 135                  some calibration issues
                Operation_History[Operation_Count] = Color_Value; 
                Operation_Count++;
                backward(c->backward_half, mL, mR); // half back off from wall
                leftTURN(c->left_135, mL, mR);
            }
            
            else if(Color_Value == 8){ //detects that it is white - return home            
                
                backward(c->backward_half, mL, mR); // half back off from wall
                // 180
                rightTURN(c->right_90, mL, mR);
                rightTURN(c->right_90, mL, mR);
                backward(c->backward_one, mL, mR);
                forward(c->forward_half, mL, mR);
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
                for (int j = 0; j < distance_back-1; j++) {
                    forward(c->forward_one, mL, mR);
                }
                forward(c->forward_half, mL, mR);
                // need to have a go back by a half  
            }
//
            else if(Operation_History[i] == 1){  //opposite of right = left
                leftTURN(c->left_90, mL, mR);
                backward(c->backward_one, mL, mR);   
            }

            else if(Operation_History[i] == 2){ //opposite of left = right
                rightTURN(c->right_90, mL, mR);
                backward(c->backward_one, mL, mR);
            }

            else if(Operation_History[i] == 3){ //opposite of 180 = 180
                rightTURN(c->right_90, mL, mR);
                rightTURN(c->right_90, mL, mR);
            }
                     
            else if(Operation_History[i] == 4){ 
                //opposite of yellow
                rightTURN(c->right_90, mL, mR);
                forward(c->forward_one, mL, mR);
                
                //reallignement to wall
                forward(c->forward_one, mL, mR);    //contact with the wall to allign
                backward(c->backward_half, mL, mR); // halfbackwards to move back to center of square
                rightTURN(c->right_90, mL, mR);
                rightTURN(c->right_90, mL, mR);
                backward(c->backward_one, mL, mR); //full backwards
                forward(c->forward_half, mL, mR); //half foward
            }

            else if(Operation_History[i] == 5){
                leftTURN(c->right_90, mL, mR);
                forward(c->forward_one, mL, mR);
                
                //reallignement to wall
                forward(c->forward_one, mL, mR);    //contact with the wall to allign
                backward(c->backward_half, mL, mR); // halfbackwards to move back to center of square
                rightTURN(c->right_90, mL, mR);
                rightTURN(c->right_90, mL, mR);
                backward(c->backward_one, mL, mR); //full backwards
                forward(c->forward_half, mL, mR); //half foward
                
            }

            else if(Operation_History[i] == 6){
                leftTURN(c->left_135, mL, mR);  //counter to right 135
                backward(c->backward_one, mL, mR);  //full backwards
                forward(c->forward_half, mL, mR);  //half forward
            }

            else if(Operation_History[i] == 7){
                rightTURN(c->right_135, mL, mR);
                backward(c->backward_one, mL, mR);  //full backwards
                forward(c->forward_half, mL, mR);  //half forward
            }
            
  
        }
        //reseting the device history so that it can complete another run
        
        Operation_Count = 0;
        for (int i = 0; i < 50; ++i) {
            Operation_History[i] = 0;
        }

        break; //quits the return home function when the history is complete
    }
}