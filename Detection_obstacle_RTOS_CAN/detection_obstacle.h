/*
BIBLIOTHEQUE DES FONCTIONS POUR L'UTILISATION DES CAPTEURS ULTRASON SRF10 (I2C)
*/

#define ID_TRAME_CAN 0x01
#include "Driver_I2C.h"
#include "stm32f4xx.h"
/*
définition events des tâches manipulés par les fonctions callback.
Utilisés partout et peuvent être modifiés à volonté.
*/

#define US_EV 0x01
#define CAN_EV 0x02
#define PARK_DEBUT 0x04
#define PARK_FIN 0x08

//indication adresses capteurs
#define CAPTEUR_ARG 0x70 //emplacement : ARRIERE GAUCHE (E0)
#define CAPTEUR_ARD 0x71 //emplacement : ARRIERE DROITE (E2)
#define CAPTEUR_AVG 0x72 //emplacement : AVANT GAUCHE (E4)
#define CAPTEUR_AVC 0x73 //emplacement : AVANT CENTRE (E6)
#define CAPTEUR_AVD 0x74 //emplacement : AVANT DROITE (E8)
#define CAPTEUR_LTR 0x75 //emplacement : LATERAL (F0) *parking*

//définition portée
#define PORTEE 0x18 /* à définir en groupe, j'ai ici mis 1 mètre */

//fonction test si adressage correct et capteur opérationnel
void test_capteurs(void);

void config_portee(uint8_t portee, uint8_t adresse);
//NE PAS MANIPULER CETTE FONCTION SVP
void adressage(uint8_t adresseCible, uint8_t adresseComposant);

//FONCTIONS RTOS
void configI2C(void);
uint8_t mesureLaterale(void);
uint8_t mesureUS(uint8_t capteur);
void mesureAvant(void);
void mesureArriere(void);
