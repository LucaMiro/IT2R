#include "stm32f4xx_hal.h"
#include "detection_obstacle.h"

//fonctions d'initialisation des périphériques
static void initialisation_GPIO(void);

//structures d'initialisation GPIO : une par broche
GPIO_InitTypeDef initGPIO_1;
GPIO_InitTypeDef initGPIO_2;
GPIO_InitTypeDef initGPIO_3;

//programme de test allumage LEDs en fonction du capteur "touché"
int main(void){
	int i=0;
	uint8_t mesure, mesureBis;

	configuration_liaisonI2C();
	initialisation_GPIO();
	
	while(1){
		//defines noms capteurs présent dans le header
		mesure = mesure_cm(CAPTEUR_E0);
		mesureBis = mesure_cm(CAPTEUR_E4);
		if(mesure < 0x10){ 
			HAL_GPIO_WritePin(GPIOC,GPIO_PIN_3, GPIO_PIN_SET);
		} else {
			HAL_GPIO_WritePin(GPIOC,GPIO_PIN_3, GPIO_PIN_RESET);
		}	
		if(mesureBis < 0x10){
			HAL_GPIO_WritePin(GPIOC,GPIO_PIN_4, GPIO_PIN_SET);
		} else {
			HAL_GPIO_WritePin(GPIOC,GPIO_PIN_4, GPIO_PIN_RESET);
		}	
		for(i=0;i<10000;i++);
		//i=0;
	}
	return(0);
}

static void initialisation_GPIO(void) {
    //activation horloge du port GPIO C
    __HAL_RCC_GPIOC_CLK_ENABLE();
    
    initGPIO_1.Pin     = GPIO_PIN_3; 
    initGPIO_1.Mode    = GPIO_MODE_OUTPUT_PP;			
	//broche 3 sélectionnée, mode sortie "PULL PUSH"
    initGPIO_1.Pull    = GPIO_PULLUP; 			//tirage PULL UP
    initGPIO_1.Speed   = GPIO_SPEED_FREQ_LOW; 	//fréquence de fonctionnement basse
		HAL_GPIO_Init(GPIOC, &initGPIO_1);
	
		initGPIO_2.Pin     = GPIO_PIN_4; 
    initGPIO_2.Mode    = GPIO_MODE_OUTPUT_PP;			
	//broche 4 sélectionnée, mode sortie "PULL PUSH"
    initGPIO_2.Pull    = GPIO_PULLUP; 			//tirage PULL UP
    initGPIO_2.Speed   = GPIO_SPEED_FREQ_LOW; 	//fréquence de fonctionnement basse
		HAL_GPIO_Init(GPIOC, &initGPIO_2);   
	
		initGPIO_3.Pin     = GPIO_PIN_5; 
    initGPIO_3.Mode    = GPIO_MODE_OUTPUT_PP;			
	//broche 5 sélectionnée, mode sortie "PULL PUSH"
    initGPIO_3.Pull    = GPIO_PULLUP; 			//tirage PULL UP
    initGPIO_3.Speed   = GPIO_SPEED_FREQ_LOW; 	//fréquence de fonctionnement basse
    //initialisation du port GPIO
    HAL_GPIO_Init(GPIOC, &initGPIO_3);    
}
