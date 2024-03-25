#include "Driver_USART.h"               // ::CMSIS Driver:USART
#include "GLCD_Config.h"                // Keil.MCB1700::Board Support:Graphic LCD
#include "Board_GLCD.h"                 // ::Board Support:Graphic LCD
#include "stdio.h"
#include <stdlib.h>
#include <string.h>

#define GPS_DATA_SIZE 70

void myUSART_callback(uint32_t event);

extern ARM_DRIVER_USART Driver_USART1;
extern GLCD_FONT GLCD_Font_16x24 ;


void Init_UART(void){
	Driver_USART1.Initialize(myUSART_callback);
	Driver_USART1.PowerControl(ARM_POWER_FULL);
	Driver_USART1.Control(	ARM_USART_MODE_ASYNCHRONOUS |
							ARM_USART_DATA_BITS_8		|
							ARM_USART_STOP_BITS_1		|
							ARM_USART_PARITY_NONE		|
							ARM_USART_FLOW_CONTROL_NONE,
							9600);
	Driver_USART1.Control(ARM_USART_CONTROL_TX,1);
	Driver_USART1.Control(ARM_USART_CONTROL_RX,1);
}		

int main (void)
{
	char  MESSAGE_ID [7], UTC_TIME [11], LATITUDE[10],LONGITUDE[11], NS[2]; 
	char gps_data[GPS_DATA_SIZE];
	int i ;
	Init_UART();
	GLCD_Initialize();
	GLCD_ClearScreen();
	GLCD_SetFont(&GLCD_Font_16x24);
	
	while(1) {
		
		Driver_USART1.Receive(gps_data,1); 
		while (Driver_USART1.GetRxCount() <1 ) ;
		
		if (gps_data[i] == "$")
		{
			GLCD_DrawString(0,1,"$");
		}
		
		Driver_USART1.Receive(gps_data,5); 
		while (Driver_USART1.GetRxCount() <5 ) ;
		
		if ((gps_data[4] == "G")&&((gps_data[5] == "A")))
			{
			GLCD_DrawString(0,1,"R"); //limiter l'affichage
			}	
	}
}
