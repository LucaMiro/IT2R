#include "stm32f4xx_hal.h"
#include "Driver_I2C.h"

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
	//return 0;
}

void adressage(uint8_t adresseCible, uint8_t adresseComposant){
	uint8_t tableau_commande[2];
	uint8_t adresseEsclave = 0x70;
	if(adresseComposant != 0x00){
		uint8_t adresseEsclave = adresseComposant;
	}
	
	tableau_commande[0] = 0x00;
	tableau_commande[1] = 0xA0;
	
	Driver_I2C1.MasterTransmit(adresseEsclave,tableau_commande,2,false);
	while(Driver_I2C1.GetStatus().busy == 1);
	
	tableau_commande[1] = 0xAA;
	Driver_I2C1.MasterTransmit(adresseEsclave,tableau_commande,2,false);
	while(Driver_I2C1.GetStatus().busy == 1);
	
	tableau_commande[1] = 0xA5;
	Driver_I2C1.MasterTransmit(adresseEsclave,tableau_commande,2,false);
	while(Driver_I2C1.GetStatus().busy == 1);
	
	tableau_commande[1] = adresseCible;
	Driver_I2C1.MasterTransmit(adresseEsclave,tableau_commande,2,false);
	while(Driver_I2C1.GetStatus().busy == 1);
}

//fonction test fonctionnement des capteurs avec leur adresse
//à mettre dans une boucle infinie c'est mieux
void test_adresse(uint8_t adresse){
	static int i=0;
	uint8_t tableau_commande[2];
	tableau_commande[0] = 0x00;
	tableau_commande[1] = 0x51;
	for(adresse = 0x70; adresse < 0x77; adresse += 0x01){
		Driver_I2C1.MasterTransmit(adresse,tableau_commande,2,false);
		while(Driver_I2C1.GetStatus().busy == 1);
	}
	for(i=0;i<1000000;i++);
}

//fonction de configuration de la portée
void config_portee(uint8_t portee, uint8_t adresse){
	uint8_t tableau_commande[2];
	tableau_commande[0] = 0x02;
	tableau_commande[1] = portee;
	
	Driver_I2C1.MasterTransmit(adresse,tableau_commande,2,false);
}

//fonction mesure de la distance en cm
uint16_t mesure_cm(uint8_t adresse){
	//registre comportant la valeur "utile" de la mesure du SRF10
	uint8_t registreLow = 0x03;
	
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
		Driver_I2C1.MasterTransmit(adresse,&registreLow,1,true);
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
