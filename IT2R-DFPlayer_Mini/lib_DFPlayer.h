#include "stdio.h"
#include "LPC17xx.h"

void init_UART2(void);
void init_DFPlayer(void);
void choisir_music(uint16_t music);
void lancer(uint16_t channel);
void demarrerSon(void);
void monterSon(char volume);						//volume de 1 à 10