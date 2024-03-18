#include "stdio.h"
#include "LPC17xx.h"
#include "lib_DFPlayer.h"

int main(void){
	init_UART2();
	init_DFPlayer();
	choisir_music(0x0002);
	
	while(1){
			
			
	}
	
	return 0;
}

