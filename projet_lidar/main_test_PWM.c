#include "RTE_Device.h"                 // Keil::Device:Startup
#include "LPC17xx.h"                    // Device header
#include "Driver_USART.h"               // ::CMSIS Driver:USART
#include "Board_GLCD.h"                 // ::Board Support:Graphic LCD
#include "GLCD_Config.h"                // Keil.MCB1700::Board Support:Graphic LCD
#include "cmsis_os.h"                   // ARM::CMSIS:RTOS:Keil RTX
#include <stdio.h> 
#include <math.h> 

#define PI 3.14159265  

osThreadId ID_GetLidarData ;
osThreadId ID_PrintLidarData;
osMailQId ID_BAL_DATALIDAR;

typedef struct {
	short angle_S;
	short distance_S;
	}DATASEND;


void Thread_Data_Lidar(void const *argument);
void Thread_Print_Lidar(void const *argument);
osMailQDef (BAL_DATALIDAR,360,DATASEND);

osThreadDef (Thread_Data_Lidar,osPriorityNormal,1,0);
osThreadDef (Thread_Print_Lidar,osPriorityNormal,1,0);



extern ARM_DRIVER_USART Driver_USART0; // Structure UART0
extern GLCD_FONT GLCD_Font_16x24; // STRucture font LCD 

void Init_UART(void); // prototypes fonctions  
void pwm_init_lidar(int DC); 


int main(void){
	
	char start[2] = {0xA5,0x20}; // tableaux de commandes pour le lidar 
	char stop[2] = {0xA5,0x25}; 
	char info[2] = {0xA5,0x50}; 
	char health[2] = {0xA5,0x52}; 
	
	osKernelInitialize() ;
	
	GLCD_Initialize();  // Initialisation LCD 
	GLCD_SetBackgroundColor(GLCD_COLOR_BLUE);
	GLCD_SetForegroundColor(GLCD_COLOR_YELLOW); 
	GLCD_SetFont(&GLCD_Font_16x24);
	GLCD_ClearScreen(); 
	
	Init_UART(); // INItialisation UART 	
	pwm_init_lidar(80); // Fonction configuration de la pwm (99 vitesse max, 0 vitesse nulle) 
	
	ID_BAL_DATALIDAR = osMailCreate(osMailQ(BAL_DATALIDAR),NULL);
	ID_GetLidarData = osThreadCreate ( osThread(Thread_Data_Lidar), NULL ) ;
	ID_GetLidarData = osThreadCreate ( osThread(Thread_Print_Lidar), NULL ) ;
	
	while(Driver_USART0.GetStatus().tx_busy == 1){}; // attente buffer TX vide		
	Driver_USART0.Send(start,2); // Envoi de la commande de début d'acquisition 
		
	osKernelStart() ; // lancement horloge TR, main retrouve sa priorité
	osDelay(osWaitForever) ;
		
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

void Thread_Data_Lidar(void const *argument){
	
	DATASEND *ptr_send; 

	char data[5]; // tableau récupération des 5 octets de réponse du lidar

	short DISTANCE_SORTED[360]; // tableau de la distance en fonction de l'angle 
	
	short qualite = 0; // variables des différentes réponses LIDAR 
	short	angle = 0;
	short	distance = 0;
	
	while(1){
		
	
	
	Driver_USART0.Receive(data,5); // reception de 5 octets de réponse du LIDAR (1 octet de qualité (2 bits -> S et S- à supp ), 2 octets angle (bit C à supp), 2 octets distance )
	while (Driver_USART0.GetRxCount() <1 ) ; // attente buffer RX vide
		
	qualite = data[0]>>2; // décalage de deux car deux bits b0 et b1 inutiles pour la qualité 
	angle = ((((data[2] << 7) | data[1])) >> 1) / 64.0; // (data[1] >> 1 ) bit C inutile donc décalage de 1, (data[2] << 7) on décale de 7 pour le mettre à la suite, le | sert à "fusionner" les deux datas, division par 60 car doc 
	distance = (((data[4]<<7) | data[3])/4.0); // même principe de "fusion" sauf que pas de bit à supprimmer puis division par 4 doc
		
	if(qualite > 20){
		
		ptr_send = osMailAlloc(ID_BAL_DATALIDAR,osWaitForever); 
		ptr_send->angle_S = angle;
		ptr_send->distance_S = distance; 	
		osMailPut(ID_BAL_DATALIDAR,ptr_send); 	
	}
	
  
	
	
	}
}
void Thread_Print_Lidar(void const *argument){
	
	osEvent EVretour; 
	DATASEND *ptr_rec; 
	short distance_R, angle_R; 
	float angle_rad; 
	int x;
	int y;
	
	
	
	
	
	
	char aff_QUALI[20]; // TAB servant de chaine de caractères pour sprintf 
	char aff_ANGLE[20];
	char aff_DISTANCE[20];
	
	while(1){
		
		EVretour = osMailGet(ID_BAL_DATALIDAR,osWaitForever); 
		
		ptr_rec = EVretour.value.p; 
		distance_R = ptr_rec->distance_S; 
		angle_R = ptr_rec->angle_S; 
		
		osMailFree(ID_BAL_DATALIDAR,ptr_rec); 
		
	  
		angle_rad = angle_R * (PI /180.0);
		
		x = (int)(distance_R*cos(angle_rad));
		y = (int)(distance_R*sin(angle_rad));
		
		GLCD_DrawPixel(x+160, y+120);
		
		/*sprintf(aff_ANGLE, " %3d" ,x); 
		GLCD_DrawString(0,40,aff_ANGLE);
		
		sprintf(aff_DISTANCE, "y  = %4d" ,y); 
		GLCD_DrawString(0,80,aff_DISTANCE);*/
		
		
		
		
	//affichage qualite 
	/*sprintf(aff_QUALI, "Qualite = %3d" ,qualite); 
	GLCD_DrawString(0,0,aff_QUALI); 
		
//affichage angle  	
	sprintf(aff_ANGLE, "Angle_f = %3d" ,angle); 
	GLCD_DrawString(0,40,aff_ANGLE);
		
//affichage distance  
	sprintf(aff_DISTANCE, "DISTANCE = %4d" ,distance); 
	GLCD_DrawString(0,80,aff_DISTANCE); 
		
		*/
		 





	}

}

