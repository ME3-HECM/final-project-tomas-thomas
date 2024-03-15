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
[Link to demonstration video showcasing the final maze]
The following links show all unknown mazes completed as part of the Final Testing Lab Session:
1.
2.
3.
4.

## General Functionality
<a name="general-functionality"></a>
[Explanation of general functionality of the project]

### Overview/Instructions for Operation
<a name="overviewinstructions-for-operation"></a>

The following description is a step-by-step guide to the operation of our buggy, outlining how it meets its required specifications:

1. The buggy is programmed and turned on, initialising the buggy's push buttons, LEDs, motors and calibration variables for its movement (turns and movement forwards and backwards).

2. The buggy then begins its executable code, first executing "pause_until_RF2_pressed" function, which awaits a starting press of the RF2 button before beginning the movement calibration process. At this point, the right LED of the clicker2 board flashes continuously until the button is pressed and the next command is executed.

3. Once the RF2 button is pressed, the buggy enters its movement calibration routine, signified by the function "calibration_routine(&calibration, &motorL, &motorR)" in the code below. This function cycles through the movements for Right/Left Turn 90 degrees, 135 degrees, Forward and Back 1 unit, and Forward and Back 1/2 unit, utilising both buttons on the clicker2 board to increase or decrease the magnitude of the movement. A double click (both buttons pressed at the same time) would execute the movement to observe whether it was accurate or not on the surface which the buggy was operating; a left click following this would remain on the same movement and allow right clicks to increase the magnitude and left clicks to decrease the magnitude of the movement; a right click following the motion would signal the calibration routine to save that movement and continue to the next one. This process then continues until all movements are calibrated.

4. On the final movement, a right click will save the final movement calibration and immediately begin execution of the buggy's main maze solving operation, indicated by "maze_search(&calibration, &motorL, &motorR)" in the code below. This function solves the maze by moving the buggy, reading the movement instructions from colored walls using the color sensor, and remembering the operations completed.

5. Once the maze has been completed and the buggy has reached the end point (indicated by a white card), the buggy then executes its final operational command "maze_return(&calibration, &motorL, &motorR)", which takes the saved history of the buggy's movements, reverses them, and performs them to retrace its steps and return back to its starting position at the beginning of the maze.

6. At this point, the buggy's code loops back and it stays stationary until button RF2 is pressed again to re-begin the buggy's executable code and operation. Notably, if the buggy remains turned on, it retains its calibration memory from the first run through, and so after pressing RF2 the calibration routine can be skipped by moving through each movement without further calibration necessary.

 #### Found within main.c   
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
[Detailed explanation of the code path]

## Movement Structure
<a name="movement-calibration"></a>

### Movement Process
[Explanation of movement process]

### Movement Calibration

## Colour Sensing Structure
<a name="colour-calibration"></a>

### Colour Sensing Process
[Explanation of movement process]

### Colour Calibration
[Explanation of colour calibration process]

## Exemptions?
<a name="exemptions"></a>
[Explanation of any exemptions or exceptions]

## Testing Videos (Mechatronics Lab)
<a name="testing-videos-(mechatronics-lab)"></a>
The links in this section contain videos showing a run through of a simple maze, made using the test set-up in the Mechatronics Lab on the 7th Floor, and a run through in the case of an exemption - final destination not found (buggy lost).

1. General Functionality (showing color card interpretation, operation, path memorisation and return-to-start execution):

   https://imperiallondon-my.sharepoint.com/:v:/g/personal/tmy21_ic_ac_uk/Ea9uLu_0IUxFpgJJi82QuJcBaavwkGb7prJe3zGw-A4DUQ?e=4gJrUY

2. Buggy Lost and Returning Home:

  https://imperiallondon-my.sharepoint.com/:v:/g/personal/tmy21_ic_ac_uk/EYyj4Jv2WW9Muw4ifc2q_ocB3cexHHfqAy9je7XQIkOYDA?e=IdKJQv

