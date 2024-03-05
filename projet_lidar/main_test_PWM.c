#include "RTE_Device.h"                 // Keil::Device:Startup
#include "LPC17xx.h"                    // Device header
#include "Driver_USART.h"               // ::CMSIS Driver:USART
#include "Board_GLCD.h"                 // ::Board Support:Graphic LCD
#include "GLCD_Config.h"                // Keil.MCB1700::Board Support:Graphic LCD
#include <stdio.h> 

extern ARM_DRIVER_USART Driver_USART0; // Structure UART0
extern GLCD_FONT GLCD_Font_16x24; // STRucture font LCD 

void Init_UART(void); // prototypes fonctions  
void pwm_init_lidar(int DC); 


int main(void){
	
	char start[2] = {0xA5,0x20}; // tableaux de commandes pour le lidar 
	char stop[2] = {0xA5,0x25}; 
	char info[2] = {0xA5,0x50}; 
	char health[2] = {0xA5,0x52}; 
	
	char aff_QUALI[20]; // TAB servant de chaine de caract�res pour sprintf 
	char aff_ANGLE[20];
	char aff_DISTANCE[20];
	
	char data[5]; // tableau r�cup�ration des 5 octets de r�ponse du lidar

	short DISTANCE_SORTED[360]; // tableau de la distance en fonction de l'angle 
	
	short qualite = 0; // variables des diff�rentes r�ponses LIDAR 
	short	angle = 0;
	short	distance = 0;
	
	GLCD_Initialize();  // Initialisation LCD 
	GLCD_SetBackgroundColor(GLCD_COLOR_BLUE);
	GLCD_SetForegroundColor(GLCD_COLOR_YELLOW); 
	GLCD_SetFont(&GLCD_Font_16x24);
	GLCD_ClearScreen(); 
	
	Init_UART(); // INItialisation UART 	
	pwm_init_lidar(80); // Fonction configuration de la pwm (99 vitesse max, 0 vitesse nulle)  
	
		
	while(Driver_USART0.GetStatus().tx_busy == 1){}; // attente buffer TX vide		
	Driver_USART0.Send(start,2); // Envoi de la commande de d�but d'acquisition 
		
//-----------------------------------------BOUCLE INFINIE------------------------------------------------------------------------	
		
	while(1){
		
	Driver_USART0.Receive(data,5); // reception de 5 octets de r�ponse du LIDAR (1 octet de qualit� (2 bits -> S et S- � supp ), 2 octets angle (bit C � supp), 2 octets distance )
	while (Driver_USART0.GetRxCount() <1 ) ; // attente buffer RX vide
		
	qualite = data[0]>>2; // d�calage de deux car deux bits b0 et b1 inutiles pour la qualit� 
	angle = ((((data[2] << 7) | data[1])) >> 1) / 64.0; // (data[1] >> 1 ) bit C inutile donc d�calage de 1, (data[2] << 7) on d�cale de 7 pour le mettre � la suite, le | sert � "fusionner" les deux datas, division par 60 car doc 
	distance = (((data[4]<<7) | data[3])/4.0); // m�me principe de "fusion" sauf que pas de bit � supprimmer puis division par 4 doc  

		
	
	
	//DISTANCE_SORTED[angle] = distance;
		
		
	
	
//affichage qualite 
	//sprintf(aff_QUALI, "Qualite = %3d" ,qualite); 
	//GLCD_DrawString(0,0,aff_QUALI); 
		
//affichage angle  	
	//sprintf(aff_ANGLE, "Angle_f = %3d" ,angle); 
	//GLCD_DrawString(0,40,aff_ANGLE);
		
//affichage distance  
	//sprintf(aff_DISTANCE, "DISTANCE = %4d" ,DISTANCE_SORTED[angle]); 
	//GLCD_DrawString(0,80,aff_DISTANCE); 
	
	
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
	LPC_PWM1->MR0 = 99;    // MR0+1=100   la p�riode de la PWM vaut 1ms
	
	LPC_PINCON->PINSEL4 = LPC_PINCON->PINSEL4| 0x00000400; // P2.5 bit 11 � 0 bit 10 � 1 
	
	LPC_PWM1->MCR = LPC_PWM1->MCR | 0x00000002; // Compteur relanc� quand MR0 repasse � 0
	LPC_PWM1->LER = LPC_PWM1->LER | 0x00000040;  // ceci donne le droit de modifier dynamiquement la valeur du rapport cyclique
	
	LPC_PWM1->PCR = LPC_PWM1->PCR | 0x00004000;  // autorise les sortie PWM1/2/3 bits 9, 10, 11
	
	LPC_PWM1->MR6 = DC-1;
	
	LPC_PWM1->TCR = 1;  /*validation de timer  et reset counter */
}

