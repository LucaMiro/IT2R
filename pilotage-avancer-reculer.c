#include "stdio.h"
#include "LPC17xx.h"  
#include "GPIO.h"

int j=0;

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
void avancer(int vitesse){
	LPC_GPIO0->FIOPIN2 = LPC_GPIO0->FIOPIN2 & 0xFC;
	LPC_GPIO0->FIOPIN2 = LPC_GPIO0->FIOPIN2 | 0x02; ////avance INA:0 INB:1 ENA:1 ENB:1 
	LPC_PWM1->MR2 = vitesse-1;    // ceci ajuste la duree de l'etat haut
}

//entre 0 et 1666 ->vitesse faible 400
void reculer(int vitesse){
	LPC_GPIO0->FIOPIN2 = LPC_GPIO0->FIOPIN2 & 0xFC;
	LPC_GPIO0->FIOPIN2 = LPC_GPIO0->FIOPIN2 | 0x01; //recule INA:1 INB:0 ENA:1 ENB:1 //Mettre a 0
	LPC_PWM1->MR2 = vitesse-1;    // ceci ajuste la duree de l'etat haut
}

void freiner(void){
	LPC_GPIO0->FIOPIN2 = LPC_GPIO0->FIOPIN2 & 0xFC;
}

int main(void){
	
	Initialise_GPIO();
	Init_moteur();
	
	while(1){
		
		
	avancer(600);
	for(j=0;j<5000000;j++){}
		freiner();
		for(j=0;j<5000000;j++){}
	reculer(600);
	for(j=0;j<5000000;j++){}
		freiner();
	for(j=0;j<5000000;j++){}
	}
	


	
	return 0;
}

