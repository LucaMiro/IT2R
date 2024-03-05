#include "stm32f4xx_hal.h"
#include "Driver_SPI.h"                 // ::CMSIS Driver:SPI
char* pilotageLED(char nbLED, char lumi, char bleu, char vert, char rouge, char tab[]);
void ADC_Initialize(ADC_HandleTypeDef *ADCHandle);
char vallum(void);
extern ARM_DRIVER_SPI Driver_SPI1;
//extern GPIO_InitTypeDef  GPIO;
ADC_HandleTypeDef *ADC1_CH1;

int main (void)
{
	

	int i=0;
	char tab[248];
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
	
	ADC_Initialize(ADC1_CH1);
	HAL_ADC_Start(ADC1_CH1); // start A/D conversion

		while(HAL_ADC_PollForConversion(ADC1_CH1, HAL_MAX_DELAY) != HAL_OK);  //Check if conversion is completed
		
		HAL_ADC_GetValue(ADC1_CH1); // read digital value when conversion is completed
		
		HAL_ADC_Stop(ADC1_CH1); // stop conversion
	

	Driver_SPI1.Initialize(NULL);
  Driver_SPI1.PowerControl(ARM_POWER_FULL);

  Driver_SPI1.Control(ARM_SPI_MODE_MASTER | ARM_SPI_CPOL1_CPHA1 | ARM_SPI_MSB_LSB | ARM_SPI_SS_MASTER_SW | ARM_SPI_DATA_BITS(8), 10000000);	
	Driver_SPI1.Control(ARM_SPI_CONTROL_SS, ARM_SPI_SS_INACTIVE);

//	Driver_SPI1.Control(ARM_SPI_CONTROL_SS, ARM_SPI_SS_ACTIVE);
//	Driver_SPI1.Send(tab, 248);

	while(1)
	{
		Driver_SPI1.Control(ARM_SPI_CONTROL_SS, ARM_SPI_SS_ACTIVE);
		Driver_SPI1.Send(tab, 248);
		Driver_SPI1.Control(ARM_SPI_CONTROL_SS, ARM_SPI_SS_INACTIVE);
		Driver_SPI1.Control(ARM_SPI_CONTROL_SS, ARM_SPI_SS_ACTIVE);
		Driver_SPI1.Send(pilotageLED(0,0xE1,0x10,0x00,0x00,tab),248);
		Driver_SPI1.Control(ARM_SPI_CONTROL_SS, ARM_SPI_SS_INACTIVE);
		Driver_SPI1.Control(ARM_SPI_CONTROL_SS, ARM_SPI_SS_ACTIVE);
		Driver_SPI1.Send(pilotageLED(60,0xFF,0x30,0x61,0x31,tab),248);
		Driver_SPI1.Control(ARM_SPI_CONTROL_SS, ARM_SPI_SS_INACTIVE);
		Driver_SPI1.Control(ARM_SPI_CONTROL_SS, ARM_SPI_SS_ACTIVE);
		Driver_SPI1.Send(pilotageLED(50,0xFF,0xF0,0x61,0x01,tab),248);
		Driver_SPI1.Control(ARM_SPI_CONTROL_SS, ARM_SPI_SS_INACTIVE);
		
		
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
void ADC_Initialize(ADC_HandleTypeDef *ADCHandle)
{	
	ADC_ChannelConfTypeDef Channel_AN; // create an instance of ADC_ChannelConfTypeDef
	GPIO_InitTypeDef ADCpin; //create an instance of GPIO_InitTypeDef C struct
	
//ADC module selection (ADC1, ADC2 or ADC3)	
	__HAL_RCC_ADC1_CLK_ENABLE(); // enable clock to ADC1 module
	ADCHandle->Instance = ADC1; // create an instance of ADC1
	ADCHandle->Init.Resolution = ADC_RESOLUTION_12B; // select 12-bit resolution 
	ADCHandle->Init.EOCSelection = ADC_EOC_SINGLE_CONV; //select  single conversion as a end of conversion event
	ADCHandle->Init.DataAlign = ADC_DATAALIGN_RIGHT; // set digital output data right justified
	ADCHandle->Init.ClockPrescaler =ADC_CLOCK_SYNC_PCLK_DIV8; 
	HAL_ADC_Init(ADCHandle); // initialize AD1 with myADC1Handle configuration settings
	
		
				__HAL_RCC_GPIOA_CLK_ENABLE(); // enable clock to GPIOA
				ADCpin.Pin = GPIO_PIN_1; // AN1 => PA1 => Select pin 1 from GPIO A
				ADCpin.Mode = GPIO_MODE_ANALOG; // Select Analog Mode
				ADCpin.Pull = GPIO_NOPULL; // Disable internal pull-up or pull-down resistor
				HAL_GPIO_Init(GPIOA, &ADCpin); // initialize PA1 as analog input pin

				Channel_AN.Channel = ADC_CHANNEL_1; // AN1 => select analog channel 1
		
	

	Channel_AN.Rank = 1; // set rank to 1
	Channel_AN.SamplingTime = ADC_SAMPLETIME_15CYCLES; // set sampling time to 15 clock cycles
	HAL_ADC_ConfigChannel(ADCHandle, &Channel_AN); // select channel_8 for ADC1 module. 
}
