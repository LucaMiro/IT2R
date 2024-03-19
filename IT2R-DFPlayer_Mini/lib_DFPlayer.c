#include "stdio.h"
#include "LPC17xx.h"
#include "lib_DFPlayer.h"
#include "UART_LPC17xx.h"

extern ARM_DRIVER_USART Driver_USART2; // "déclaration" structure UART1

typedef struct{

		uint8_t start;							//toujours 0x7E
		uint8_t version;						//version 0xFF
		uint8_t len;								//nombre d'octet de la commande aprés len
		uint8_t commande;						//octet de commande
		uint8_t commande_feedback;	//besoin de retour = 1 pas besoin de retour = 0
		uint8_t parametre1;					//ocet de poid fort
		uint8_t parametre2;					//ocet de poid faible
		uint8_t checksum1;					//checksum sur 16 bit avec octet poid fort en premier
		uint8_t checksum2;					//
		uint8_t end;								//toujours 0xEF
	
}commande;



void init_UART2(void){
	Driver_USART2.Initialize(NULL); // début initialisation
	Driver_USART2.PowerControl(ARM_POWER_FULL); // alimentation périphérique
	Driver_USART2.Control( ARM_USART_MODE_ASYNCHRONOUS |
												 ARM_USART_DATA_BITS_8 |
												 ARM_USART_STOP_BITS_1 |
												 ARM_USART_PARITY_NONE |
												 ARM_USART_FLOW_CONTROL_NONE ,
												 9600);
	Driver_USART2.Control(ARM_USART_CONTROL_TX,1); // validation émission
	Driver_USART2.Control(ARM_USART_CONTROL_RX,1); // validation réception
	return;
}


	
void choisir_music(uint16_t channel)
	{
		short checksum=0;
		commande music;
		music.start=0x7E;
		music.version=0xFF;
		music.len=0x06;
		music.commande=0x0F;
		music.commande_feedback=0x00;
		music.parametre1 = channel>>8;
		music.parametre2 = channel;
		checksum = - music.version - music.len - music.commande - music.commande_feedback - music.parametre1 - music.parametre2;
		music.checksum1 = checksum>>8;
		music.checksum2 = checksum;
		music.end = 0xEF;
		
		while(Driver_USART2.GetStatus().tx_busy == 1); // attente buffer TX vide
		Driver_USART2.Send(&music,10);
		return;
	}
	


	
	
