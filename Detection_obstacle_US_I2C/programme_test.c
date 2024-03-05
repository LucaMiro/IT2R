#include "stm32f4xx_hal.h"
#include "detection_obstacle.h"
#include "Board_LED.h"

extern ARM_DRIVER_I2C Driver_I2C1;

//programme de test pour déterminer les adresses des capteurs
int main(void){
	int i=0,j=0;
	uint8_t adresse, portee;

	configuration_liaisonI2C();
	LED_Initialize();
	
	while(1){
		mesure_cm(adresse);
		for(i=0;i<100000;i++);
		i=0;
	}
	return(0);
}
