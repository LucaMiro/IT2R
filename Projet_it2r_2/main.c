#include "Driver_USART.h"               // ::CMSIS Driver:USART
#include "GLCD_Config.h"                // Keil.MCB1700::Board Support:Graphic LCD
#include "Board_GLCD.h"                 // ::Board Support:Graphic LCD
#include "stdio.h"
#include <stdlib.h>
#include <string.h>

extern ARM_DRIVER_USART Driver_USART1;
extern GLCD_FONT GLCD_Font_16x24 ;


void Init_UART(void){
	Driver_USART1.Initialize(NULL);
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
	char tab [36]; // MESSAGE ID : 6 --- UTC TIME = 10 --- LATITUDE = 9 -- N/S = 1 -- LONGITUDE = 10  
	int i = 0 ;
	char MESSAGE_ID[6],UTC_TIME[10],LATITUDE[9],INDICATOR[1],LONGITUDE[10];
	Init_UART();
	GLCD_Initialize();
	GLCD_ClearScreen();
	GLCD_SetFont(&GLCD_Font_16x24);

	while (1){
		while(Driver_USART1.GetStatus().tx_busy == 1); // attente buffer TX vide
		//GLCD_DrawString(0, 2*24,"etape 1");
		
		Driver_USART1.Receive(tab,36); 
		//GLCD_DrawString(0, 3*24,"etape 2");
		
		while (Driver_USART1.GetRxCount() < 36 ) ; 
	//GLCD_DrawString(0, 4*24,"etape 3 ");
		
		if ((tab[0] == '$')&& (tab[4] == 'G') && (tab[5] == 'A'))
		{
		for ( i = 0 ; tab[i]!=0 ; i++)
   {
       if (tab[i] == ',')
       {
           tab[i] = ' ' ;
       }
    }
		//sscanf(tab,"%s",MESSAGE_ID);
	 sscanf(tab,"%s %s %s %s %s",MESSAGE_ID,UTC_TIME,LATITUDE,INDICATOR,LONGITUDE);
	 
	 GLCD_DrawString(6*24,6*24,MESSAGE_ID);
		}
		else {
			GLCD_DrawString(2*24,2*24,"ERREUR");
		}
	}
}