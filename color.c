#include <xc.h>
#include <stdio.h>
#include "color.h"
#include "i2c.h"
//#include "serial.h"

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
    
    //what other lights should we have on? Ask TOM!
}

void color_writetoaddr(char address, char value){
    I2C_2_Master_Start();         //Start condition
    I2C_2_Master_Write(0x52 | 0x00);     //7 bit device address + Write mode
    I2C_2_Master_Write(0x80 | address);    //command + register address
    I2C_2_Master_Write(value);    
    I2C_2_Master_Stop();          //Stop condition
}


unsigned int color_read_Red(void) //this function reads the red light value from the color sensor (as a 16bit integer)
{
	unsigned int tmp;
	I2C_2_Master_Start();         //Start condition
	I2C_2_Master_Write(0x52 | 0x00);     //7 bit address + Write mode
	I2C_2_Master_Write(0xA0 | 0x16);    //command (auto-increment protocol transaction) + start at RED low register
	I2C_2_Master_RepStart();			// start a repeated transmission
	I2C_2_Master_Write(0x52 | 0x01);     //7 bit address + Read (1) mode
	tmp=I2C_2_Master_Read(1);			//read the Red LSB ((1) means acknowledge that it has been read here)
	tmp=tmp | (I2C_2_Master_Read(0)<<8); //read the Red MSB (don't acknowledge as this is the last read, i.e. (0) in function to set ack=0)
	I2C_2_Master_Stop();          //Stop condition
	return tmp;
}

unsigned int color_read_Green(void) //same function as above but for green light
{
	unsigned int tmp;
	I2C_2_Master_Start();         //Start condition
	I2C_2_Master_Write(0x52 | 0x00);     //7 bit address + Write mode
	I2C_2_Master_Write(0xA0 | 0x18);    //command (auto-increment protocol transaction) + start at Green low register
	I2C_2_Master_RepStart();			// start a repeated transmission
	I2C_2_Master_Write(0x52 | 0x01);     //7 bit address + Read (1) mode
	tmp=I2C_2_Master_Read(1);			//read the Green LSB
	tmp=tmp | (I2C_2_Master_Read(0)<<8); //read the Green MSB (don't acknowledge as this is the last read)
	I2C_2_Master_Stop();          //Stop condition
	return tmp;
}

unsigned int color_read_Blue(void) //same function as above but for blue light
{
	unsigned int tmp;
	I2C_2_Master_Start();         //Start condition
	I2C_2_Master_Write(0x52 | 0x00);     //7 bit address + Write mode
	I2C_2_Master_Write(0xA0 | 0x1A);    //command (auto-increment protocol transaction) + start at Blue low register
	I2C_2_Master_RepStart();			// start a repeated transmission
	I2C_2_Master_Write(0x52 | 0x01);     //7 bit address + Read (1) mode
	tmp=I2C_2_Master_Read(1);			//read the Blue LSB
	tmp=tmp | (I2C_2_Master_Read(0)<<8); //read the Blue MSB (don't acknowledge as this is the last read)
	I2C_2_Master_Stop();          //Stop condition
	return tmp;
}

unsigned int color_read_Clear(void) //same function as above but examining the clear channel on the color sensor
                                    //can use this value to normalise the others (r,g,b) and to interpret brightness of reflected light, indicating when the buggy is near/approaching a colored card
                                    //clear channel provides a measure of the brightness/ambient light
{
	unsigned int tmp;
	I2C_2_Master_Start();         //Start condition
	I2C_2_Master_Write(0x52 | 0x00);     //7 bit address + Write mode
	I2C_2_Master_Write(0xA0 | 0x14);    //command (auto-increment protocol transaction) + start at Clear low register
	I2C_2_Master_RepStart();			// start a repeated transmission
	I2C_2_Master_Write(0x52 | 0x01);     //7 bit address + Read (1) mode
	tmp=I2C_2_Master_Read(1);			//read the Clear LSB
	tmp=tmp | (I2C_2_Master_Read(0)<<8); //read the Clear MSB (don't acknowledge as this is the last read)
	I2C_2_Master_Stop();          //Stop condition
	return tmp;
}


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