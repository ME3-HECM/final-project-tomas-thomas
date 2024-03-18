#include <xc.h>
#include "pathfinder.h"
#include "color.h"
#include "dc_motor_v1.h"

void maze_search(calibration_structure *c, DC_motor *mL, DC_motor *mR){
    //Maze searching algorythm
    //has two exit conditions - white paper, or forward_reset_threshold moves forward conseqeutively (defined in .h file)
    color_click_init();
    
    while(1){
        //turn LEDs on when the car is moving
        __delay_ms(500);
        LATHbits.LATH3 = 1;
        LATDbits.LATD7 = 1; 

        //moves the car forward, readsd the colour value
        forward(c->forward_one, mL, mR);//move forward for the value set in the calibration.foward_one value set in main
        Forward_Count++;                //add 1 to the forward counter
        Color_Value = color_cardCheck(); //reutrns 1 to 8 integer value
        
        LATHbits.LATH3 = 0;
        LATDbits.LATD7 = 0;
        
        //Failsafe to ensure the buggy always gets home if lost - assumes hitting a solid wall
        //if the forward count exceeds a defined threshold then the buggy will exit the maze finding alogrythm from then the maze return command can be called
        // CAREFUL if the threshold is set too low the buggy will reset in a straight line
        if(Forward_Count > forward_reset_threshold){
            backward(c->backward_half, mL, mR); // back off from wall you've just hit
            rightTURN(c->right_90, mL, mR);     //do a 180* rotation
            rightTURN(c->right_90, mL, mR);
            backward(c->backward_one, mL, mR);  //reverse into the wall to allign rotation  
            for (int i = 0; i < forward_reset_threshold; i++) {
                 forward(c->forward_one, mL, mR);  //moves forward by forward_reset_threshold units 
                                                   //this means that if we were going in a straight line without hitting an obstacle we still would reverse the correct steps to get back home
            }
            backward(c->backward_half, mL, mR);     //since our car has moved 1 unit * a constant away from the edge of the wall we need to move half a step back to return to centre of the unit square space
            break;                                  //exit maze finder algorythm
        }
        
        if(Color_Value != 0){   //detects a colour  
            //save the number of times forward the car went forward in the Operation_History Array
            Operation_History[Operation_Count] = Forward_Count + 10;    //10 offset for reasons
            Forward_Count = 0;  //reset the forward counter
            Operation_Count++;  //increase operation counter 
                    
            if(Color_Value == 1){ //detects that it is red - turn right 90
                Operation_History[Operation_Count] = Color_Value;    //1 = red value 
                Operation_Count++;
                backward(c->backward_half, mL, mR);                     //reverse from wall to create alignment 
                rightTURN(c->right_90, mL, mR);                         //turn right 90 degrees
            }
            
            else if(Color_Value == 2){ //detects that it is green - turn left 90
                Operation_History[Operation_Count] = Color_Value;    //1 = red value 
                Operation_Count++;
                backward(c->backward_half, mL, mR);
                leftTURN(c->left_90, mL, mR);  
            }
           
            else if(Color_Value == 3){ //detects that it is blue - turn 180                    
                Operation_History[Operation_Count] = Color_Value;    //1 = red value 
                Operation_Count++;
                backward(c->backward_half, mL, mR);
                rightTURN(c->right_90, mL, mR);
                rightTURN(c->right_90, mL, mR);
                backward(c->backward_one, mL, mR); //reverse for 1 square
                forward(c->forward_half, mL, mR);     
            }
            
            else if(Color_Value == 4){ //detects that it is yellow - reverse 1 square turn right 90                   
                Operation_History[Operation_Count] = Color_Value;    
                Operation_Count++;
                backward(c->backward_half, mL, mR); // half back off from wall
                backward(c->backward_one, mL, mR); //reverse for 1 square
                rightTURN(c->right_90, mL, mR); //turn 90 left
            }
            
            else if(Color_Value == 5){ //detects that it is pink - reverse 1 square turn left 90                     
                Operation_History[Operation_Count] = Color_Value;   
                Operation_Count++;
                backward(c->backward_half, mL, mR); // half back off from wall
                backward(c->backward_one, mL, mR); //reverse for 1 square
                leftTURN(c->left_90, mL, mR);  
            }
            
            else if(Color_Value == 6){ //detects that it is orange - turn right 135                
                Operation_History[Operation_Count] = Color_Value;    
                Operation_Count++;
                backward(c->backward_half, mL, mR); // half back off from wall
                rightTURN(c->right_135, mL, mR);
            }
            
            else if(Color_Value == 7){ //detects that it is light blue - turn Left 135                 
                Operation_History[Operation_Count] = Color_Value; 
                Operation_Count++;
                backward(c->backward_half, mL, mR); // half back off from wall
                leftTURN(c->left_135, mL, mR);
            }
            
            else if(Color_Value == 8){ //detects that it is white - return home            
                
                backward(c->backward_half, mL, mR); // half back off from wall
                rightTURN(c->right_90, mL, mR);     //180* turn
                rightTURN(c->right_90, mL, mR);
                backward(c->backward_one, mL, mR);  //aligns off backwall
                forward(c->forward_half, mL, mR);   //moves forward half to allign to center of unit square
                break;  //exit the maze finder alogrythm
            }
        }
    }
}



void maze_return(calibration_structure *c, DC_motor *mL, DC_motor *mR){
    //alogrythm to recall buggy movement history and reverse moves to get home
    
    while(1){
        
        for (int i = length; i >= 0; i--) {//read the operation_history array from reverse

            if(Operation_History[i] > 10){  //all values 10 or greater relate to forward movement 
                unsigned char distance_back = Operation_History[i] - 10;    //remove the offset from the forward movement tracker such that 1 = 1 unit of the square moved
                for (int j = 0; j < distance_back-1; j++) {                 //move the buggy forward "distance_back" number of times minus 1 to adjust for calibrating agaisnt the wall
                    forward(c->forward_one, mL, mR);                        //call the forward command to move the buggy forward
                }
            }
            
            else if(Operation_History[i] == 1){  //opposite of right = left
                leftTURN(c->left_90, mL, mR);
                backward(c->backward_one, mL, mR); 
                forward(c->forward_half, mL, mR);
            }

            else if(Operation_History[i] == 2){ //opposite of left = right
                rightTURN(c->right_90, mL, mR);
                backward(c->backward_one, mL, mR);
                forward(c->forward_half, mL, mR);
            }

            else if(Operation_History[i] == 3){ //opposite of 180 = 180
                rightTURN(c->right_90, mL, mR);
                rightTURN(c->right_90, mL, mR);
                backward(c->backward_one, mL, mR); //reverse for 1 square
                forward(c->forward_half, mL, mR);
            }
                     
            else if(Operation_History[i] == 4){ //opposite action of yellow
                //opposite of yellow
                rightTURN(c->right_90, mL, mR);
                forward(c->forward_one, mL, mR);
                
                //reallignement to wall section
                forward(c->forward_one, mL, mR);    //contact with the wall to allign
                backward(c->backward_half, mL, mR); // halfbackwards to move back to center of square
                rightTURN(c->right_90, mL, mR);
                rightTURN(c->right_90, mL, mR);
                backward(c->backward_one, mL, mR); //full backwards
                forward(c->forward_half, mL, mR); //half foward
            }

            else if(Operation_History[i] == 5){ // opposite action of pink
                leftTURN(c->left_90, mL, mR);
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
                rightTURN(c->right_135, mL, mR);    //counter to left 135
                backward(c->backward_one, mL, mR);  //full backwards
                forward(c->forward_half, mL, mR);  //half forward
            }
        }
        
        //reseting the device history so that it can complete another run without turning the device off
        Operation_Count = 0;
        for (char i = 0; i < length; ++i) {
            Operation_History[i] = 0;
        }
        break; //quits the return home function when the history is complete
    }
}