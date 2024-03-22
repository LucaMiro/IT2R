#include <stdio.h>// ::CMSIS Driver:I2C
#include "Board_GLCD.h"                 // ::Board Support:Graphic LCD
#include "GLCD_Config.h"                // Keil.MCB1700::Board Support:Graphic LCD
#include "Driver_USART.h"               // ::CMSIS Driver:USART
#include "Driver_USART.h" 
#include "GPIO.h"
#include "Timer.h"
#include "moteur-fonctions.h"


/////////****///////
//Bibliothèques pour les fonctions des moteurs
/////////****///////


//Moteur direction
/////////****///////

int a=0;
int b=0;
float rapport_cyclique=0;

void TIMER0_IRQHandler(void){
	
	LPC_TIM0->IR |= 1<<0;
	if(a==0){LPC_GPIO3->FIOPIN3 = LPC_GPIO3->FIOPIN3 & 0xFB;a=1;} //P3.26 a 0
	else if(a==1){LPC_GPIO3->FIOPIN3 = LPC_GPIO3->FIOPIN3 | 0x04;a=0;} //P3.26 a 1
	
	if(b==0){LPC_TIM0->MR0 =(1-rapport_cyclique)*499999; b=1;} //P3.26 a 1
	else if(b==1){LPC_TIM0->MR0 = rapport_cyclique*499999; b=0;} //P3.26 a 0
}

//0.05 minmum DROITE
//0.075 milieu 
//0.1 maximum GAUCHE

void Init_moteur_direction(void){
	
	rapport_cyclique = 0.05; 
	
	LPC_GPIO3->FIODIR3 = LPC_GPIO3->FIODIR3 | 0x04;//  P3.26 en sortie
	LPC_GPIO3->FIOPIN3 = LPC_GPIO3->FIOPIN3 | 0x04;//P3.26 a 1

	//Initialisation du Timer
	initTimer0(0,rapport_cyclique*499999); 
	LPC_TIM0->MCR |= 0x03;
	NVIC_SetPriority(TIMER0_IRQn,0);
	NVIC_EnableIRQ(TIMER0_IRQn);
	
}

void tourner(float valeur){
	 rapport_cyclique = valeur;
}




/////////****///////


//Moteur propulsion
/////////****///////
void Init_moteur(){
	LPC_GPIO0->FIODIR2 = LPC_GPIO0->FIODIR2 | 0x01; //P0.16 en sortie INA
	LPC_GPIO0->FIODIR2 = LPC_GPIO0->FIODIR2 | 0x02; //P0.17 en sortie INB
	LPC_GPIO0->FIODIR2 = LPC_GPIO0->FIODIR2 | 0x08; //P0.19 en sortie ENA
	LPC_GPIO0->FIODIR2 = LPC_GPIO0->FIODIR2 | 0x04; //P0.18 en sortie ENB
	

	
	// initialisation de timer 1
	LPC_SC->PCONP = LPC_SC->PCONP | 0x00000040;   // enable PWM1


	// TC s'incremente toutes les 15 kHz
	LPC_PWM1->PR = 0;  // prescaler

  LPC_PWM1->MR0 = 1666;    // Ceci ajuste la periode de la PWM
	LPC_PWM1->MR2 = 1;    // ceci ajuste la duree de l'etat haut

	LPC_PINCON->PINSEL7 = LPC_PINCON->PINSEL7 | (3<<18); //  P3.25 est la sortie PWM Channel 3 de Timer 1

	LPC_PWM1->MCR = LPC_PWM1->MCR | 0x00000002; // Timer relance quand MR0 repasse a 0
	LPC_PWM1->LER = LPC_PWM1->LER | 0x0000000D;  // ceci donne le droit de modifier dynamiquement la valeur du rapport cyclique
	                                             // bit 0 = MR0    bit3 = MR3
	LPC_PWM1->PCR = LPC_PWM1->PCR | 0x00000e00;  // autorise la sortie PWM

  LPC_PWM1->TCR = 1; //validation de timer 1 et reset counter 
	
	LPC_GPIO0->FIOPIN2 = LPC_GPIO0->FIOPIN2 | 0x0C;
}

//entre 0 et 1666 ->vitesse faible 400
void reculer(int vitesse){
	LPC_GPIO0->FIOPIN2 = LPC_GPIO0->FIOPIN2 & 0xFC;
	LPC_GPIO0->FIOPIN2 = LPC_GPIO0->FIOPIN2 | 0x02; ////avance INA:0 INB:1 ENA:1 ENB:1 
	LPC_PWM1->MR2 = vitesse-1;    // ceci ajuste la duree de l'etat haut
}

//entre 0 et 1666 ->vitesse faible 400
void avancer(int vitesse){
	LPC_GPIO0->FIOPIN2 = LPC_GPIO0->FIOPIN2 & 0xFC;
	LPC_GPIO0->FIOPIN2 = LPC_GPIO0->FIOPIN2 | 0x01; //recule INA:1 INB:0 ENA:1 ENB:1 //Mettre a 0
	LPC_PWM1->MR2 = vitesse-1;    // ceci ajuste la duree de l'etat haut
}

void freiner(void){
	LPC_GPIO0->FIOPIN2 = LPC_GPIO0->FIOPIN2 & 0xFC;
}

/////////****///////