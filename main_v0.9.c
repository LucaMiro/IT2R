#include "stm32f4xx_hal.h"
#include "Driver_SPI.h"                 // ::CMSIS Driver:SPI
char* pilotageLED(char nbLED, char lumi, char bleu, char vert, char rouge, char tab[]);
char vallum(void);
extern ARM_DRIVER_SPI Driver_SPI1;
//extern GPIO_InitTypeDef  GPIO;


int main (void)
{
	

	int i=0;
	char tab[248];
	short lesho=0;
//	RCC_AHB1PeriphClockCmd(GPIO, ENABLE); 
//	GPIO.GPIO_Pin = GPIO_Pin_PA1
//	GPIO.GPIO_Mode = GPIO_Mode_OUT;
	 uint32_t GPIO_Pin;  
//HAL_GPIO_Init();
	for(i=0;i<247;i++){
		if (i<4)tab[i]=0x00;
		else if (i>=244)tab[i]=0xFF;
		else tab[i]=0xFF;
		
	}	
	

	
	RCC->APB2ENR |= (1<<8);
	RCC->AHB1ENR |= (1<<0); //GPIOA
	
	GPIOA->MODER |= (3<<2); //PA1 en analog
	
	ADC1->SQR3 |= (1<<0);
	ADC1->CR2 |= (1<<0);


	Driver_SPI1.Initialize(NULL);
  Driver_SPI1.PowerControl(ARM_POWER_FULL);

  Driver_SPI1.Control(ARM_SPI_MODE_MASTER | ARM_SPI_CPOL1_CPHA1 | ARM_SPI_MSB_LSB | ARM_SPI_SS_MASTER_SW | ARM_SPI_DATA_BITS(8), 10000000);	
	Driver_SPI1.Control(ARM_SPI_CONTROL_SS, ARM_SPI_SS_INACTIVE);

//	Driver_SPI1.Control(ARM_SPI_CONTROL_SS, ARM_SPI_SS_ACTIVE);
//	Driver_SPI1.Send(tab, 248);

	while(1)
	{
//		Driver_SPI1.Control(ARM_SPI_CONTROL_SS, ARM_SPI_SS_ACTIVE);
//		Driver_SPI1.Send(tab, 248);
//		Driver_SPI1.Control(ARM_SPI_CONTROL_SS, ARM_SPI_SS_INACTIVE);
//		Driver_SPI1.Control(ARM_SPI_CONTROL_SS, ARM_SPI_SS_ACTIVE);
//		Driver_SPI1.Send(pilotageLED(0,0xE1,0x10,0x00,0x00,tab),248);
//		Driver_SPI1.Control(ARM_SPI_CONTROL_SS, ARM_SPI_SS_INACTIVE);
//		Driver_SPI1.Control(ARM_SPI_CONTROL_SS, ARM_SPI_SS_ACTIVE);
//		Driver_SPI1.Send(pilotageLED(60,0xFF,0x30,0x61,0x31,tab),248);
//		Driver_SPI1.Control(ARM_SPI_CONTROL_SS, ARM_SPI_SS_INACTIVE);
//		Driver_SPI1.Control(ARM_SPI_CONTROL_SS, ARM_SPI_SS_ACTIVE);
//		Driver_SPI1.Send(pilotageLED(50,0xFF,0xF0,0x61,0x01,tab),248);
//		Driver_SPI1.Control(ARM_SPI_CONTROL_SS, ARM_SPI_SS_INACTIVE);
		
		ADC1->CR2 |= (1<<30);
		while((ADC1->SR & (1<<1)) == 0);
		
		lesho = ADC1->DR;
		
		if (lesho>= 3000)
		{
			Driver_SPI1.Control(ARM_SPI_CONTROL_SS, ARM_SPI_SS_ACTIVE);
		Driver_SPI1.Send(pilotageLED(0,0xE3,0x00,0xFF,0xFF,tab),248);
		Driver_SPI1.Control(ARM_SPI_CONTROL_SS, ARM_SPI_SS_INACTIVE);
		}
		else{
			Driver_SPI1.Control(ARM_SPI_CONTROL_SS, ARM_SPI_SS_ACTIVE);
		Driver_SPI1.Send(pilotageLED(0,0xE1,0x55,0x00,0x55,tab),248);
		Driver_SPI1.Control(ARM_SPI_CONTROL_SS, ARM_SPI_SS_INACTIVE);

		}
		
		
		
	}
	
	//return 0;

}

// nbLED si 0 change tout le bandeau LED si entre 1 et 60 change la LED correspondante
// lumi commande la luminosité, attention les 3 bits de poids fort doivent être a 1
// bleu commande la couleur bleue sur un octet, idem pour les autres couleurs
// tab le tableau contenant les valeurs a envoyer au bandeau LED
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
char vallum(void){

}
