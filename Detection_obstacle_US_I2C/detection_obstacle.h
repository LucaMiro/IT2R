#include "Driver_I2C.h"

//définition adresse/numéro capteurs
#define CAPTEUR_E0 0x70 //emplacement :
#define CAPTEUR_E2 0x71 //emplacement :
#define CAPTEUR_E4 0x72 //emplacement :
#define CAPTEUR_E6 0x73 //emplacement :
#define CAPTEUR_E8 0x74 //emplacement :

//définition de la portée
#define PORTEE_MAX 0xFF
#define PORTEE_DEMI 0x80
#define PORTEE_4M 0x5D
#define PORTEE_2M 0x2F
#define PORTEE_1M 0x18

//fonctions I2C / Ultrason
int configuration_liaisonI2C(void);

//NE PAS MANIPULER CETTE FONCTION SVP
void adressage(uint8_t adresseCible, uint8_t adresseComposant);

void config_portee(uint8_t portee, uint8_t adresse);
void test(uint8_t adresse);
uint8_t mesure_cm(uint8_t adresse);

