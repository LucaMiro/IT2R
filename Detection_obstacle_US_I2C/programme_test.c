#include "stm32f4xx_hal.h"
#include "detection_obstacle.h"

extern ARM_DRIVER_I2C Driver_I2C1;

static void initialisation_GPIO(void);

GPIO_InitTypeDef initGPIO_1;
GPIO_InitTypeDef initGPIO_2;
GPIO_InitTypeDef initGPIO_3;

//programme de test pour déterminer les adresses des capteurs
int main(void){
	int i=0,j=0;
	uint8_t adresse, portee, mesure;

	configuration_liaisonI2C();
	adresse = 0x70;
	initialisation_GPIO();
	
	while(1){
		mesure = mesure_cm(adresse);
		if(mesure < 0x25){
			HAL_GPIO_WritePin(GPIOC,GPIO_PIN_3, GPIO_PIN_SET);
			//mesure = 0xFF;
		} else {HAL_GPIO_WritePin(GPIOC,GPIO_PIN_3, GPIO_PIN_RESET);}
			
		//else if(mesure < 0x20)HAL_GPIO_WritePin(GPIOC,GPIO_PIN_4, GPIO_PIN_SET);
		//else if(mesure < 0x05)HAL_GPIO_WritePin(GPIOC,GPIO_PIN_5, GPIO_PIN_SET);
	
		for(i=0;i<1000000;i++);
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