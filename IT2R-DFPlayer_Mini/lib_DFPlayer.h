#include "stdio.h"
#include "LPC17xx.h"

#define doc_voiture 1//Les son sous les doc ne sont disponible que dans ce doc
#define son_klaxon 1
#define son_klaxon2 6
#define son_klaxon3 7
#define son_moteur 2
#define son_demarrageMot 4
#define son_moteurRS6 9
#define son_drift 3
#define son_clignotant 5


#define doc_ultrason 0x02
#define son_alerteGeneral 0x02





void init_UART2(void);
void choisir_music(uint8_t doc, uint8_t son);
