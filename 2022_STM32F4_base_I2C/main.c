#include "main.h"
#include "cmsis_os.h"                   
#include "Driver_I2C.h"                
#include "Driver_USART.h"
#include "Board_LED.h"                  
#include "RTE_Components.h"           
                

extern ARM_DRIVER_I2C Driver_I2C1;
extern ARM_DRIVER_USART Driver_USART2;

int i=0;

void Init_UART(void){
Driver_USART2.Initialize(NULL);
Driver_USART2.PowerControl(ARM_POWER_FULL);
Driver_USART2.Control( ARM_USART_MODE_ASYNCHRONOUS |
ARM_USART_DATA_BITS_8 |
ARM_USART_STOP_BITS_1 |
ARM_USART_PARITY_NONE |
ARM_USART_FLOW_CONTROL_NONE,
115200);

Driver_USART2.Control(ARM_USART_CONTROL_TX,1);
Driver_USART2.Control(ARM_USART_CONTROL_RX,1);
}

void Init_I2C(void){
	Driver_I2C1.Initialize(NULL);
	Driver_I2C1.PowerControl(ARM_POWER_FULL);
	Driver_I2C1.Control(	ARM_I2C_BUS_SPEED,				
							ARM_I2C_BUS_SPEED_STANDARD  );	   // 100 kHz
}

void write1byte (unsigned char composant, unsigned char registre, unsigned char valeur)
{
	   unsigned char tab[2];
    tab[0] = registre;
		tab[1] = valeur;
		Driver_I2C1.MasterTransmit (composant, tab, 2, false);		
		while (Driver_I2C1.GetStatus().busy == 1);
}


unsigned char read1byte (unsigned char composant, unsigned char registre){

	unsigned char data[6];
	Driver_I2C1.MasterReceive (composant, data, 1, false);		
	while (Driver_I2C1.GetStatus().busy == 1);	
  return data[0]; 
}

static void SystemClock_Config(void);
static void Error_Handler(void);

int main(void)
{
	 uint16_t posX = 0;
	 uint16_t posY = 0;
	 uint16_t accX = 0;
	 uint16_t accY = 0;
	 uint16_t accZ = 0;
	 uint8_t boutons;
	 uint8_t chaine[6];
	
   HAL_Init();
   SystemClock_Config();
   SystemCoreClockUpdate();
	 LED_Initialize ();
	 Init_I2C();
	 Init_UART();
	
	 write1byte(0x52,0xF0,0x55); //envoie de la donnee 0x55	Initialisation 
	 //for(i=0;i<1000;i++){}

  while (1)
  {
		
		write1byte(0x52,0x00,0x00);	 //envoie 00 pour demander la conversion 		
		for(i=0;i<15000;i++){}
			
			
		posX = read1byte (0x52, 0x00); // position x de la nunchuck 
		for(i=0;i<15000;i++){}

		posY = read1byte (0x52,0x01); // position y de la nunchuck 
		for(i=0;i<15000;i++){}
			
		accX = read1byte (0x52, 0x02); // acc x de la nunchuck 
		for(i=0;i<15000;i++){}
			
		accY = read1byte (0x52, 0x03); // acc y de la nunchuck 
		for(i=0;i<15000;i++){}
			
		accZ = read1byte (0x52, 0x04); // acc z de la nunchuck 
		for(i=0;i<15000;i++){}
			
		boutons = read1byte (0x52,0x05);// boutons Z ET C
		for(i=0;i<15000;i++){}
			
			
		boutons=boutons&0x03;
			
		chaine[0]=posX;	
		chaine[1]=posY;
		chaine[2]=accX;
		chaine[3]=accY;
		chaine[4]=accZ;
		chaine[5]=boutons&=0x03;
			
			for(i=0;i<150000;i++){}
			
		while(Driver_USART2.GetStatus().tx_busy == 1);
		Driver_USART2.Send(chaine,6);
				
			osDelay(400);
		
  }
}



static void SystemClock_Config(void)
{
  RCC_ClkInitTypeDef RCC_ClkInitStruct;
  RCC_OscInitTypeDef RCC_OscInitStruct;

  /* Enable Power Control clock */
  __HAL_RCC_PWR_CLK_ENABLE();

  /* The voltage scaling allows optimizing the power consumption when the device is 
     clocked below the maximum system frequency, to update the voltage scaling value 
     regarding system frequency refer to product datasheet.  */
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /* Enable HSE Oscillator and activate PLL with HSE as source */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 336;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 7;
  if(HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    /* Initialization Error */
    Error_Handler();
  }

  /* Select PLL as system clock source and configure the HCLK, PCLK1 and PCLK2 
     clocks dividers */
  RCC_ClkInitStruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2);
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;  
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;  
  if(HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
  {
    /* Initialization Error */
    Error_Handler();
  }

  /* STM32F405x/407x/415x/417x Revision Z devices: prefetch is supported */
  if (HAL_GetREVID() == 0x1001)
  {
    /* Enable the Flash prefetch */
    __HAL_FLASH_PREFETCH_BUFFER_ENABLE();
  }
}

/**
  * @brief  This function is executed in case of error occurrence.
  * @param  None
  * @retval None
  */
static void Error_Handler(void)
{
  /* User may add here some code to deal with this error */
  while(1)
  {
  }
}

#ifdef  USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{ 
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
		
  }
}

#endif

/**
  * @}
  */ 

/**
  * @}
  */ 

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
