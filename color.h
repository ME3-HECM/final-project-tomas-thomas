#ifndef _color_H
#define _color_H

#include <xc.h>

#define _XTAL_FREQ 64000000 //note intrinsic _delay function is 62.5ns at 64,000,000Hz  


/********************************************//**
 *  Function to initialise the colour click module using I2C
 ***********************************************/
void color_click_init(void);

/********************************************//**
 *  Function to write to the colour click module
 *  address is the register within the colour click to write to
 *	value is the value that will be written to that address
 ***********************************************/

void color_TRILED_ON(void);
//This function turns on all three LEDs (r,g,b) in the tri-color LED on the color-clicker
//Produces white light which is used for detecting cards and their color



void color_writetoaddr(char address, char value);

/********************************************//**
 *  Functions to read the red, green, blue, and clear channels
 *	Returns a 16 bit ADC value representing colour intensity
 ***********************************************/
unsigned int color_read_Red(void);
unsigned int color_read_Green(void);
unsigned int color_read_Blue(void);
unsigned int color_read_Clear(void);

//custom float modulo function to avoid need to import math.h (saves memory usage)
float custom_floatmodulo(float x, float y);

//function which can convert 16bit RGB values measured by RGBC channels on color-clicker to HSV (0-360,0-100,0-100)
void RGB_to_HSV(float R, float G, float B, float C, float &H, float &S, float &V);

//Function to check card color on maze wall, and outputs an integer corresponding to the color
unsigned int color_cardCheck(void);

#endif
