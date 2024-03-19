#include "stdio.h"
#include "LPC17xx.h"
#include "lib_DFPlayer.h"

int main(void){
	int i;
	
	init_UART2();
      
	
	choisir_music(0x0707);
	for(i=0;i<10000;i++);
	
	while(1){

	
	
	}
	
	return 0;
}

