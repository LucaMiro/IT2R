#include "stm32f4xx_hal.h"
#include "Driver_I2C.h"
//définition adresse/numéro capteurs

extern ARM_DRIVER_I2C Driver_I2C1;

//fonction configuration liaison I2C
int configuration_liaisonI2C(void){
	int i=0;
	Driver_I2C1.Initialize(NULL);
	Driver_I2C1.PowerControl(ARM_POWER_FULL);
	
	Driver_I2C1.Control(ARM_I2C_BUS_SPEED, ARM_I2C_BUS_SPEED_STANDARD);
	//Driver_I2C1.Control(ARM_I2C_BUS_CLEAR,0);//PROBLEME
	for(i=0;i<10000;i++);
	return 0;
}

void configuration_capteurs(void){
/*ajouter configuration adresses*/	
}

//fonction test fonctionnement liaison capteur
//Ajout adresse réglable
void test(uint8_t adresse){
	//Adresses possibles :
	uint8_t commande = 0x51;
	uint8_t tableau_commande[2];
	tableau_commande[0] = 0x00;
	tableau_commande[1] = commande;
	
	Driver_I2C1.MasterTransmit(adresse,tableau_commande,2,false);
	while(Driver_I2C1.GetStatus().busy == 1);
}

//fonction mesure de la distance en cm
uint8_t mesure_cm(void){
	/*
	fonctionnement :
	On donne l'adresse de l'esclave, on lui envoie l'octet 0x51 pour lui donner
	l'ordre d'effectuer une mesure de distance et la renvoyer en cm
	*/
	/*capteur = 0xE6;
	uint8_t adresse = capteur;*/
	uint8_t adresse =  0x73;
	uint8_t commande = 0x51;
	uint8_t registreLow,registreHigh;
	uint8_t tableau_commande[2];
	uint8_t lecture = 0, lecture2 = 0;
	uint8_t donnee;
	tableau_commande[0] = 0x00;
	tableau_commande[1] = commande;
	registreHigh = 0x02;
	registreLow = 0x03;
	
	Driver_I2C1.MasterTransmit(adresse,tableau_commande,2,false);
	while(Driver_I2C1.GetStatus().busy == 1);
	//boucle d'attente de la fin de la mesure
	Driver_I2C1.MasterTransmit(adresse,&registreHigh,1,true);
	do{
		while(Driver_I2C1.GetStatus().busy == 1);
		Driver_I2C1.MasterReceive(adresse,&lecture,1,false);
	}while(lecture == 0xFF);
	//rangement de la "bonne" valeur dans la variable lecture2
	Driver_I2C1.MasterTransmit(adresse,&registreLow,1,true);
	while(Driver_I2C1.GetStatus().busy == 1);
	Driver_I2C1.MasterReceive(adresse,&lecture2,1,false);
	return lecture2;
}
