#include "stdio.h"
#include "LPC17xx.h"
#include "lib_DFPlayer.h"

int main(void){
	int i;
	
	init_UART2();
      
	
	choisir_music(doc_ultrason,son_alerteGeneral);
	for(i=0;i<10000;i++);
	
	while(1){

	
	
	}
	
	return 0;
}

