#include "Driver_USART.h"               // ::CMSIS Driver:USART
#include "GLCD_Config.h"                // Keil.MCB1700::Board Support:Graphic LCD
#include "Board_GLCD.h"                 // ::Board Support:Graphic LCD
#include "stdio.h"


extern ARM_DRIVER_USART Driver_USART1;
extern GLCD_FONT GLCD_Font_16x24 ;


void Init_UART(void){
	Driver_USART1.Initialize(NULL);
	Driver_USART1.PowerControl(ARM_POWER_FULL);
	Driver_USART1.Control(	ARM_USART_MODE_ASYNCHRONOUS |
							ARM_USART_DATA_BITS_8		|
							ARM_USART_STOP_BITS_1		|
							ARM_USART_PARITY_EVEN		|
							ARM_USART_FLOW_CONTROL_NONE,
							9600);
	Driver_USART1.Control(ARM_USART_CONTROL_TX,1);
	Driver_USART1.Control(ARM_USART_CONTROL_RX,1);
}

int main (void){
	
	char identite_badge[14] = {0x02,0x33,0x43,0x30,0x30,0x34,0x44,0x39,0x35,0x44,0x32,0x33,0x36,0x03}  ; 
	char tab [14]; // 16 octets = 16 * 8 - 1 = 127
	int i = 0 ;
	Init_UART();
	GLCD_Initialize();
	GLCD_ClearScreen();
	GLCD_SetFont(&GLCD_Font_16x24);
	GLCD_DrawString(0, 1*24,"etape 0");
	
	while (1){
		while(Driver_USART1.GetStatus().tx_busy == 1); // attente buffer TX vide
		GLCD_DrawString(50,100,"etape 1");
		Driver_USART1.Receive(tab,128); // la fonction remplira jusqu'à 128 cases
		GLCD_DrawString(50,100,"etape 2");
		while (Driver_USART1.GetRxCount() <128 ) ; // on attend que 128 cases soient pleines DOUTE !
		GLCD_DrawString(50,100,"etape 3");
		
		
		for ( i = 0 ; i < 14 ; i ++ ) 
		{
			if ( tab[i] == identite_badge[i] )
			{
				GLCD_DrawString(50,100,"BON BADGE !");
			}
			else 
				{
					GLCD_DrawString(50,100,"VOLEUR !");
				}	
		}	
	return 0;
	}
}