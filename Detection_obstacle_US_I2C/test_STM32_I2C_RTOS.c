#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_gpio.h"
#include "cmsis_os.h" 
#include "detection_obstacle.h"
#include "Driver_I2C.h"

extern ARM_DRIVER_I2C Driver_I2C1;
extern osThreadId ID_US;
osThreadId ID_GPIO;

uint8_t mesure(uint8_t capteur);
void initialisation_GPIO(void);

typedef struct {
	uint8_t mesureA, mesureB, mesureC;
}structMail;

void tache_US(void const *arg);
void tache_GPIO(void const *arg);

osMailQId ID_BAL;
osMailQDef(BAL, 10, structMail);

GPIO_InitTypeDef GPIOC_3;
GPIO_InitTypeDef GPIOC_4;
GPIO_InitTypeDef GPIOC_5;

//tache de mesure sur 3 capteurs US
void tache_US(void const *arg){
	structMail *envoi;
	uint8_t mesureA, mesureB, mesureC;
	while(1){
		envoi = osMailAlloc(ID_BAL,osWaitForever);
		mesureA = mesureUS_RTOS(CAPTEUR_E4);
		mesureB = mesureUS_RTOS(CAPTEUR_E6);
		mesureC = mesureUS_RTOS(CAPTEUR_E8);
		envoi->mesureA = mesureA;
		envoi->mesureB = mesureB;
		envoi->mesureC = mesureC;
		osDelay(10);
	  osMailPut(ID_BAL, envoi);
	}
}

//tache de test traitement GPIO
void tache_GPIO(void const *arg){
	osEvent eventBAL;
	structMail *reception;
	while(1){
		eventBAL = osMailGet(ID_BAL, osWaitForever);
		reception = eventBAL.value.p;
		osDelay(10);
		if(reception->mesureA < 0x10) HAL_GPIO_WritePin(GPIOC, GPIO_PIN_5, GPIO_PIN_SET);
		else HAL_GPIO_WritePin(GPIOC, GPIO_PIN_5, GPIO_PIN_RESET);
		
		if(reception->mesureB < 0x10) HAL_GPIO_WritePin(GPIOC, GPIO_PIN_4, GPIO_PIN_SET);
		else HAL_GPIO_WritePin(GPIOC, GPIO_PIN_4, GPIO_PIN_RESET);
		
		if(reception->mesureC < 0x10) HAL_GPIO_WritePin(GPIOC, GPIO_PIN_3, GPIO_PIN_SET);
		else HAL_GPIO_WritePin(GPIOC, GPIO_PIN_3, GPIO_PIN_RESET);
		osMailFree(ID_BAL, reception);
	}
}

int main(void){
	osThreadDef(tache_US, osPriorityNormal, 1, 0);
	osThreadDef(tache_GPIO, osPriorityNormal, 1, 0);
	
	osKernelInitialize();
	//création des objets
	ID_US = osThreadCreate(osThread(tache_US), NULL);
	ID_GPIO = osThreadCreate(osThread(tache_GPIO), NULL);
	
	ID_BAL = osMailCreate(osMailQ(BAL), NULL);
	
	initialisation_GPIO();
	configI2C_RTOS();
	
	osKernelStart();
	osDelay(osWaitForever);
	return(0);
}

void initialisation_GPIO(void) {
    //activation horloge du port GPIO C
    __HAL_RCC_GPIOC_CLK_ENABLE();
	
		GPIOC_3.Pin     = GPIO_PIN_3; 
    GPIOC_3.Mode    = GPIO_MODE_OUTPUT_PP;			
		//broche 4 sélectionnée, mode sortie "PULL PUSH"
    GPIOC_3.Pull    = GPIO_PULLUP; 			//tirage PULL UP
    GPIOC_3.Speed   = GPIO_SPEED_FREQ_LOW; 	//fréquence de fonctionnement basse
		HAL_GPIO_Init(GPIOC, &GPIOC_3);
	
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

uint8_t mesure(uint8_t capteur){
	osEvent event;
	uint8_t LowValReg=0x03;
	uint8_t temp, mesure_cm;
	uint8_t tab[2];
	tab[0] = 0x00; //registre
	tab[1] = 0x51; //commande mesure cm
	
	Driver_I2C1.MasterTransmit(capteur, tab,2,false);
	event = osSignalWait(0x0002, osWaitForever);
	temp = 0xFF;
	do{
		Driver_I2C1.MasterTransmit(capteur,&LowValReg,1, true);
		event = osSignalWait(0x0002, osWaitForever);
		Driver_I2C1.MasterReceive(capteur, &temp, 1, false);
	}while(temp == 0xFF);
	//récupération valeur réelle mesurée
	Driver_I2C1.MasterTransmit(capteur,&LowValReg,1, true);
	event = osSignalWait(0x0002, osWaitForever);
	Driver_I2C1.MasterReceive(capteur, &mesure_cm, 1, false);
	event = osSignalWait(0x0002, osWaitForever);
	return mesure_cm;
}