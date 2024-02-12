#include "stm32f4xx_hal.h"
#include "detection_obstacle.h"
#include "Board_LED.h"

extern ARM_DRIVER_I2C Driver_I2C1;

//programme de test pour déterminer les adresses des capteurs
int main(void){
	int i=0, j=0, attente=0;
	uint8_t adresse;
	configuration_liaisonI2C();
	while(1){
		for(i=0;i<16;i++){
			for(j=0;j<50;j++){
				attente=0;
				test(adresse);
				for(attente=0;attente<1000;attente++);
			}
			adresse++;
		}
	}
	return(0);
}
