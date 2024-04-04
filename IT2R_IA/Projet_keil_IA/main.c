#include "LPC17xx.h"                    // Device header
#include "RTE_Device.h"                 // Keil::Device:Startup
#include "RTE_Components.h"             // Component selection
#include "GPIO_LPC17xx.h"               // Keil::Device:GPIO
#include "Driver_USART.h"               // ::CMSIS Driver:USART
#include "Board_GLCD.h"                 // ::Board Support:Graphic LCD
#include "GLCD_Config.h"                // Keil.MCB1700::Board Support:Graphic LCD 


#include <stdio.h>
#include <string.h>


extern ARM_DRIVER_USART Driver_USART3;
extern GLCD_FONT GLCD_Font_16x24; 

void Init_UART(void);
void Init_LCD(void);

int main(void){
		
	
	char STOP[10]; 
	char Texte[10] = "STOP";
	char Texte2[10];
	int i;
		
	Init_UART();
	Init_LCD(); 
	
	while(1){
		
		
		Driver_USART3.Receive(STOP,4);
		while (Driver_USART3.GetRxCount() <4 ) ;
		
	//sprintf(Texte, "%5c", STOP); 
		GLCD_DrawString(0,0,STOP);

		//for (i = 0; i<1000000;i++){GLCD_ClearScreen(); }
		i = strcmp(STOP,Texte);
		sprintf(Texte2, "comp = %d",i);
		GLCD_DrawString(0,70,Texte2);
	

	
	}		
	return 0;
	
}



void Init_UART(void){
	
	Driver_USART3.Initialize(NULL);
	Driver_USART3.PowerControl(ARM_POWER_FULL);
	
	Driver_USART3.Control(ARM_USART_MODE_ASYNCHRONOUS |
							ARM_USART_DATA_BITS_8		|
							ARM_USART_STOP_BITS_1		|
							ARM_USART_PARITY_NONE		|
							ARM_USART_FLOW_CONTROL_NONE,
							115200);
	
	Driver_USART3.Control(ARM_USART_CONTROL_TX,1);
	Driver_USART3.Control(ARM_USART_CONTROL_RX,1);
	
}

void Init_LCD(void){
	
	GLCD_Initialize(); 
	GLCD_SetBackgroundColor(GLCD_COLOR_BLUE);
	GLCD_SetForegroundColor(GLCD_COLOR_YELLOW); 
	GLCD_SetFont(&GLCD_Font_16x24);
	GLCD_ClearScreen();
	
}

