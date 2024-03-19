/*
BIBLIOTHEQUE DES FONCTIONS POUR L'UTILISATION DES CAPTEURS ULTRASON SRF10 (I2C)
NE PAS MODIFIER CE FICHIER
*/
#include "detection_obstacle.h"
#include "stm32f4xx_hal.h"
#include "Driver_I2C.h"
#include "cmsis_os.h"


extern ARM_DRIVER_I2C Driver_I2C1;
osThreadId ID_US;

//fonction callback event transfert I2C 
void ARM_I2C_SignalEvent(uint32_t event){
	if(event & ARM_I2C_EVENT_TRANSFER_DONE){
		osSignalSet(ID_US, US_EVENT);
	}
}

void configI2C_RTOS(void){
	Driver_I2C1.Initialize(ARM_I2C_SignalEvent);
	Driver_I2C1.PowerControl(ARM_POWER_FULL);
	//configuration I2C en vitesse de transmission standard
	Driver_I2C1.Control(ARM_I2C_BUS_SPEED, ARM_I2C_BUS_SPEED_STANDARD);
	osDelay(10);
}

//fonction mesure de la distance en cm
uint8_t mesureUS_RTOS(uint8_t capteur){
	osEvent event;
	uint8_t LowValReg=0x03;
	uint8_t temp, mesure_ptr;
	uint8_t tab[2];
	tab[0] = 0x00; //registre
	tab[1] = 0x51; //commande mesure cm
	
	Driver_I2C1.MasterTransmit(capteur, tab ,2,false);
	event = osSignalWait(US_EVENT, osWaitForever);
	
	temp = 0xFF;
	do{
		Driver_I2C1.MasterTransmit(capteur,&LowValReg,1, true);
		event = osSignalWait(US_EVENT, osWaitForever);
		Driver_I2C1.MasterReceive(capteur, &temp, 1, false);
	}while(temp == 0xFF);
		
	Driver_I2C1.MasterTransmit(capteur,&LowValReg,1, true);
	event = osSignalWait(US_EVENT, osWaitForever);
	Driver_I2C1.MasterReceive(capteur, &mesure_ptr, 1, false);
	event = osSignalWait(US_EVENT, osWaitForever);
	return mesure_ptr;
}

//fonction de configuration de la portée
void config_portee(uint8_t portee, uint8_t adresse){
	osEvent event;
	uint8_t tableau_commande[2];
	tableau_commande[0] = 0x02;
	tableau_commande[1] = portee;
	
	Driver_I2C1.MasterTransmit(adresse,tableau_commande,2,false);
	event = osSignalWait(US_EVENT, osWaitForever);
}

void adressage(uint8_t adresseCible, uint8_t adresseComposant){
	osEvent event;
	uint8_t tableau_commande[2];
	uint8_t adresseEsclave = 0x70;
	if(adresseComposant != 0x00){
		uint8_t adresseEsclave = adresseComposant;
	}

	tableau_commande[0] = 0x00;
	tableau_commande[1] = 0xA0;
	
	Driver_I2C1.MasterTransmit(adresseEsclave,tableau_commande,2,false);
	event = osSignalWait(US_EVENT, osWaitForever);
	
	tableau_commande[1] = 0xAA;
	Driver_I2C1.MasterTransmit(adresseEsclave,tableau_commande,2,false);
	event = osSignalWait(US_EVENT, osWaitForever);
	
	tableau_commande[1] = 0xA5;
	Driver_I2C1.MasterTransmit(adresseEsclave,tableau_commande,2,false);
	event = osSignalWait(US_EVENT, osWaitForever);
	
	tableau_commande[1] = adresseCible;
	Driver_I2C1.MasterTransmit(adresseEsclave,tableau_commande,2,false);
	event = osSignalWait(US_EVENT, osWaitForever);
}
