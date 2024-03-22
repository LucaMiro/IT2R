#include <stdio.h>// ::CMSIS Driver:I2C
#include "Board_GLCD.h"                 // ::Board Support:Graphic LCD
//#include "GLCD_Config.h"                // Keil.MCB1700::Board Support:Graphic LCD
#include "Driver_USART.h"               // ::CMSIS Driver:USART
#include "Driver_USART.h" 
#include "moteur-fonctions.h" 
#include "GPIO.h"
#include "Timer.h"
#include "LPC17xx.h"  


extern ARM_DRIVER_USART Driver_USART1;
//extern GLCD_FONT GLCD_Font_16x24;

int i=0;


void Init_UART(void){
	Driver_USART1.Initialize(NULL);
	Driver_USART1.PowerControl(ARM_POWER_FULL);
	Driver_USART1.Control( ARM_USART_MODE_ASYNCHRONOUS |
	ARM_USART_DATA_BITS_8 |
	ARM_USART_STOP_BITS_1 |
	ARM_USART_PARITY_NONE |
	ARM_USART_FLOW_CONTROL_NONE,
	115200);

	Driver_USART1.Control(ARM_USART_CONTROL_TX,1);
	Driver_USART1.Control(ARM_USART_CONTROL_RX,1);
}
int j=0;

int main(void){

	uint8_t tab[2];
	char chaine1[30];
	char chaine2[30];
	char chaine3[30];
	char chaine4[30];
	int posX;
	int posY;
	
	Init_UART();
	//GLCD_Initialize();
	Initialise_GPIO();
	Init_moteur_direction();
	for(i=0;i<2000;i++){}
	Init_moteur();
		for(i=0;i<2000;i++){}
	//GLCD_ClearScreen();
  //GLCD_SetFont(&GLCD_Font_16x24);
	//GLCD_DrawString(0, 1*24,"Init fin");
	
	  while(1){
			
			Driver_USART1.Receive(tab,2); // recieve de x et y
			while(Driver_USART1.GetRxCount()<1);
			
				posX=tab[0];
				posY=tab[1];
			
			//sprintf(chaine1, "position X : %03d", posX); // min: 30 max: 230
			//GLCD_DrawString(0, 2*24,chaine1);
			//sprintf(chaine2, "position Y : %03d", posY); // min: 25 max: 225
			//GLCD_DrawString(0, 3*24,chaine2);
			
			//sprintf(chaine4, "rapport m : %.2f", ((0.05*posX)/180)+0.0375 ); // min: 25 max: 225
			//GLCD_DrawString(0, 6*24,chaine4);
			
//			if(posY>200){avancer(600);GLCD_DrawString(0, 5*24,"avancer");}
//			else if(posY<110){reculer(400);GLCD_DrawString(0, 5*24,"reculer");}
//			else if(posY>110 && posY<200){freiner();GLCD_DrawString(0, 5*24,"arret");}
//			
//			if(posX>200){tourner(0.04);}
//			else if(posX<110){tourner(0.11);}
//			else if(posX>110 && posX<200){tourner(0.075);}

				if(posY>150){avancer(1666);}
			else if(posY<110){reculer(1666);}
			else if(posY>110 && posY<200){freiner();}			
			tourner(  ((0.05*(255-posX))/180)+0.0375 );
	
			
		}
	

	



}