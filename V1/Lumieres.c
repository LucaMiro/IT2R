#include "stm32f4xx_hal.h"
#include "Driver_SPI.h"                 // ::CMSIS Driver:SPI

extern ARM_DRIVER_SPI Driver_SPI1;






void Lum_init(void){
	
	int i=0;


	RCC->APB2ENR |= (1<<8);
	RCC->AHB1ENR |= (1<<0); //GPIOA
	
	GPIOA->MODER |= (3<<2); //PA1 en analog
	
	ADC1->SQR3 |= (1<<0);
	ADC1->CR2 |= (1<<0);


	Driver_SPI1.Initialize(NULL);
  Driver_SPI1.PowerControl(ARM_POWER_FULL);

  Driver_SPI1.Control(ARM_SPI_MODE_MASTER | ARM_SPI_CPOL1_CPHA1 | ARM_SPI_MSB_LSB | ARM_SPI_SS_MASTER_SW | ARM_SPI_DATA_BITS(8), 10000000);	
	Driver_SPI1.Control(ARM_SPI_CONTROL_SS, ARM_SPI_SS_INACTIVE);
}

char* pilotageLED(char nbLED, char lumi, char bleu, char vert, char rouge, char tab[])
{
	int i=0,mod;
	if (nbLED ==0)
	{
		for(i=0;i<247;i++){
		mod = i%4;
		if (i<4)
			{tab[i]=0x00;}
		else if (i>=244)
			{tab[i]=0xFF;}
		else 
			{
				if (mod ==0)
				{tab[i]=lumi;}
				
				else if (mod ==1)
				{tab[i]=bleu;}
				
				else if (mod ==2)
				{tab[i]=vert;}
				
				else if (mod ==3)
				{tab[i]=rouge;}
			
			}
		}
	}
	else 
	{
		mod = nbLED*4;
		tab[mod]= lumi;
		tab[mod+1]= bleu;
		tab[mod+2]= vert;
		tab[mod+3]= rouge;
	}
	return tab;
}

short LectureADC(){
	short lesho;
	ADC1->CR2 |= (1<<30);
	while((ADC1->SR & (1<<1)) == 0);
	lesho = ADC1->DR;
	return lesho;
}


void tabSend(char tab[]){
	Driver_SPI1.Control(ARM_SPI_CONTROL_SS, ARM_SPI_SS_ACTIVE);
	Driver_SPI1.Send(tab,248);
	Driver_SPI1.Control(ARM_SPI_CONTROL_SS, ARM_SPI_SS_INACTIVE);
}
