#include "GLCD_Config.h"                // Keil.MCB1700::Board Support:Graphic LCD
#include "Board_GLCD.h"                 // ::Board Support:Graphic LCD
#include "stdio.h"
#include <stdlib.h>
#include <string.h>

extern GLCD_FONT GLCD_Font_16x24 ;
extern GLCD_FONT GLCD_Font_6x8 ;
int main ( void )
{
	GLCD_Initialize();
	GLCD_ClearScreen();
	GLCD_SetFont(&GLCD_Font_16x24);
	GLCD_SetFont(&GLCD_Font_6x8);
	//GLCD_DrawPixel(1,1);
	GLCD_DrawRectangle(15,10,80,180);
	GLCD_DrawRectangle(165,170,140,40);
	GLCD_DrawRectangle(165,10,40,120);
}