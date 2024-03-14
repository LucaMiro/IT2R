#include "Driver_USART.h"               // ::CMSIS Driver:USART
#include "GLCD_Config.h"                // Keil.MCB1700::Board Support:Graphic LCD
#include "Board_GLCD.h"                 // ::Board Support:Graphic LCD
#include "stdio.h"

extern ARM_DRIVER_USART Driver_USART1;
extern GLCD_FONT GLCD_Font_16x24 ;

void Init_UART_RFID(void){
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




void Identification_RFID (void)
{
	char bon_badge;
	char identite_badge[14] = {0x02,0x33,0x43,0x30,0x30,0x34,0x44,0x39,0x35,0x44,0x32,0x33,0x36,0x03}  ; 
	char tab [14]; // 14 octets = 14 * 8 - 1 = 111
	int i = 0 ;
	Init_UART_RFID();
	GLCD_Initialize();
	GLCD_ClearScreen();
	GLCD_SetFont(&GLCD_Font_16x24);
	//GLCD_DrawString(0, 1*24,"etape 0");
	
	while (1){
		while(Driver_USART1.GetStatus().tx_busy == 1); // attente buffer TX vide
		//GLCD_DrawString(0, 2*24,"etape 1");
		Driver_USART1.Receive(tab,14); 
		//GLCD_DrawString(0, 3*24,"etape 2");		
		while (Driver_USART1.GetRxCount() < 14 ) ; 
		//GLCD_DrawString(0, 4*24,"etape 3 ");
		bon_badge = 1;
		for ( i = 0 ; i < 14 ; i ++ ) 
		{
			//GLCD_DrawString(0, 10*24,"avant if");
			if ( tab[i] != identite_badge[i] )
			{
				bon_badge = 0;
				break;
			}
		}	
		
		if (bon_badge == 1)
			{
			GLCD_DrawString(0, 5*24,"            ");
			GLCD_DrawString(0, 5*24,"BON BADGE");
		}
		else
			{
			
			GLCD_DrawString(0, 5*24,"            ");
			GLCD_DrawString(0, 5*24,"VOLEUR");
			}
	}
	
}