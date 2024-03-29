[![Review Assignment Due Date](https://classroom.github.com/assets/deadline-readme-button-24ddc0f5d75046c5622901739e7c5dd533143b0c8e959d652212380cedb1ea36.svg)](https://classroom.github.com/a/c8ng1gdc)
# Course Final Project - Mine Navigation Search and Rescue (Tomas and Thomas)
## Table of Contents
- [Project Challenge and Instructions](#project-challenge-and-instructions)
- [Introduction](#introduction)
- [Demonstration Video Final Maze](#demonstration-video-final-maze)
- [General Functionality](#general-functionality)
  - [Overview/Instructions for Operation](#overviewinstructions-for-operation)
  - [Overview of Code Path](#Overview-of-Code-Path)
- [DC Motors](#DC-motors)
- [Calibration Structure](#calibration-structure)
- [Colour Sensing Structure](#colour-sensing-structure)
- [Path Finding Structure](#path-finding-structure)
- [Potential Future Improvements](#potential-future-improvements)
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


### Overview/Instructions for Operation
<a name="overviewinstructions-for-operation"></a>

The following description is a step-by-step guide to the operation of our buggy, outlining how it meets its required specifications:

1. When the buggy is turned on using the switch in the back it initialising the clicker board's push buttons, LEDs, motors and calibration variables for its movement (turns and movement forwards and backwards).

2. The buggy then begins its executable code, first executing "pause_until_RF2_pressed" function, which awaits a starting press of the RF2 button before beginning the movement calibration process. At this point, the right LED of the clicker2 board flashes continuously until the button is pressed and the next command is executed.

3. Once the RF2 button is pressed, the buggy enters its movement calibration routine, signified by the function **"calibration_routine(&calibration, &motorL, &motorR)"**. This function cycles through the movements for **Right/Left Turn 90 degrees, 135 degrees, Forward and Back 1 unit, and Forward and Back 1/2 unit,** utilising both buttons on the clicker2 board to increase or decrease the magnitude of the movement. A double click (both buttons pressed at the same time) would execute the movement to observe whether it was accurate or not on the surface which the buggy was operating; a left click following this would remain on the same movement and allow right clicks to increase the magnitude and left clicks to decrease the magnitude of the movement; a right click following the motion would signal the calibration routine to save that movement and continue to the next one. This process then continues until all movements are calibrated. See the images below for button locations.

4. On the final movement, a right click will save the final movement calibration and immediately begin execution of the buggy's main maze solving operation, indicated by "maze_search(&calibration, &motorL, &motorR)" in the code below. This function solves the maze by moving the buggy, reading the movement instructions from colored walls using the color sensor, and remembering the operations completed. Make sure to place the buggy in the center of the square.

5. Once the maze has been completed and the buggy has reached the end point (indicated by a white card), the buggy then executes its final operational command "maze_return(&calibration, &motorL, &motorR)", which takes the saved history of the buggy's movements, reverses them, and performs them to retrace its steps and return back to its starting position at the beginning of the maze.

6. At this point, the buggy's code loops back and it stays stationary until button RF2 is pressed again to re-begin the buggy's executable code and operation. Notably, if the buggy remains turned on, it retains its calibration memory from the first run through, and so after pressing RF2 the calibration routine can be skipped by moving through each movement without further calibration necessary.

### Movement Calibration Buttons
![image](https://github.com/ME3-HECM/final-project-tomas-thomas/assets/156346074/238bdf45-08f7-42ca-aadb-4db6b7b23573)

First use these buttons to adjust the calibration value 
When both buttons are pressed together it will save the calibration value and show you the movement.

![image](https://github.com/ME3-HECM/final-project-tomas-thomas/assets/156346074/e259588b-ef60-41f9-a08e-5fa1c0100360)

Use these button to either stay on the currrent calibration, and move to the next calibration. 


 ### The 4 key stages in the buggy code found within main.c:   
    while(1){ 
        //note that the calibration values are not reset if the buggy is not turned off after it has solved the first maze
        //you will still have to go through the calibration routine but no adjustments will have to be made

        pause_until_RF2_pressed();

        calibration_routine(&calibration, &motorL, &motorR);    // calibration routine

        maze_search(&calibration, &motorL, &motorR);            // solve the maze moving using the colours
        
        maze_return(&calibration, &motorL, &motorR);            // return to start function

    }

The table below provides a brief summary of each function - helpful to the user when using it:


![image](https://github.com/ME3-HECM/final-project-tomas-thomas/assets/156346074/01a83b85-2115-409e-bd3d-6fd1e7023971)


### Overview of Code Path
<a name="Overview-of-Code-Path"></a>

1. **Main.c** 
    - Houses the executable code for the buggy
    - Contains the calibration values
1. **Dc_motor_v1.c**
    - Resposnable for communication with the DC motors
    - Responsable for the robot movement functions.
1. **Calibration.c**
    - Responsable for the calibration routine
1. **Color.c**
    - Responsable for the color detection
1. **pathfinder.c**
    - Combined color functions and movement functions together creating two new functions maze_search and maze_return
    - The pathfinder.h file contains the "length" variable which determines the operational_history array length (adjustable if a really long maze needs to be solved)
    - The pathfinder.h file also contains the "forward_reset_threshold" determining how long the robot will move before considering itself lost and exectuing the maze_return command
1. **serial.c**
    - Used for colour calibration and testing throughout the project

## DC Motors
<a name="DC-motors"></a>

The motor configuration of the car was built upon the lab 6 work done on the course. The DC_motor_v1 file contains the following functions:
- **initDCmotorsPWM** - intialised the 4 dc motors on the buggy and set up the modules on the clicker board to ouput a PWM signal such that the motor RPM could be controlled by later functions. The PWM period is configured using a 1:16 prescaler with a 64 Mhz internal clock.
- **delay_ms_function** - was used to delay an amount of time equal to a variable as using __delay_ms(variable) does not work.
- **setMotorPWM** - this configures the motors by changing the PWM, allowing us to coast or break, and determine the direction of the motor.
- **stop** - This command slows the car down gradiually. It was not used in the final code as the stop command was integrated into the forward command however it could prove useful for future development
- **forward** - This command takes the DC motor strucutres (left and right) and a calibration value and makes the car go forward for a distance propotional to the calibration value. It first acceleartes to a set power in a set time using the delay_ms function, travels for a configerable distance, then de-accelerates. 
- **backward** - This command is identical to the forward command however the motors move in the opposite direction making the car reverse.
- **leftTURN** - This command makes the right side of the car go forward, and the left side of the car go backwards creating a left rotation on the spot. Similary the acceleration is by reaching a maximum power over a configerable time, roating for a configerable amount of time, and then deaccelreating.
- **rightTURN** - This command is identical to the left turn except in the other direction

Using these functions and the calibration structure below for the different values the movement distances/angles could be calibrated.
```
typedef struct calibration_structure { //definition of DC_motor structure
    char index;
    char right_90;
    char left_90;
    char right_135;
    char left_135;
    char forward_one;
    char backward_one;
    char forward_half;
    char backward_half;

} calibration_structure;
```
This reduced the amount of functions, because for example turnLEFT works for both 90 and 135 degree rotations because of the different calibration values left_90 and left_135.

## Calibration Structure 
<a name="calibration-structure"></a>

The calibration code has 4 functions 
- **pause_until_RF2_pressed()** - This function does not directly control the calibration values but is used to prevent a while(1) loop from running until button RF2 is pressed. This helps the user control the buggy.
- **adjust_calibration(int *->calibration_label)*** - Responsable for increasing or decreasing the calibration values with a right or left button press respectively, and then running the movement for the calibration value when both buttons are pressed.
    ```
        if(!PORTFbits.RF3 || !PORTFbits.RF2){   //check to see if user has pressed a button left or right
            __delay_ms(100);                    //wait 100ms to account for user input delay between pressing both buttons - without this step the double press was unreliable
                
            if(!PORTFbits.RF3 && !PORTFbits.RF2 ){ //if both buttons were pressed together within 100ms then exit the calibration loop
                //flash lights to signal that loop has been broken
                LATHbits.LATH3 = 1;
                LATDbits.LATD7 = 1; 
                __delay_ms(1000);
                LATHbits.LATH3 = 0;
                LATDbits.LATD7 = 0;
                break;
            }
        }
    ```
    - An exert from the function here highlights the use of the or command to solve the timing issue of trying to press two buttons at the same time. This allows a 100ms delay between presses for a better user expereince.
- **switch_calibration(int *->calibration_index)*** - This is a very simple function to increase the calibration index by 1 if the right button is pressed and stay on the current index if left is pressed.
- **calibration_routine(calibration_structure *->c, DC_motor *->mL, DC_motor *->mR)***** - This function combines the 2 previous functions together to calibrate the 8 movements in a single callable callable function. It takes the inputs of left motor, right motor, calibration values.
    - Sample calibration code for a right 90 degree turn
    ```
        if(c->index == 1){ //calibrate right 90 turn
            adjust_calibration(&(c->right_90));         //use the left and right buttons to increase or decrease the calibration angle (turn value)
            rightTURN(c->right_90, mL, mR);             //calls the right turn at 90 degrees for the user to determine if the calibration was correct
            switch_calibration(&(c->index));            //press left button to re-calibrate turn angle or press right button to increase index and move to next movement
        }
    ```
    - This step is then repeated 7 more times to cycle through the 8 total movements that we use whilst solving the maze 
    ```
        if(c->index >= 9){                             //when index has completed all 8 functions reset back to 1 so user can make adjustments on the next run
            c->index = 1;                              //set index to beginning (1)
            break;                                     //quits us out of the calibration routine
        }
    ```
    - Here once again the if statement combined with the while loop (not shown in the snippet - see the full code) is a helpful tool to ensure the code is only doing what we want it to.


## Colour Sensing Structure
<a name="colour-sensing-structure"></a>

### Colour Sensing Process
Colour sensing was required in order to feed instructions to the buggy to solve the maze. Our buggy needed to recognise 9 different colours (see Project Challenge Brief), each with their own specific operation command. 

To do this, firstly a colour initialisation function was called in order to set up the I2C interface required to communicate with the colour sensor and to turn on the tri-colour LED (R,G,B). This tri-colour LED was set to have white light shining looking for card surfaces within the maze. 

#### All the following code found within color.c:
```
void color_click_init(void)
{   
    //setup colour sensor via i2c interface
    I2C_2_Master_Init();      //Initialise i2c Master

     //set device PON
	 color_writetoaddr(0x00, 0x01);
    __delay_ms(3); //need to wait 3ms for everthing to start up
    
    //turn on device ADC
	color_writetoaddr(0x00, 0x03);

    //set integration time
	color_writetoaddr(0x01, 0xD5);
    
    //Turn on the tri-color LED (red, green, and blue) on color-clicker, to have white LED shining looking for card surfaces in maze
    
    //set all LEDs to be outputs
    TRISGbits.TRISG1 = 0; //Set TRIS value for red LED to zero (output)
    TRISAbits.TRISA4 = 0; //Set TRIS value for green LED to zero (output)
    TRISFbits.TRISF7 = 0; //Set TRIS value for blue LED to zero (output)
    
    //Turn on red, green and blue LEDs in tri-color LED to (form white light)
    LATGbits.LATG1 = 1; //Set red LED on
    LATAbits.LATA4 = 1; //Set green LED on
    LATFbits.LATF7 = 1; //Set blue LED on
}
```
This light, when reflected off the card onto the sensor, provided R, G, B values on its read-out, which was used to identify the card colour. The colour sensor contained 4 channels - red, green, blue and clear (representing brightness); these were read to obtain these values using the following function, edited for each channel according to their specific address (e.g. here 0x16 for red LSB):
```
unsigned int color_read_Red(void) //this function reads the red light value from the color sensor (as a 16bit integer)
{
	unsigned int tmp;
	I2C_2_Master_Start();         //Start condition
	I2C_2_Master_Write(0x52 | 0x00);     //7 bit address + Write mode
	I2C_2_Master_Write(0xA0 | 0x16);    //command (auto-increment protocol transaction) + start at RED low register
	I2C_2_Master_RepStart();	    // start a repeated transmission
	I2C_2_Master_Write(0x52 | 0x01);    //7 bit address + Read (1) mode
	tmp=I2C_2_Master_Read(1);	    //read the Red LSB ((1) means acknowledge that it has been read here)
	tmp=tmp | (I2C_2_Master_Read(0)<<8); //read the Red MSB (don't acknowledge as this is the last read, i.e. (0) in function to set ack=0)
	I2C_2_Master_Stop();          //Stop condition
	return tmp;
}
```
These values were converted to HSV (Hue, Saturation, and Value) and then compared against thresholds for each colour card present in the mazes. These thresholds were predetermined by testing; the tests involved taking our sample set of cards, holding them right up against the edge of the buggy (mimicing the distance that the sensor would read colours in practice), and then reading the output using serial communication and a laptop, to obtain the HSV ranges for each card colour.
```
//---------------------USE FOR TESTING WITH SERIAL AND LAPTOP - DETERMINE CARD HSV VALUES------------------------------------------
    char colortest[30]; //Empty char array to contain HSV values and Color determined for color testing
    sprintf(colortest,"H:%.2f S: %.2f V: %.2f C:%u",H,S,V,card_color); //Constructing the string with HSV values and determined color
    sendStringSerial4(colortest); // Sending the string through serial communication
    __delay_ms(50); //Delay needed for buffering, etc.
//---------------------------------------------------------------------------------------------------------------------------------    
```
These ranges were then used within if statements to compare the real-time measured value against the thresholds. If the measured values for HSV fell within the ranges of one of the card colours, it was identified and then the colour card checking function (see below) would return an integer value (0-8), corresponding to that colour. This integer would then be used to trigger the operation required to continue to move through the maze, e.g. at a red card, the function would output 1, and this value would trigger the Turn Right 90 operation elsewhere within the code. 
```
unsigned int color_cardCheck(void) { //function to check the color of the card on the maze wall. Output is an integer corresponding to color
    
    //read 16bit RGB values from each channel on color-clicker
    float r = color_read_Red();
    float g = color_read_Green();
    float b = color_read_Blue();
    
    //read 16bit Clear Channel (brightness) value for normalisation
    float c = color_read_Clear();
    
    //define HSV variables
    float H;
    float S;
    float V;
    
    RGB_to_HSV(r,g,b,c,&H,&S,&V); //convert 16bit RGB values to H (0-360), S (0-100), and V(0-100)
    
    //comparing with predetermined thresholds from testing, to determine which color the card is
    //Labels - 1.Red 2.Green 3.Blue 4.Yellow 5.Pink 6.Orange 7.Light Blue 8. White (0. Black)
    
    unsigned int card_color = 0;
    
    //Check HSV values against known colors for cards - all measured using color clicker and recorded with serial output and laptop
    if (H>355 && H<360 && S>85 && S<90 && V>80 && V<85) {card_color = 1;} //1. Red check
    
    else if (H>67 && H<79 && S>50 && S<57 && V>40 && V<46) {card_color = 2;} //2. Green Check
    
    else if (S<10 && V>30 && V<40) {card_color = 3;} //3. Blue Check - needed expanded bands to work in practice. H value varied too much in tests to be used but S and V were consistent.
    
    else if (H>20 && H<25 && S>65 && S<70 && V>54 && V<57) {card_color = 4;} //4. Yellow Check
    
    else if (H>13 && H<18 && S>55 && S<60 && V>50 && V<54) {card_color = 5;} //5. Pink Check
    
    else if (H>5 && H<10 && S>70 && S<75 && V>62 && V<67) {card_color = 6;} //6. Orange Check
    
    else if (H>74 && H<85 && S>28 && S<33 && V>37 && V<42) {card_color = 7;} //7. Light Blue Check - needed expanded bands to work in practice
    
    else if (H>22 && H<27 && S>48 && S<53 && V>45 && V<50) {card_color = 8;} //8. White Check
    
    //---------------------USE FOR TESTING WITH SERIAL AND LAPTOP - DETERMINE CARD HSV VALUES------------------------------------------
//    char colortest[30]; //Empty char array to contain HSV values and Color determined for color testing
//    sprintf(colortest,"H:%.2f S: %.2f V: %.2f C:%u",H,S,V,card_color); //Constructing the string with HSV values and determined color
//    sendStringSerial4(colortest); // Sending the string through serial communication
//    __delay_ms(50); //Delay needed for buffering, etc.
//---------------------------------------------------------------------------------------------------------------------------------
    
    return card_color; //output the determined color from the function
}
```
### Colour Calibration
It was necessary to read the card colours in varying levels of ambient lighting and with slight differences and imperfections (e.g. dust, dirt, marks) on the actual colour cards used in the mazes. Therefore, colour calibration was important to provide consistent, accurate, and reliable readings. 

The effects of ambient lighting were limited in two ways. Firstly, physically by using a black cardboard shield around the color clicker light sensor, effectively blocking ambient light when reading the colour, with edges flush with the card. The second method was by normalising according to the Clear channel (measure of brightness) on the color clicker. 

The color_cardCheck() shown above reads the clear channel and then uses the value within the RGB-to-HSV conversion function to normalise the RBG readings. This is shown in the code below:
```
void RGB_to_HSV(float R, float G, float B, float C, float *H, float *S, float *V) { //function to convert 16bit RGB values to HSV
    
    //Normalise RGB values to be within the range of 0-1, using the Clear Channel value (i.e. brightness (always greater R,G,B values)
    float r = R/C; //normalised according to clear channel C
    float g = G/C; 
    float b = B/C;
        
    //finding Maximum/Minimum of the RGB values and Difference between them
    float maxRGB = (r > g) ? ((r > b) ? r : b) : ((g > b) ? g : b); //shorthand conditional statements for finding max/min
    float minRGB = (r < g) ? ((r < b) ? r : b) : ((g < b) ? g : b); //? symbol effectively asks condition that precedes it, then continues to either side of colon - if true : if false.
    float deltaRGB = maxRGB - minRGB;
    
    //using standard conversion equations to convert RGB to HSV
    
    //calculating Hue (different equations depending on which color intensity is highest)
    float H_temp;
    
    if (deltaRGB == 0) {H_temp = 0;}
    
    else if (maxRGB == r) {H_temp = custom_floatmodulo((g-b)/deltaRGB, 6.0) * 60;} //using custom float modulo function (modulo by 6)
    
    else if (maxRGB == g) {H_temp = (((b-r)/deltaRGB) + 2) * 60;}
    
    else if (maxRGB == b) {H_temp = (((r-g)/deltaRGB) + 4) * 60;}
    
    if (H_temp < 0) {H_temp = H_temp + 360;}
    //if value is negative, add 360 degrees to make positive angle on color wheel for HUE
    
    *H = H_temp; //assign Hue value (in degrees)
    
    //calculating Saturation
    if (maxRGB == 0) {*S = 0;}
    
    else {
        *S = (deltaRGB/maxRGB) * 100; //Assigning Saturation value. Multiplying by 100 converts the value to a percentage. 
    }
    
    //calculating Value
    *V = maxRGB * 100; //Assigning Value. Multiplying by 100 converts the value to a percentage, and normalised by brightness normalisation factor.
}   
```
The function above converts from 16 bit RGB values to HSV, by first normalising according to the clear channel reading, then finding the maximum and minimum of the normalised RBG values and their difference, using these values in standard conversion equations sourced from online (https://www.rapidtables.com/convert/color/rgb-to-hsv.html), and finally assigning them to their respective variables to be returned. 

This provided much more consistent and reliable results and ranges compared with testing without normalisation, in all lighting conditions. HSV was used for comparison as in testing the ranges overlapped less compared with RGB, and the hue values were more consistent as they were independent of the brightness of the colour card (influenced by ambient lighting and blemishes on the cards). 

These calibration techniques made the colour detection very effective and robust in all lighting conditions and with new or well-worn cards.

Finally, in the code above a custom float modulo function was used in order to find the remainder from the division of two floats (needed in calculating Hue). This was done used to avoid having to import math.h, thus saving memory and making the code more efficient. The function was hard coded into the file as seen below:
```
//custom modulo function for floats to avoid importing math.h and saving memory. % function brings errors with floats, only good with integers, therefore need to use this function
float custom_floatmodulo(float x, float y) { 
    // Ensure y is not zero to avoid division by zero
    if (y == 0.0) {
        return 0.0; // Return 0 if y is zero
    }
    
    // Calculate the quotient
    float quotient = x / y;
    
    // Calculate the integer part of the quotient
    int intPart = (int)quotient;
    
    // Calculate the remainder
    float remainder = x - intPart * y;
    
    return remainder;
}
```
## Path Finding Structure
<a name="path-finding-structure"></a>
There are 2 functions that control the robot movement, **maze_search** and **maze_return**. These functions combine aspects from the motor functions, and colour functions together to solve the maze and return home.

### **maze_search**
First the we intialise the color clicker, Operation_History array, and forward_reset_threshold.

The operation history array is used to log of all the movements done by the buggy with an integer value assigned to each action. Each time a movement is made the Operation_count variable increses. Values 1 through 8 are colour actions and 10 through "n" are forward actions.

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
10 |  | Forward 0 units 
11 |  | Forward 1 unit
12 |  | Forward 2 units 
...||
"n" |  | Forward "n" units 

The buggy is limited by how many times it can go forward by the "forward_reset_threshold" value in the patherfinder.h file as this limit is used to determine if the buggy is lost.

The buggy operates through an easily to follow process:
1. The buggy moves forward 1 unit, incrementing the movement counter, each time checking if it reads a colour. 
    - If no colour is read then it repeats step 1.
    - If a colour is detected it saves the amount of times it has gone forward to the operation history moving to step 2.
1. Using the "color_cardCheck()" function the variable Color_value is assigned the integer asssociated with that colour.
1. The colour detected is then stored in the operation_history array, and the associated movement is made. An example a 90 degree turn for a green card is below:
    ```
    if(Color_Value == 2){ //detects that it is green - turn left 90
        Operation_History[Operation_Count] = Color_Value;    //1 = red value 
        Operation_Count++;
        backward(c->backward_half, mL, mR);
        leftTURN(c->left_90, mL, mR);  
    }
    ```
    - Note that we are using the wall as a reference point hence the half a unit reverse is to allign the buggy to the center of the unit square.

    - Additional adjustements are also made for other moves, such as a blue card (180 degrees).
    ```
    else if(Color_Value == 3){ //detects that it is blue - turn 180                    
        Operation_History[Operation_Count] = Color_Value;    //1 = red value 
        Operation_Count++;
        backward(c->backward_half, mL, mR);
        rightTURN(c->right_90, mL, mR);
        rightTURN(c->right_90, mL, mR);
        backward(c->backward_one, mL, mR); //reverse for 1 square
        forward(c->forward_half, mL, mR);     
    }
    ```
    - After its done the 180 degree turn (or two 90 degree turns), the buggy reverses to the wall alligning itself, and then moves forward half a unit before moving forward again.
    These calibration steps are essential to keep the buggy going straight due to the nature of the floor.
1. The steps 1 through 3 are then repeated until the buggy reads white for a return home or the lost condition is met!

#### Return Home
The return home function causes the car to do a 180 and exit the maze_search function using a break when it detects a white card. From here it is ready to execute the maze_return command.
```
else if(Color_Value == 8){ //detects that it is white - return home            
    backward(c->backward_half, mL, mR); // half back off from wall
    rightTURN(c->right_90, mL, mR);     //180* turn
    rightTURN(c->right_90, mL, mR);
    backward(c->backward_one, mL, mR);  //aligns off backwall
    forward(c->forward_half, mL, mR);   //moves forward half to allign to center of unit square
    break;  //exit the maze finder alogrythm
}
```

#### Lost command
The lost function assumes the robot is hitting a wall or has continued in a straight line uninterupted for "forward_reset_threshold" units of distance. Once this threshold is reached the robot executes the code below.
```
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
```
The buggy first reverses half a unit back, does a 180 and then reverses back into the wall to ensure a perpendicular allignment. This works if the robot has hit a wall or if its just gone too far on a straight. The buggy then goes forward the total distance it has travelled before. 

The buggy reverses half a unit to return to the center of the square from which it exits the maze_search while(1) loop. We have tested this extensively and a video is provided at the end of the read.me


### **maze_return**
Maze return is very similar to maze_search function however instead of reading colour values it reads the Operation_history array in reverse. Numerous if statements then determine what movements the buggy does to return to its starting position.

```
while(1){
    for (int i = length; i >= 0; i--) {//read the operation_history array from reverse
        if(Operation_History[i] > 10){  //all values 10 or greater relate to forward movement 
            unsigned char distance_back = Operation_History[i] - 10;    //remove the offset from the forward movement tracker such that 1 = 1 unit of the square moved
            for (int j = 0; j < distance_back-1; j++) {                 //move the buggy forward "distance_back" number of times minus 1 to adjust for calibrating agaisnt the wall
                forward(c->forward_one, mL, mR);                        //call the forward command to move the buggy forward
            }
        }
```
An example return code for a yellow card:
```
else if(Color_Value == 4){ //detects that it is yellow - reverse 1 square turn right 90                   
    Operation_History[Operation_Count] = Color_Value;    
    Operation_Count++;
    backward(c->backward_half, mL, mR); // half back off from wall
    backward(c->backward_one, mL, mR); //reverse for 1 square
    rightTURN(c->right_90, mL, mR); //turn 90 left
}
```
The same allignmnet philsophy is applied here expect it has to be applied in a different order since the buggy is retracing its steps. 

Because of the way the Operation_history array was defined the unused values are set to zero which in the function doesn't do anything and is skipped. These additional zeros are necessary because if the array is not long enough it will not "remember" all the moves to go home. 

Once the "for loop" is completed and the buggy is back to its starting location we reset our operation_history values for the next run.
```
Operation_Count = 0;
        for (char i = 0; i < length; ++i) {
            Operation_History[i] = 0;
        }
        break; //quits the return home function when the history is complete
```

## Potential Future Improvements
<a name="potential-future-improvements"></a>

Currently, there is no adjustment made for the effect that the buggy's battery level has on the motors. We saw that in testing over extended periods of time the buggy's turn radius was affected by the decreasing battery level, without changing the calibration value. In the future, this feature would ensure the buggy would be more consistent over extended operation - however this would be beyond what is required to solve our mazes.

Another potential improvement would be to have continuous smooth movement of the buggy. This could allow for faster maze solving times and could be completed by adding an interrupt sequence off the color click clear channel to indicate when the buggy has approached a wall/colour card. 

Our color detection worked very well and identified all colours in various operating conditions, but potentially another more robust method and improvement for color detection would be to include an additional calibration sequence to calibrate for the specific, individual colour cards used in each maze. This would be automatic and would read the color values/information from the cards and store it within its color detection ranges, ready for use in solving the maze. This would ensure that all colour cards would be correctly identified, regardless of operating conditions (ambient light, wear-and-tear on the cards, etc.).

## Testing Videos (Mechatronics Lab)
<a name="testing-videos-(mechatronics-lab)"></a>
The links in this section contain videos showing a run through of a simple maze, made using the test set-up in the Mechatronics Lab on the 7th Floor, and a run through in the case of an exemption - final destination not found (buggy lost).

1. General Functionality (showing color card interpretation, operation, path memorisation and return-to-start execution):

https://github.com/ME3-HECM/final-project-tomas-thomas/assets/156346074/d48db5ad-6e8f-4a5a-a21b-90069f03c462

   https://imperiallondon-my.sharepoint.com/:v:/g/personal/tmy21_ic_ac_uk/Ea9uLu_0IUxFpgJJi82QuJcBaavwkGb7prJe3zGw-A4DUQ?e=4gJrUY

2. Buggy Lost and Returning Home:

https://github.com/ME3-HECM/final-project-tomas-thomas/assets/156346074/9c50fc74-eec5-4896-ad83-c21b7627549f
  
  https://imperiallondon-my.sharepoint.com/:v:/g/personal/tmy21_ic_ac_uk/EYyj4Jv2WW9Muw4ifc2q_ocB3cexHHfqAy9je7XQIkOYDA?e=IdKJQv

