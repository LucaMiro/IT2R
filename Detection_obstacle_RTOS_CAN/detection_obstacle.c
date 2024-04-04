/*
BIBLIOTHEQUE DES FONCTIONS POUR L'UTILISATION DES CAPTEURS ULTRASON SRF10 (I2C)
POUR L'UTILISATION SUR LA CARTE STM32F4 AVANT : mettre le 2e osSignalSet de la callback en commentaire
*/

#include "stm32f4xx_hal.h"
#include "Driver_I2C.h"
#include "Driver_CAN.h"
#include "detection_obstacle.h"
#include "cmsis_os.h"

extern ARM_DRIVER_I2C Driver_I2C1;
extern ARM_DRIVER_CAN Driver_CAN2;

osThreadId ID_US;

void ARM_I2C_CallBack(uint32_t event){
	if(event & ARM_I2C_EVENT_TRANSFER_DONE){
		osSignalSet(ID_US, US_EV);
	}
}

void configI2C(void){
	Driver_I2C1.Initialize(ARM_I2C_CallBack);
	Driver_I2C1.PowerControl(ARM_POWER_FULL);
	//configuration I2C en vitesse de transmission standard
	Driver_I2C1.Control(ARM_I2C_BUS_SPEED, ARM_I2C_BUS_SPEED_STANDARD);
	osDelay(10);
}

void mesureAvant(void){
	ARM_CAN_MSG_INFO envoiCAN;
	uint8_t cotes, alerte, envoi;
	uint8_t  CAN_data[5];
	uint8_t droite, gauche, centre;

	envoiCAN.rtr = 0; 
	envoiCAN.id = ARM_CAN_STANDARD_ID(ID_TRAME_CAN);
	
	gauche = mesureUS(CAPTEUR_AVG);
	centre = mesureUS(CAPTEUR_AVC);
	droite = mesureUS(CAPTEUR_AVD);

	cotes = gauche & droite;
	if((cotes < 0x08)||(centre < 0x08)){
		alerte = 0x02;
		CAN_data[0] = 0x23;
		CAN_data[1] = alerte;
		CAN_data[2] = gauche;
		CAN_data[3] = centre;
		CAN_data[4] = droite;
		Driver_CAN2.MessageSend(2, &envoiCAN, CAN_data, 5);
		osSignalWait(0x01, osWaitForever);
	}
	else if((cotes < 0x19)||(centre < 0x19)){
		alerte = 0x01;
		CAN_data[0] = 0x23;
		CAN_data[1] = alerte;
		CAN_data[2] = gauche;
		CAN_data[3] = centre;
		CAN_data[4] = droite;
		Driver_CAN2.MessageSend(2, &envoiCAN, CAN_data, 5);
		osSignalWait(0x01, osWaitForever);
	}
}

void mesureArriere(void){
	ARM_CAN_MSG_INFO envoiCAN;
	uint8_t droite, gauche, alerte, envoi;
	uint8_t CAN_data[4];

	envoiCAN.rtr = 0; 
	envoiCAN.id = ARM_CAN_STANDARD_ID(ID_TRAME_CAN);

	gauche = mesureUS(CAPTEUR_ARG);
	droite = mesureUS(CAPTEUR_ARD);
	envoi = 0;
	if((gauche < 0x10)||(droite < 0x10)){
		alerte = 0x02;
		CAN_data[0] = 0x01;
		CAN_data[1] = alerte;
		CAN_data[2] = gauche;
		CAN_data[3] = droite;
		Driver_CAN2.MessageSend(2, &envoiCAN, CAN_data, 4);
	}
	else if((gauche < 0x20)||(droite < 0x20)){
		alerte = 0x01;
		CAN_data[0] = 0x01;
		CAN_data[1] = alerte;
		CAN_data[2] = gauche;
		CAN_data[3] = droite;
		Driver_CAN2.MessageSend(2, &envoiCAN, CAN_data, 4);
	}
}

uint8_t mesureLaterale(void){
	uint8_t mesure;
	mesure = mesureUS(0x00/*CAPTEUR_PARK*/);
	return mesure;
}

uint8_t mesureUS(uint8_t capteur){
	uint8_t RegLecture = 0x03;
	uint8_t tab[2];
	uint8_t temp, mesure;

	tab[0] = 0x00; //registre
	tab[1] = 0x51; //commande mesure cm

	Driver_I2C1.MasterTransmit(capteur, tab, 2,false);
	osSignalWait(US_EV, osWaitForever);
	temp = 0xFF;
	do{
		Driver_I2C1.MasterTransmit(capteur, &RegLecture, 1, true);
		osSignalWait(US_EV, osWaitForever);
		Driver_I2C1.MasterReceive(capteur, &temp, 1, false);
	}while(temp == 0xFF);
	Driver_I2C1.MasterTransmit(capteur, &RegLecture, 1, true);
	osSignalWait(US_EV, osWaitForever);
	Driver_I2C1.MasterReceive(capteur, &mesure, 1, false);
	osSignalWait(US_EV, osWaitForever);
	return mesure;
}

void config_portee(uint8_t portee, uint8_t adresse){
	uint8_t tableau_commande[2];
	tableau_commande[0] = 0x02;
	tableau_commande[1] = portee;
	
	Driver_I2C1.MasterTransmit(adresse,tableau_commande,2,false);
	osSignalWait(US_EV, osWaitForever);
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
	osSignalWait(US_EV, osWaitForever);
	
	tableau_commande[1] = 0xAA;
	Driver_I2C1.MasterTransmit(adresseEsclave,tableau_commande,2,false);
	osSignalWait(US_EV, osWaitForever);
	
	tableau_commande[1] = 0xA5;
	Driver_I2C1.MasterTransmit(adresseEsclave,tableau_commande,2,false);
	osSignalWait(US_EV, osWaitForever);
	
	tableau_commande[1] = adresseCible;
	Driver_I2C1.MasterTransmit(adresseEsclave,tableau_commande,2,false);
	osSignalWait(US_EV, osWaitForever);
}

void test_capteurs(void){
	uint8_t tab[2];
	uint8_t adresse = 0x70;
	tab[0] = 0x00;
	
	for(adresse = 0x70; adresse < 0x76; adresse += 0x01){
		Driver_I2C1.MasterTransmit(adresse,tab,1,false);
		osSignalWait(US_EV, osWaitForever);
	}
}

