#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_gpio.h"
#include "stm32f4xx_hal_tim.h"

//déclaration des structures pour gérer les périphériques
TIM_HandleTypeDef timer5_note;
TIM_HandleTypeDef timer2_rythme;

GPIO_InitTypeDef initGPIO;

static void initialisation_timer5_note(void);
static void initialisation_timer2_rythme(void);
static void initialisation_GPIO(void);

//Routines d'interruptions
void TIM5_IRQHandler(void){
	HAL_TIM_IRQHandler(&timer5_note); //RAZ de l'interruption
	HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_3); //changement d'état du buzzer
}


void TIM2_IRQHandler(void){
	//le but de cette routine est de tester si on peut modifier en direct la fréquence de bip du buzzer = succès.
	static uint8_t i = 0;
	static unsigned int nombre = 0;
	HAL_TIM_IRQHandler(&timer2_rythme); //RAZ de l'interruption
	i ^= 1;
	nombre++;
	//dans cette machine à états on diminue le match du timer rythme afin d'accélérer le bip du buzzer
	switch(nombre){
		case 6 :
		timer2_rythme.Init.Period = 8000000;
		break;

		case 12 :
		timer2_rythme.Init.Period = 2000000;
		break;

		case 18 :
		timer2_rythme.Init.Period = 100000;
		break;

		case 24 :
		timer2_rythme.Init.Period = 50000;
		break;

		case 30:
		//arrêt des interruptions pour terminer le test
		HAL_TIM_Base_Stop_IT(&timer5_note); 
		break;
	}
	//start/stop du buzzer
	if(i == 0){
		HAL_TIM_Base_Start(&timer5_note);
	} else {
		HAL_TIM_Base_Stop(&timer5_note);
	}
}

int main(void) {
	HAL_Init(); 	
	/* /!\ HAL INIT -> HORLOGE 16 MHz /!\ */					
	initialisation_GPIO();
	initialisation_timer5_note(); 
	initialisation_timer2_rythme();
	while(1){
		//boucle infinie vide pour laisser agir les ISR
	} 
}

//fonctions d'initialisation des périphériques

static void initialisation_timer5_note(void){
	__HAL_RCC_TIM5_CLK_ENABLE(); 	//activation horloge du timer 5
	
	timer5_note.Instance = TIM5;
	timer5_note.Init.CounterMode = TIM_COUNTERMODE_UP;	
	timer5_note.Init.Prescaler = 0; 
	timer5_note.Init.Period = 18182;		//pas de prescaler, match 18182 => f ~ 440.2 Hz
	
	HAL_TIM_Base_Init(&timer5_note);		//initialisation du timer avec la structure établie

	//mise en place de l'interruption
	HAL_TIM_Base_Start_IT(&timer5_note);	
	HAL_NVIC_SetPriority(TIM5_IRQn, 2, 0);
	HAL_NVIC_EnableIRQ(TIM5_IRQn);

	//démarrage du timer
	HAL_TIM_Base_Start(&timer5_note);
}

static void initialisation_timer2_rythme(void){
	__HAL_RCC_TIM2_CLK_ENABLE(); 	//activation horloge du timer 5
	
	timer2_rythme.Instance = TIM2;
	timer2_rythme.Init.CounterMode = TIM_COUNTERMODE_UP;	
	timer2_rythme.Init.Prescaler = 0;
	timer2_rythme.Init.Period = 16000000; //nombre à atteindre pour avoir un switch par seconde, no prescaler
	
	HAL_TIM_Base_Init(&timer2_rythme); //initialisation du timer avec la structure établie

	//mise en place de l'interruption
	HAL_TIM_Base_Start_IT(&timer2_rythme);
	HAL_NVIC_SetPriority(TIM2_IRQn, 1, 0);
	HAL_NVIC_EnableIRQ(TIM2_IRQn);

	//démarrage du timer
	HAL_TIM_Base_Start(&timer2_rythme);
}

static void initialisation_GPIO(void) {
    //activation horloge du port GPIO C
    __HAL_RCC_GPIOC_CLK_ENABLE();
    
    initGPIO.Pin     = GPIO_PIN_3; 
    initGPIO.Mode    = GPIO_MODE_OUTPUT_PP;			
	//broche 3 sélectionnée, mode sortie "PULL PUSH"
    initGPIO.Pull    = GPIO_PULLUP; 			//tirage PULL UP
    initGPIO.Speed   = GPIO_SPEED_FREQ_LOW; 	//fréquence de fonctionnement basse
	
    //initialisation du port GPIO
    HAL_GPIO_Init(GPIOC, &initGPIO);    
}