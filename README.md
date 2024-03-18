[![Review Assignment Due Date](https://classroom.github.com/assets/deadline-readme-button-24ddc0f5d75046c5622901739e7c5dd533143b0c8e959d652212380cedb1ea36.svg)](https://classroom.github.com/a/c8ng1gdc)
# Course project - Mine Navigation Search and Rescue (Tomas and Thomas)
## Table of Contents
- [Project Challenge and Instructions](#project-challenge-and-instructions)
- [Introduction](#introduction)
- [Demonstration Video Final Maze](#demonstration-video-final-maze)
- [General Functionality](#general-functionality)
  - [Overview/Instructions for Operation](#overviewinstructions-for-operation)
  - [In Depth View of Code Path](#in-depth-view-of-code-path)
- [Movement Calibration](#movement-calibration)
- [Colour Calibration](#colour-calibration)
- [Exemptions?](#exemptions)
- [Testing Videos (Mechatronics Lab)](#testing-videos-(mechatronics-lab))

## Project Challenge and Instructions
<a name="project-challenge-and-instructions"></a>

### Challenge brief

Your task is to develop an autonomous robot that can navigate a "mine" using a series of instructions coded in coloured cards and return to its starting position.  Your robot must be able to perform the following: 

1. Navigate towards a coloured card and stop before impacting the card
1. Read the card colour
1. Interpret the card colour using a predefined code and perform the navigation instruction
1. When the final card is reached, navigate back to the starting position
1. Handle exceptions and return back to the starting position if final card cannot be found

### "Mine" environment specification

A "mine" is contstructed from black plywood walls 100mm high with some walls having coloured cards located on the sides of the maze to assist with navigation. The following colour code is to be used for navigation:

No | Colour | Instruction
---------|---------|--------
1 | Red | Turn Right 90
2 | Green | Turn Left 90
3 | Blue | Turn 180
4 | Yellow | Reverse 1 square and turn right 90
5 | Pink | Reverse 1 square and turn left 90
6 | Orange | Turn Right 135
7 | Light blue | Turn Left 135 
8 | White | Finish (return home) 
(0) | Black | Maze wall colour

Mine courses will vary in difficulty, with the simplest requiring 4 basic moves to navigate. More advanced courses may require 10 or more moves to navigate. The mines may have features such as dead ends but colour cards will always direct you to the end of the maze. Once the end of the maze has been reached, you must return to the starting position. Colours used in the course or how many is unknown in advance.
Final testing will take place in the CAGB 6th Floor Drawing Room and testing areas will be provided around the mechatronics lab. The exact layout of the maze(s) will not be known in advance. You will also be asked to give a short presentation on the testing day.

## Introduction and Buggy Hardware
<a name="introduction"></a>
This project involved coding a buggy in using a clicker2 board comprising of a PIC18 Advanced microcontroller. The aim of the project was to produce an autonomous buggy, which can navigate its way through an unknown maze using the coloured walls as directions for its operations. It would store these operations, remembering its path through the maze and then return back to the beginning after having completed the maze. In the case of getting lost or hitting a dead end, the buggy would automatically trigger its operation to return back to the beginning of the maze.

This project consisted of 5 key functionalities: Movement, Movement Calibration, Light Sensing, Light Calibration, and Path Memorisation. These are explained in detail in the following sections, along with examples of the code employed and its structure. To complete this task, these key functionalities were split between us and completed in branches on Github and then merged together into our main file. This allowed individual work to be carried out initially, before working together side-by-side on the final functionalities and integration of our work. 

Some hardware modifications were made to the buggy to improve performance. After testing, we found that our specific buggy operated more consistently (moving in straighter lines, turning with less slip) with the rubber wheels removed. We also added a small, black cardboard shield over the Color Click module; this was done to limit ambient light from entering the color click sensing channels and affecting readings. This allowed for more accurate and consistent readings from the color sensor. No further modifications were made.

## Demonstration Video Final Maze
<a name="demonstration-video-final-maze"></a>

Below is a video of the robot solving the final maze in 2 minutes 16 seconsd with only 1 external adjustment. In order to get the video to play within the GitHub ReadMe the file has been heavily compressed - for the high quality recording click the sharepoint link below.

https://github.com/ME3-HECM/final-project-tomas-thomas/assets/156346074/72a4541f-bc0b-4615-bdca-3fec4af7d9b5

https://imperiallondon-my.sharepoint.com/:v:/g/personal/tl2121_ic_ac_uk/EXhYaJbaIzdMlHfeNb4KDAUB8b3JEIibTt1xOKkrldDfHw?e=usuL9M



## General Functionality
<a name="general-functionality"></a>

For the user who is only interested in the functionality of the buggy the procedure for operating it is as follows.

1. Turn the buggy and the clicker board on using their respective swithces
1. Press the buttun RF2 on the clicker board to proceed
1. You are now calibrating the 8 different movements of the buggy
    1. Press the right button to increase the turn/distance
    1. Press the left button to decrease the turn/distance
    1. Press both left and right to view the calibration value
    1. Press the left button to recalibrate the same movement function
    1. Or press the right button to move to the next movement function
    1. Repeat steps 1 through 5 for the 7 remaining movements. 
    1. After the 8th movement has been displayed for the user pressing the right button will begin the maze finding sequence
1. Place the buggy in the center of the square.
1. Press RF2 and the colour sense will turn on and begin solving the maze.
1. The robot will solve the maze or if lost will return back to the start. It will always return home.

These steps can be repeated to run the maze again saving the calibration values inputed from the first calibration.



### Movement Calibration

![image](https://github.com/ME3-HECM/final-project-tomas-thomas/assets/156346074/238bdf45-08f7-42ca-aadb-4db6b7b23573)

Buttons to adjust the calibration value 
When both buttons are pressed together it will save the calibration value and show you the movemnet

![image](https://github.com/ME3-HECM/final-project-tomas-thomas/assets/156346074/e259588b-ef60-41f9-a08e-5fa1c0100360)

Buttons to stay on the currrent calibration, and move to the next calibration

### A More Detailed Overview/Instructions for Operation
<a name="overviewinstructions-for-operation"></a>

The following description is a step-by-step guide to the operation of our buggy, outlining how it meets its required specifications:

1. When the buggy is turned on using the switch in the back it initialising the clicker board's push buttons, LEDs, motors and calibration variables for its movement (turns and movement forwards and backwards).

2. The buggy then begins its executable code, first executing "pause_until_RF2_pressed" function, which awaits a starting press of the RF2 button before beginning the movement calibration process. At this point, the right LED of the clicker2 board flashes continuously until the button is pressed and the next command is executed.

3. Once the RF2 button is pressed, the buggy enters its movement calibration routine, signified by the function **"calibration_routine(&calibration, &motorL, &motorR)"**. This function cycles through the movements for **Right/Left Turn 90 degrees, 135 degrees, Forward and Back 1 unit, and Forward and Back 1/2 unit,** utilising both buttons on the clicker2 board to increase or decrease the magnitude of the movement. A double click (both buttons pressed at the same time) would execute the movement to observe whether it was accurate or not on the surface which the buggy was operating; a left click following this would remain on the same movement and allow right clicks to increase the magnitude and left clicks to decrease the magnitude of the movement; a right click following the motion would signal the calibration routine to save that movement and continue to the next one. This process then continues until all movements are calibrated.

4. On the final movement, a right click will save the final movement calibration and immediately begin execution of the buggy's main maze solving operation, indicated by "maze_search(&calibration, &motorL, &motorR)" in the code below. This function solves the maze by moving the buggy, reading the movement instructions from colored walls using the color sensor, and remembering the operations completed.

5. Once the maze has been completed and the buggy has reached the end point (indicated by a white card), the buggy then executes its final operational command "maze_return(&calibration, &motorL, &motorR)", which takes the saved history of the buggy's movements, reverses them, and performs them to retrace its steps and return back to its starting position at the beginning of the maze.

6. At this point, the buggy's code loops back and it stays stationary until button RF2 is pressed again to re-begin the buggy's executable code and operation. Notably, if the buggy remains turned on, it retains its calibration memory from the first run through, and so after pressing RF2 the calibration routine can be skipped by moving through each movement without further calibration necessary.

 #### Found within main.c:   
    while(1){ 
        //note that the calibration values are not reset if the buggy is not turned off after it has solved the first maze
        //you will still have to go through the calibration routine but no adjustments will have to be made

        pause_until_RF2_pressed();

        calibration_routine(&calibration, &motorL, &motorR);    // calibration routine

        maze_search(&calibration, &motorL, &motorR);            // solve the maze moving using the colours
        
        maze_return(&calibration, &motorL, &motorR);            // return to start function

    }

### In Depth View of Code Path
<a name="in-depth-view-of-code-path"></a>
 
1. Main.c 
    - houses the executable code for the bugg
    - Has the calibration values
1. Calibration.c
    - Responsable for the calibration routine
1. Color.c
    - responsable for the color detection
1. Dc_motor_v1.c
    - Resposnable for communication to the DC motors, along with the called functions for the robot movement.
1. pathfinder.c
    - combined the color functions and movement functions together creating two new functions maze_search and maze_return
    - the pathfinder.h file contains the length variable which determines the operational_history array length (adjustable if a really long maze needs to be solved)
    - the pathfinder.h file also contains the forward_reset_threshold determining how long the robot will be lost for before exectuing the maze_return command
1. serial.c
    - used for colour calibration and testing throughout the project




    

//

#### Found in pathfinder_file.c:
void maze_search(calibration_structure *c, DC_motor *mL, DC_motor *mR){
    //Maze searching algorythm
    //has two exit conditions - white paper, or forward_reset_threshold moves forward conseqeutively (defined in .h file)
    color_click_init();
    
    while(1){
        //turn LEDs on when the car is moving
        __delay_ms(500);
        LATHbits.LATH3 = 1;
        LATDbits.LATD7 = 1; 

        //moves the car forward, reads the colour value
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

This code below shows how if white (end of maze) is read by the color sensor, the buggy will perform a 180 degree turn and end the maze searching function, beginning the maze return home function.

else if(Color_Value == 8){ //detects that it is white - return home            
                
                backward(c->backward_half, mL, mR); // half step back off from wall
                rightTURN(c->right_90, mL, mR);     //180* turn
                rightTURN(c->right_90, mL, mR);
                backward(c->backward_one, mL, mR);  //aligns off back wall
                forward(c->forward_half, mL, mR);   //moves forward half to align to center of unit square
                break;  //exit the maze finder algorithm
            }

Code below is maze_return [needs more here]

void maze_return(calibration_structure *c, DC_motor *mL, DC_motor *mR){
    //algorithm to recall buggy movement history and reverse moves to get home
    
    while(1){
        
        for (int i = length; i >= 0; i--) {//read the operation_history array from reverse

            if(Operation_History[i] > 10){  //all values 10 or greater relate to forward movement 
                unsigned char distance_back = Operation_History[i] - 10;    //remove the offset from the forward movement tracker such that 1 = 1 unit of the square moved
                for (int j = 0; j < distance_back-1; j++) {                 //move the buggy forward distance_back number of times
                    forward(c->forward_one, mL, mR);
                }
                forward(c->forward_half, mL, mR);
                // need to have a go back by a half  - not sure why 
            }
//
            else if(Operation_History[i] == 1){  //opposite of right = left
                leftTURN(c->left_90, mL, mR);
                backward(c->backward_one, mL, mR);   
            }

Code below is maze_return [needs more here]

## Calibration Structure - needs adding to TOC
The calibration code has 4 functions 
```
pause_until_RF2_pressed()
```
This function does not directly control the calibration values but is used to prevent a while(1) loop from running until button RF2 is pressed.
```
adjust_calibration(int *calibration_label)
```
```
switch_calibration(int *calibration_index)
```
```
calibration_routine(calibration_structure *c, DC_motor *mL, DC_motor *mR)
```

// calibration section
### calibration_routine Function
This function combines the 2 previous functions together to calibrate the 8 movements in a single callable callable function. It takes the inputs of left motor, right motor, calibration values.

![image](https://github.com/ME3-HECM/final-project-tomas-thomas/assets/156346074/01a83b85-2115-409e-bd3d-6fd1e7023971)

Summary of the 4 key stages in the buggy code
#### adjustment 
```
    if(c->index == 1){ //calibrate right 90 turn
        adjust_calibration(&(c->right_90));         //use the left and right buttons to increase or decrease the calibration angle (turn value)
        rightTURN(c->right_90, mL, mR);             //calls the right turn at 90 degrees for the user to determine if the calibration was correct
        switch_calibration(&(c->index));            //press left button to re-calibrate turn angle or press right button to increase index and move to next movement
    }
```
This step is then repeated 7 more times to cycle through the 8 total movements that we use whilst solving the maze 

#### Ending the Calibration cycle
```
    if(c->index >= 9){                             //when index has completed all 8 functions reset back to 1 so user can make adjustments on the next run
        c->index = 1;                              //set index to beginning (1)
        break;                                     //quits us out of the calibration routine
    }
    
```
## Movement Structure
<a name="movement-calibration"></a>



### Movement Process
[Explanation of movement process]



## Colour Sensing Structure
<a name="colour-calibration"></a>

### Colour Sensing Process
[Explanation of movement process]

### Colour Calibration
[Explanation of colour calibration process]

## Potential Areas to improve upon?
<a name="exemptions"></a>
[Explanation of any exemptions or exceptions]

currently there is no adjsment made for affect the buggy's battery level has on the motors. We saw that in testing over extended periods of time the buggy's turn radius was affected by the battery level without changing the calibration value. In the future this feature would ensure the buggy would be more consistent over extended operation - however this would be beyond what is required to solve our mazes.

## Testing Videos (Mechatronics Lab)
<a name="testing-videos-(mechatronics-lab)"></a>
The links in this section contain videos showing a run through of a simple maze, made using the test set-up in the Mechatronics Lab on the 7th Floor, and a run through in the case of an exemption - final destination not found (buggy lost).

1. General Functionality (showing color card interpretation, operation, path memorisation and return-to-start execution):

https://github.com/ME3-HECM/final-project-tomas-thomas/assets/156346074/d48db5ad-6e8f-4a5a-a21b-90069f03c462

   https://imperiallondon-my.sharepoint.com/:v:/g/personal/tmy21_ic_ac_uk/Ea9uLu_0IUxFpgJJi82QuJcBaavwkGb7prJe3zGw-A4DUQ?e=4gJrUY

2. Buggy Lost and Returning Home:

https://github.com/ME3-HECM/final-project-tomas-thomas/assets/156346074/9c50fc74-eec5-4896-ad83-c21b7627549f
  
  https://imperiallondon-my.sharepoint.com/:v:/g/personal/tmy21_ic_ac_uk/EYyj4Jv2WW9Muw4ifc2q_ocB3cexHHfqAy9je7XQIkOYDA?e=IdKJQv

