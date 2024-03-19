/*
BIBLIOTHEQUE DES FONCTIONS POUR L'UTILISATION DES CAPTEURS ULTRASON SRF10 (I2C)
*/

#include "Driver_I2C.h"

/*
d�finition de l'event de la t�che US modifi� par la fonction callback.
Utilis� partout et peut �tre modifi� � volont�.
*/

#define US_EVENT 0x0002

//d�finition adresse/num�ro capteurs
#define CAPTEUR_E0 0x70 //emplacement :
#define CAPTEUR_E2 0x71 //emplacement :
#define CAPTEUR_E4 0x72 //emplacement :
#define CAPTEUR_E6 0x73 //emplacement :
#define CAPTEUR_E8 0x74 //emplacement :

//d�finition de la port�e
#define PORTEE_MAX 0xFF
#define PORTEE_DEMI 0x80
#define PORTEE_4M 0x5D
#define PORTEE_2M 0x2F
#define PORTEE_1M 0x18

//NE PAS MANIPULER CETTE FONCTION SVP
void adressage(uint8_t adresseCible, uint8_t adresseComposant);

void config_portee(uint8_t portee, uint8_t adresse);

//FONCTIONS RTOS
void configI2C_RTOS(void);
uint8_t mesureUS_RTOS(uint8_t capteur);

