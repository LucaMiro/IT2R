#include "Driver_I2C.h"

//définition adresse/numéro capteurs
#define CAPTEUR_E0 0x70 //emplacement :
#define CAPTEUR_E4 0x72 //emplacement :
#define CAPTEUR_E6 0x73 //emplacement :
#define CAPTEUR_E8 0x74 //emplacement :
#define CAPTEUR_YY 0xYY //emplacement :
#define CAPTEUR_ZZ 0xZZ //emplacement :

//définition de la portée
#define PORTEE_MAX 0xFF
#define PORTEE_DEMI 0x80
#define PORTEE_4M 0x5D
#define PORTEE_2M 0x2F
#define PORTEE_1M 0x18
int configuration_liaisonI2C(void);

void config_portee(uint8_t portee, uint8_t adresse);

void test(uint8_t adresse);

uint8_t mesure_cm(uint8_t adresse);

