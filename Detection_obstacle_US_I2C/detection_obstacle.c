#include "stm32f4xx_hal.h"
#include "Driver_I2C.h"

//utilisation du pilote CMSIS d'ARM
extern ARM_DRIVER_I2C Driver_I2C1;

//fonction configuration liaison I2C
int configuration_liaisonI2C(void){
	int i=0;
	//initialisation et alimentation du driver
	Driver_I2C1.Initialize(NULL);
	Driver_I2C1.PowerControl(ARM_POWER_FULL);
	
	//configuration I2C en vitesse de transmission standard
	Driver_I2C1.Control(ARM_I2C_BUS_SPEED, ARM_I2C_BUS_SPEED_STANDARD);
	//boucle d'attente qui remplace le BUS CLEAR pour l'instant
	for(i=0;i<100000;i++);
	return 0;
}

//fonction test fonctionnement liaison capteur
//elle nous a permis dans une boucle de connaître les adresses des composants esclaves
void test(uint8_t adresse){
	uint8_t tableau_commande[2];
	tableau_commande[0] = 0x00;
	tableau_commande[1] = 0x51;
	
	Driver_I2C1.MasterTransmit(adresse,tableau_commande,2,false);
	while(Driver_I2C1.GetStatus().busy == 1);
}
//fonction de configuration de la portée
void config_portee(uint8_t portee, uint8_t adresse){
	uint8_t tableau_commande[2];
	tableau_commande[0] = 0x02;
	tableau_commande[1] = portee;
	
	Driver_I2C1.MasterTransmit(adresse,tableau_commande,2,false);
}
//fonction mesure de la distance en cm
uint8_t mesure_cm(uint8_t adresse){
	int i=0;
	//définition de l'adresse du capteur lu
	//registres comportant la valeur de la mesure du SRF10
	uint8_t registreLow = 0x02;
	uint8_t registreHigh = 0x03; 
	//initialisation du tableau d'envoi de commande
	uint8_t tableau_commande[2];
	uint8_t lecture = 0, lecture2 = 0;

	tableau_commande[0] = 0x00;
	tableau_commande[1] = 0x51;
	/*Envoi de la commande 81 au registre 0 :
	ordre de mesure et renvoi d'une valeur correspondant à l'unité métrique
	*/
	//envoi de l'ordre de mesure, écriture
	Driver_I2C1.MasterTransmit(adresse,tableau_commande,2,false);
	while(Driver_I2C1.GetStatus().busy == 1);
	
	lecture = 0xFF;
	//boucle d'attente de la fin de la mesure, lecture continue
	do{
		Driver_I2C1.MasterTransmit(adresse,&registreHigh,1,true);
		while(Driver_I2C1.GetStatus().busy == 1);
		Driver_I2C1.MasterReceive(adresse,&lecture,1,false);
		while(Driver_I2C1.GetStatus().busy == 1);
	}while(lecture == 0xFF);
	//rangement de la mesure dans la variable lecture2
	Driver_I2C1.MasterTransmit(adresse,&registreLow,1,true);
	while(Driver_I2C1.GetStatus().busy == 1);
	Driver_I2C1.MasterReceive(adresse,&lecture2,1,false);
	while(Driver_I2C1.GetStatus().busy == 1);
	return lecture2;
}
