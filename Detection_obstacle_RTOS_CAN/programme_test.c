#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_gpio.h"
#include "cmsis_os.h" 
#include "detection_obstacleBis.h"
#include "Driver_I2C.h"

GPIO_InitTypeDef initGPIO_1;

extern osThreadId ID_US;
osThreadId ID_CAN;

static void initialisation_GPIO(void);

void tache_US(void const *arg);

void tache_US(void const *arg){
	uint8_t d,g;
	while(1){
		g = mesureUS(CAPTEUR_ARG);
		//d = mesureUS(CAPTEUR_ARD);
		osDelay(10);
	}
}

int main(void){
	//définition des priorités
	osThreadDef(tache_US, osPriorityNormal, 1, 0);
	configI2C();
	initialisation_GPIO();
	osKernelInitialize();
	//cr?ation des objets
	ID_US = osThreadCreate(osThread(tache_US), NULL);
	osKernelStart();
	osDelay(osWaitForever);
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
}
