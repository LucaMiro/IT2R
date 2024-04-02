#include "GLCD_Config.h"                // Keil.MCB1700::Board Support:Graphic LCD
#include "Board_GLCD.h"                 // ::Board Support:Graphic LCD
#include "stdio.h"
#include <stdlib.h>
#include <string.h>

extern GLCD_FONT GLCD_Font_16x24 ;
extern GLCD_FONT GLCD_Font_6x8 ;
int main ( void )
{
	float x,y;
	float LONGITUDE,LATITUDE;
	
	LONGITUDE = 00219.6301;
	LATITUDE = 4847.2184;
	
	GLCD_Initialize();
	GLCD_ClearScreen();
	
	GLCD_SetFont(&GLCD_Font_16x24);
	GLCD_SetFont(&GLCD_Font_6x8);
	GLCD_DrawPixel(1,1);
	GLCD_DrawRectangle(55,10,100,140);
	GLCD_DrawRectangle(55,160,200,60);
	GLCD_DrawRectangle(180,10,75,120);
	//GLCD_DrawVLine(320,0,10);
	
	
	x = (int)((4847.2653 - LATITUDE)*(265/0.0646))+55;
	y = (int)((00219.7497 - LONGITUDE )*(240/0.1218));
	//GLCD_DrawPixel(x,y);
	GLCD_SetForegroundColor();
	GLCD_DrawString(x,y,"X");
	//GLCD_DrawHLine(x,y,10);

}