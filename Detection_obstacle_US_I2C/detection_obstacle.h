/*
BIBLIOTHEQUE DES FONCTIONS POUR L'UTILISATION DES CAPTEURS ULTRASON SRF10 (I2C)
*/

#include "Driver_I2C.h"

/*
définition des events des tâches manipulés par les fonctions callback.
Utilisés partout et peuvent être modifiés à volonté.
*/

#define CAN_EVENT 0x04

#define US_EVENT 0x01
#define US_CAN_EVENT 0x02

#define ID_TRAME_ALERTE 0x0002
#define ID_TRAME_PARK 0x0001

//d�finition adresse/numéro capteurs
#define CAPTEUR_ARG 0x70 //emplacement : ARRIERE GAUCHE (E0)
#define CAPTEUR_ARD 0x71 //emplacement : ARRIERE DROITE (E2)
#define CAPTEUR_AVG 0x72 //emplacement : AVANT GAUCHE (E4)
#define CAPTEUR_AVC 0x73 //emplacement : AVANT CENTRE (E6)
#define CAPTEUR_AVD 0x74 //emplacement : AVANT DROITE (E8)

//définition de la portee
#define PORTEE 0x18

typedef struct{
	uint8_t gauche, droite, centre, alerte;
}mailBox;

//NE PAS MANIPULER CETTE FONCTION SVP
void adressage(uint8_t adresseCible, uint8_t adresseComposant);

void config_portee(uint8_t portee, uint8_t adresse);

//FONCTIONS RTOS
void configI2C_RTOS(void);
uint8_t mesureUS_RTOS(uint8_t capteur);
void mesureUS_RTOSBis(mailBox *mesureDist);
