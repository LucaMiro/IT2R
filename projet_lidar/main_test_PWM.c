#include "RTE_Device.h"                 // Keil::Device:Startup
#include "LPC17xx.h"                    // Device header
#include "Driver_USART.h"               // ::CMSIS Driver:USART
#include "Board_GLCD.h"                 // ::Board Support:Graphic LCD
#include "GLCD_Config.h"                // Keil.MCB1700::Board Support:Graphic LCD
#include <stdio.h> 

extern ARM_DRIVER_USART Driver_USART0;
extern GLCD_FONT GLCD_Font_16x24; 

void Init_UART(void);
void pwm_init_lidar(int DC); 


int main(void){
	
	char start[2] = {0xA5,0x20}; 
	char stop[2] = {0xA5,0x25}; 
	char info[2] = {0xA5,0x50}; 
	char health[2] = {0xA5,0x52}; 
	
	char aff_QUALI[20]; 
	char aff_ANGLE[20];
	char aff_DISTANCE[20];
	uint8_t data[5];
	uint16_t	angle_f = 0;
	
	GLCD_Initialize(); 
	GLCD_SetBackgroundColor(GLCD_COLOR_BLUE);
	GLCD_SetForegroundColor(GLCD_COLOR_YELLOW); 
	GLCD_SetFont(&GLCD_Font_16x24);
	GLCD_ClearScreen(); 
	
	Init_UART();	
	pwm_init_lidar(80);
	
		
	while(Driver_USART0.GetStatus().tx_busy == 1){}; // attente buffer TX vide		
	Driver_USART0.Send(start,2);
			
	while(1){
	Driver_USART0.Receive(data,5);
	while (Driver_USART0.GetRxCount() <1 ) ;
		
	sprintf(aff_QUALI, "Qualite = %3d" ,data[0] >> 2); 
	GLCD_DrawString(0,0,aff_QUALI); 
		
	angle_f = (data[2] << 7) + (data[1] >> 1);
	sprintf(aff_ANGLE, "Angle_f = %3d" ,angle_f); 
	GLCD_DrawString(0,40,aff_ANGLE);
		

	
	}
	return 0;
}

void Init_UART(void){
	Driver_USART0.Initialize(NULL);
	Driver_USART0.PowerControl(ARM_POWER_FULL);
	Driver_USART0.Control(	ARM_USART_MODE_ASYNCHRONOUS |
							ARM_USART_DATA_BITS_8		|
							ARM_USART_STOP_BITS_1		|
							ARM_USART_PARITY_NONE		|
							ARM_USART_FLOW_CONTROL_NONE,
							115200);
	Driver_USART0.Control(ARM_USART_CONTROL_TX,1);
	Driver_USART0.Control(ARM_USART_CONTROL_RX,1);
}


void pwm_init_lidar(int DC){
	
	
	LPC_SC->PCONP = LPC_SC->PCONP | 0x00000040;   // enable PWM1
	
	LPC_PWM1->PR = 12;  // prescaler
	LPC_PWM1->MR0 = 99;    // MR0+1=100   la période de la PWM vaut 1ms
	
	LPC_PINCON->PINSEL4 = LPC_PINCON->PINSEL4| 0x00000400; // P2.5 bit 11 à 0 bit 10 à 1 
	
	LPC_PWM1->MCR = LPC_PWM1->MCR | 0x00000002; // Compteur relancé quand MR0 repasse à 0
	LPC_PWM1->LER = LPC_PWM1->LER | 0x00000040;  // ceci donne le droit de modifier dynamiquement la valeur du rapport cyclique
	
	LPC_PWM1->PCR = LPC_PWM1->PCR | 0x00004000;  // autorise les sortie PWM1/2/3 bits 9, 10, 11
	
	LPC_PWM1->MR6 = DC-1;
	
	LPC_PWM1->TCR = 1;  /*validation de timer  et reset counter */
}

