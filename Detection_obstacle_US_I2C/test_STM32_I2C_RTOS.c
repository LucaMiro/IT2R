#include "stm32f4xx_hal.h"
#include "cmsis_os.h" 
#include "detection_obstacle.h"
#include "Driver_I2C.h"
#include "Driver_CAN.h"

extern osThreadId ID_US;
extern osThreadId ID_PARKING;

osThreadId ID_GPIO;

void initialisation_GPIO(void);

void tache_US(void const *arg);
void tache_GPIO(void const *arg);

osMailQId ID_BAL;
osMailQDef(BAL, 10, mailBox);

GPIO_InitTypeDef GPIOC_4;
GPIO_InitTypeDef GPIOC_5;

void tache_US(void const *arg){	
	mailBox *envoi;
	uint8_t cotes;
	while(1){
		envoi = osMailAlloc(ID_BAL,osWaitForever);
		mesureUS_RTOSBis(envoi);
		cotes = envoi->gauche & envoi->droite;
		if((cotes < 0x19)||(envoi->centre < 0x19)){ //SI UN DES CAPTEURS DETECTE
			envoi->alerte = 0x01;
		}
		if((cotes < 0x19)&&(envoi->centre < 0x19)){ //SI TOUS LES CAPTEURS DETECTENT
			envoi->alerte = 0x02;
		}
	  osMailPut(ID_BAL, envoi);
	}
}

void tache_PARKING(void const *arg){
	mailBox *envoiPark;
	uint8_t mesureGauche, mesureDroite, mesureCentre;
	while(1){
		//osSignalWait(US_CAN_EVENT, osWaitForever);
		envoiPark = osMailAlloc(ID_BAL,osWaitForever);
		envoiPark->alerte = 0x00;
		mesureUS_RTOSBis(envoiPark);
		osMailPut(ID_BAL, envoiPark);
	}
}


//tache de test traitement GPIO
void tache_GPIO(void const *arg){
	osEvent eventBAL;
	mailBox *reception;
	while(1){
		eventBAL = osMailGet(ID_BAL, osWaitForever);
		reception = eventBAL.value.p; 
		if(reception->alerte == 0x01) {
			HAL_GPIO_WritePin(GPIOC, GPIO_PIN_5, GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOC, GPIO_PIN_4, GPIO_PIN_RESET);
		}
		else if(reception->alerte == 0x02){
			HAL_GPIO_WritePin(GPIOC, GPIO_PIN_4, GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOC, GPIO_PIN_5, GPIO_PIN_RESET);
		}
		else {
			HAL_GPIO_WritePin(GPIOC, GPIO_PIN_5, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(GPIOC, GPIO_PIN_5, GPIO_PIN_RESET);
		}
		osMailFree(ID_BAL, reception);
	}
}

int main(void){
	//définition des priorités
	osThreadDef(tache_US, osPriorityNormal, 1, 0);
	osThreadDef(tache_GPIO, osPriorityNormal, 1, 0);

	osKernelInitialize();
	//cr?ation des objets
	ID_US = osThreadCreate(osThread(tache_US), NULL);
	ID_GPIO = osThreadCreate(osThread(tache_GPIO), NULL);

	ID_BAL = osMailCreate(osMailQ(BAL), NULL);
	
	initialisation_GPIO();
	configI2C_RTOS();
	
	osKernelStart();
	osSignalSet(ID_US, 0x0001);
	osDelay(osWaitForever);
	return(0);
}

void initialisation_GPIO(void) {
    //activation horloge du port GPIO C
    __HAL_RCC_GPIOC_CLK_ENABLE();
	
		GPIOC_4.Pin     = GPIO_PIN_4; 
    GPIOC_4.Mode    = GPIO_MODE_OUTPUT_PP;			
		//broche 4 sélectionnée, mode sortie "PULL PUSH"
    GPIOC_4.Pull    = GPIO_PULLUP; 			//tirage PULL UP
    GPIOC_4.Speed   = GPIO_SPEED_FREQ_LOW; 	//fréquence de fonctionnement basse
		HAL_GPIO_Init(GPIOC, &GPIOC_4); 
	
		GPIOC_5.Pin     = GPIO_PIN_5; 
    GPIOC_5.Mode    = GPIO_MODE_OUTPUT_PP;			
		//broche 5 sélectionnée, mode sortie "PULL PUSH"
    GPIOC_5.Pull    = GPIO_PULLUP; 			//tirage PULL UP
    GPIOC_5.Speed   = GPIO_SPEED_FREQ_LOW; 	//fréquence de fonctionnement basse
		HAL_GPIO_Init(GPIOC, &GPIOC_5); 
}