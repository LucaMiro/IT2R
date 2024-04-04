#include "stm32f4xx_hal.h"
#include "Driver_SPI.h"                 // ::CMSIS Driver:SPI
#include "cmsis_os.h"                   // ARM::CMSIS:RTOS:Keil RTX
#include "Lumieres.h"
#include "Driver_CAN.h"                 // CMSIS Driver:CAN
#include "RTE_Device.h"                 // Device:STM32Cube Framework:Classic


extern ARM_DRIVER_SPI Driver_SPI1;
extern ARM_DRIVER_CAN Driver_CAN2;

ARM_CAN_MSG_INFO rx_msg_info;

uint8_t data_buf[8];
char tab[248];
char activ;
	
void Capte (void const * argument);
void Allumage(void const * argument);
void Clignotant(void const * argument);
void ClignotantD(void const * argument);
void ClignotantG(void const * argument);
void Freinage(void const * argument);
void AppelPhare(void const * argument);
void Recul(void const * argument);
void Simu(void const * argument);
void Arc(void const * argument);
void CAN(void const * argument);

void InitCAN(void);

static void SystemClock_Config(void);
static void Error_Handler(void);

osThreadId  ID_CAPTE;
osThreadId  ID_ALLUMAGE;
osThreadId  ID_CLIGNOTANT;
osThreadId  ID_CLIGNOTANTD;
osThreadId  ID_CLIGNOTANTG;
osThreadId  ID_FREINAGE;
osThreadId  ID_APPELPHARE;
osThreadId  ID_RECUL;
osThreadId  ID_SIMU;
osThreadId  ID_ARC;
osThreadId  ID_CAN;

osThreadDef (Capte, osPriorityBelowNormal, 1, 0);
osThreadDef (Allumage, osPriorityNormal, 1, 0);
osThreadDef (Clignotant, osPriorityNormal, 1, 0);
osThreadDef (ClignotantD, osPriorityNormal, 1, 0);
osThreadDef (ClignotantG, osPriorityNormal, 1, 0);
osThreadDef (Freinage, osPriorityNormal, 1, 0);
osThreadDef (AppelPhare, osPriorityNormal, 1, 0);
osThreadDef (Recul, osPriorityNormal, 1, 0);
osThreadDef (Simu, osPriorityHigh, 1, 0);
osThreadDef (Arc, osPriorityNormal, 1, 0);
osThreadDef (CAN, osPriorityBelowNormal, 1, 0);

osMutexId ID_mut;
osMutexDef(mut);

void CAN_callback(uint32_t obj_idx, uint32_t event)
{
	if (event & ARM_CAN_EVENT_RECEIVE)
	{
		Driver_CAN2.MessageRead(0, &rx_msg_info, data_buf,8);
		tabSend(pilotageLED(12,0xE1, 0xFF, 0x00, 0xFF, tab));
		osSignalSet(ID_CAN, 0x0001);
}
}

void InitCAN(void)
{
	
}

int main (void)
{
	osKernelInitialize ();                    // initialize CMSIS-RTOS

//	HAL_Init();
	
	SystemClock_Config();
  SystemCoreClockUpdate();
	
	Lum_init();
	InitCAN();
	
	Driver_CAN2.Initialize(NULL,CAN_callback);
	Driver_CAN2.PowerControl(ARM_POWER_FULL);
	
	Driver_CAN2.SetMode(ARM_CAN_MODE_INITIALIZATION);
	Driver_CAN2.SetBitrate(ARM_CAN_BITRATE_NOMINAL,
											125000,
											ARM_CAN_BIT_PROP_SEG(5U)  |
											ARM_CAN_BIT_PHASE_SEG1(1U)|
											ARM_CAN_BIT_PHASE_SEG2(1U)|
											ARM_CAN_BIT_SJW(1U));
	
	Driver_CAN2.ObjectConfigure(0,ARM_CAN_OBJ_RX);
	Driver_CAN2.ObjectConfigure(2,ARM_CAN_OBJ_TX);
	//Driver_CAN2.ObjectSetFilter(0, ARM_CAN_FILTER_ID_MASKABLE_ADD, ARM_CAN_STANDARD_ID(0xFFF),0xFFF);
	Driver_CAN2.SetMode(ARM_CAN_MODE_NORMAL);
	
	tabSend(pilotageLED(0,0xE0, 0x00, 0x00, 0x00, tab));
		
	ID_mut = osMutexCreate(osMutex(mut));
	//ID_CAPTE = osThreadCreate ( osThread ( Capte ), NULL ) ;
	ID_ALLUMAGE = osThreadCreate ( osThread ( Allumage ), NULL ) ;
	ID_CLIGNOTANT = osThreadCreate ( osThread ( Clignotant ), NULL ) ;
	ID_CLIGNOTANTD = osThreadCreate ( osThread ( ClignotantD ), NULL ) ;
	ID_CLIGNOTANTG = osThreadCreate ( osThread ( ClignotantG ), NULL ) ;
	ID_FREINAGE = osThreadCreate ( osThread ( Freinage ), NULL ) ;
	ID_APPELPHARE = osThreadCreate ( osThread ( AppelPhare ), NULL ) ;
	ID_RECUL = osThreadCreate ( osThread ( Recul ), NULL ) ;
	//ID_SIMU = osThreadCreate ( osThread ( Simu ), NULL ) ;
  //ID_ARC = osThreadCreate ( osThread ( Arc ), NULL ) ;
	ID_CAN = osThreadCreate ( osThread ( CAN ), NULL ) ;
	
	osKernelStart ();
	osDelay(osWaitForever); 

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


static void Error_Handler(void)
{
  /* User may add here some code to deal with this error */
  while(1)
  {
  }
}

void Capte (void const * argument)
{
	short val;
	while(1)
	{
		if (activ == 15)
		{
			osSignalWait(0x0001,osWaitForever);
		}
		else
		{
			val = LectureADC();
			if (val >=3000)
			{

				//pilotageLED(0,0xE1, 0xFF, 0x01, 0x80, tab);
				pilotageLED(12,0xE1, 0xFF, 0xFF, 0xFF, tab);
				pilotageLED(18,0xE1, 0xFF, 0xFF, 0xFF, tab);
				pilotageLED(11,0xE1, 0xFF, 0xFF, 0xFF, tab);
				pilotageLED(19,0xE1, 0xFF, 0xFF, 0xFF, tab);
				pilotageLED(10,0xE1, 0xFF, 0xFF, 0xFF, tab);
				pilotageLED(20,0xE1, 0xFF, 0xFF, 0xFF, tab);
				
				pilotageLED(30,0xE5, 0x00, 0x00, 0xFF, tab);
				pilotageLED(40,0xE5, 0x00, 0x00, 0xFF, tab);
				pilotageLED(31,0xE5, 0x00, 0x00, 0xFF, tab);
				pilotageLED(39,0xE5, 0x00, 0x00, 0xFF, tab);
				pilotageLED(32,0xE5, 0x00, 0x00, 0xFF, tab);
				pilotageLED(38,0xE5, 0x00, 0x00, 0xFF, tab);
			}
			else
			{
				//pilotageLED(0,0xFF, 0x00, 0x40, 0xFF, tab);
				pilotageLED(12,0xFF, 0xFF, 0xFF, 0xFF, tab);
				pilotageLED(18,0xFF, 0xFF, 0xFF, 0xFF, tab);
				pilotageLED(11,0xFF, 0xFF, 0xFF, 0xFF, tab);
				pilotageLED(19,0xFF, 0xFF, 0xFF, 0xFF, tab);
				pilotageLED(10,0xFF, 0xFF, 0xFF, 0xFF, tab);
				pilotageLED(20,0xFF, 0xFF, 0xFF, 0xFF, tab);
				
				pilotageLED(30,0xEB, 0x00, 0x00, 0xFF, tab);
				pilotageLED(40,0xEB, 0x00, 0x00, 0xFF, tab);
				pilotageLED(31,0xEB, 0x00, 0x00, 0xFF, tab);
				pilotageLED(39,0xEB, 0x00, 0x00, 0xFF, tab);
				pilotageLED(32,0xE5, 0x00, 0x00, 0xFF, tab);
				pilotageLED(38,0xE5, 0x00, 0x00, 0xFF, tab);
				}
			}
//	osMutexWait(ID_mut,osWaitForever);		
		tabSend(tab);
		osDelay(5);
//	osMutexRelease(ID_mut);		
	}
}

void Allumage(void const * argument)
{
	while(1)
	{
		osSignalWait(0x0001, osWaitForever); // attente activation par le RFID (la c'est le simu)
		
		pilotageLED(10,0xFF, 0x00, 0x80, 0xFF, tab);
		pilotageLED(20,0xFF, 0x00, 0x80, 0xFF, tab);
		pilotageLED(30,0xFF, 0x00, 0x80, 0xFF, tab);
		tabSend(pilotageLED(40,0xFF, 0x00, 0x80, 0xFF, tab));
		osDelay(200);
		
		pilotageLED(11,0xFF, 0x00, 0x80, 0xFF, tab);
		pilotageLED(19,0xFF, 0x00, 0x80, 0xFF, tab);
		pilotageLED(31,0xFF, 0x00, 0x80, 0xFF, tab);
		tabSend(pilotageLED(39,0xFF, 0x00, 0x80, 0xFF, tab));
		osDelay(200);
		
		pilotageLED(12,0xFF, 0x00, 0x80, 0xFF, tab);
		pilotageLED(18,0xFF, 0x00, 0x80, 0xFF, tab);
		pilotageLED(32,0xFF, 0x00, 0x80, 0xFF, tab);
		tabSend(pilotageLED(38,0xFF, 0x00, 0x80, 0xFF, tab));
		osDelay(300);
		
		pilotageLED(10,0xE0, 0x00, 0x00, 0x00, tab);
		pilotageLED(20,0xE0, 0x00, 0x00, 0x00, tab);
		pilotageLED(11,0xE0, 0x00, 0x00, 0x00, tab);
		pilotageLED(19,0xE0, 0x00, 0x00, 0x00, tab);
		pilotageLED(12,0xE0, 0x00, 0x00, 0x00, tab);
		pilotageLED(18,0xE0, 0x00, 0x00, 0x00, tab);
		pilotageLED(30,0xE0, 0x00, 0x00, 0x00, tab);
		pilotageLED(40,0xE0, 0x00, 0x00, 0x00, tab);
		pilotageLED(31,0xE0, 0x00, 0x00, 0x00, tab);
		pilotageLED(39,0xE0, 0x00, 0x00, 0x00, tab);
		pilotageLED(32,0xE0, 0x00, 0x00, 0x00, tab);
		tabSend(pilotageLED(38,0xE0, 0x00, 0x00, 0x00, tab));
		osDelay(300);
		
		pilotageLED(10,0xFF, 0x00, 0x80, 0xFF, tab);
		pilotageLED(20,0xFF, 0x00, 0x80, 0xFF, tab);
		pilotageLED(30,0xFF, 0x00, 0x80, 0xFF, tab);
		tabSend(pilotageLED(40,0xFF, 0x00, 0x80, 0xFF, tab));
		osDelay(200);
		
		pilotageLED(11,0xFF, 0x00, 0x80, 0xFF, tab);
		pilotageLED(19,0xFF, 0x00, 0x80, 0xFF, tab);
		pilotageLED(31,0xFF, 0x00, 0x80, 0xFF, tab);
		tabSend(pilotageLED(39,0xFF, 0x00, 0x80, 0xFF, tab));
		osDelay(200);
		
		pilotageLED(12,0xFF, 0x00, 0x80, 0xFF, tab);
		pilotageLED(18,0xFF, 0x00, 0x80, 0xFF, tab);
		pilotageLED(32,0xFF, 0x00, 0x80, 0xFF, tab);
		tabSend(pilotageLED(38,0xFF, 0x00, 0x80, 0xFF, tab));
		osDelay(300);
		
		pilotageLED(10,0xE0, 0x00, 0x00, 0x00, tab);
		pilotageLED(20,0xE0, 0x00, 0x00, 0x00, tab);
		pilotageLED(11,0xE0, 0x00, 0x00, 0x00, tab);
		pilotageLED(19,0xE0, 0x00, 0x00, 0x00, tab);
		pilotageLED(12,0xE0, 0x00, 0x00, 0x00, tab);
		pilotageLED(18,0xE0, 0x00, 0x00, 0x00, tab);
		pilotageLED(30,0xE0, 0x00, 0x00, 0x00, tab);
		pilotageLED(40,0xE0, 0x00, 0x00, 0x00, tab);
		pilotageLED(31,0xE0, 0x00, 0x00, 0x00, tab);
		pilotageLED(39,0xE0, 0x00, 0x00, 0x00, tab);
		pilotageLED(32,0xE0, 0x00, 0x00, 0x00, tab);
		tabSend(pilotageLED(38,0xE0, 0x00, 0x00, 0x00, tab));
		osDelay(300);
		
		pilotageLED(10,0xFF, 0xFF, 0xFF, 0xFF, tab);
		pilotageLED(20,0xFF, 0xFF, 0xFF, 0xFF, tab);
		pilotageLED(30,0xE5, 0x00, 0x00, 0xFF, tab);
		tabSend(pilotageLED(40,0xE5, 0x00, 0x00, 0xFF, tab));
		osDelay(5);
		
		pilotageLED(11,0xFF, 0xFF, 0xFF, 0xFF, tab);
		pilotageLED(19,0xFF, 0xFF, 0xFF, 0xFF, tab);
		pilotageLED(31,0xE5, 0x00, 0x00, 0xFF, tab);
		tabSend(pilotageLED(39,0xE5, 0x00, 0x00, 0xFF, tab));
		
		pilotageLED(10,0xE0, 0x00, 0x00, 0x00, tab);
		pilotageLED(20,0xE0, 0x00, 0x00, 0x00, tab);
		pilotageLED(30,0xE0, 0x00, 0x00, 0x00, tab);
		tabSend(pilotageLED(40,0xE0, 0x00, 0x00, 0x00, tab));
		osDelay(5);
		
		pilotageLED(12,0xFF, 0xFF, 0xFF, 0xFF, tab);
		pilotageLED(18,0xFF, 0xFF, 0xFF, 0xFF, tab);
		pilotageLED(32,0xE5, 0x00, 0x00, 0xFF, tab);
		tabSend(pilotageLED(38,0xE5, 0x00, 0x00, 0xFF, tab));
		
		pilotageLED(11,0xE0, 0x00, 0x00, 0x00, tab);
		pilotageLED(19,0xE0, 0x00, 0x00, 0x00, tab);
		pilotageLED(31,0xE0, 0x00, 0x00, 0x00, tab);
		tabSend(pilotageLED(39,0xE0, 0x00, 0x00, 0x00, tab));
		osDelay(1000);
		
		pilotageLED(10,0xFF, 0xFF, 0xFF, 0xFF, tab);
		pilotageLED(20,0xFF, 0xFF, 0xFF, 0xFF, tab);
		pilotageLED(30,0xFF, 0x00, 0x00, 0xFF, tab);
		tabSend(pilotageLED(40,0xFF, 0x00, 0x00, 0xFF, tab));
		osDelay(5);
		
		pilotageLED(11,0xFF, 0xFF, 0xFF, 0xFF, tab);
		pilotageLED(19,0xFF, 0xFF, 0xFF, 0xFF, tab);
		pilotageLED(10,0xE0, 0x00, 0x00, 0x00, tab);
		pilotageLED(20,0xE0, 0x00, 0x00, 0x00, tab);
		pilotageLED(31,0xE5, 0x00, 0x00, 0xFF, tab);
		pilotageLED(39,0xE5, 0x00, 0x00, 0xFF, tab);
		pilotageLED(30,0xE0, 0x00, 0x00, 0x00, tab);
		tabSend(pilotageLED(40,0xE0, 0x00, 0x00, 0x00, tab));
		
		osDelay(1000);
		
		pilotageLED(10,0xFF, 0xFF, 0xFF, 0xFF, tab);
		pilotageLED(20,0xFF, 0xFF, 0xFF, 0xFF, tab);
		pilotageLED(30,0xE5, 0x00, 0x00, 0xFF, tab);
		tabSend(pilotageLED(40,0xE5, 0x00, 0x00, 0xFF, tab));
		osDelay(1500);
		
		pilotageLED(12,0xE1, 0xFF, 0xFF, 0xFF, tab);
		pilotageLED(18,0xE1, 0xFF, 0xFF, 0xFF, tab);
		pilotageLED(11,0xE1, 0xFF, 0xFF, 0xFF, tab);
		pilotageLED(19,0xE1, 0xFF, 0xFF, 0xFF, tab);
		pilotageLED(10,0xE1, 0xFF, 0xFF, 0xFF, tab);
		tabSend(pilotageLED(20,0xE1, 0xFF, 0xFF, 0xFF, tab));
		osSignalSet(ID_SIMU,0x0001);//Annonce fin
		
	}
}

void Clignotant(void const * argument)
{
	while(1)
	{
		osSignalWait(0x0001, osWaitForever);//attente activation par une autre fonction
		
		pilotageLED(10,0xFF, 0x00, 0x80, 0xFF, tab);
		pilotageLED(20,0xFF, 0x00, 0x80, 0xFF, tab);
		pilotageLED(30,0xFF, 0x00, 0x80, 0xFF, tab);
		tabSend(pilotageLED(40,0xFF, 0x00, 0x80, 0xFF, tab));
		osDelay(200);
		
		pilotageLED(11,0xFF, 0x00, 0x80, 0xFF, tab);
		pilotageLED(19,0xFF, 0x00, 0x80, 0xFF, tab);
		pilotageLED(31,0xFF, 0x00, 0x80, 0xFF, tab);
		tabSend(pilotageLED(39,0xFF, 0x00, 0x80, 0xFF, tab));
		osDelay(200);
		
		pilotageLED(12,0xFF, 0x00, 0x80, 0xFF, tab);
		pilotageLED(18,0xFF, 0x00, 0x80, 0xFF, tab);
		pilotageLED(32,0xFF, 0x00, 0x80, 0xFF, tab);
		tabSend(pilotageLED(38,0xFF, 0x00, 0x80, 0xFF, tab));
		osDelay(300);

		pilotageLED(12,0xE1, 0xFF, 0xFF, 0xFF, tab);
		pilotageLED(18,0xE1, 0xFF, 0xFF, 0xFF, tab);
		pilotageLED(11,0xE1, 0xFF, 0xFF, 0xFF, tab);
		pilotageLED(19,0xE1, 0xFF, 0xFF, 0xFF, tab);
		pilotageLED(10,0xE1, 0xFF, 0xFF, 0xFF, tab);
		pilotageLED(20,0xE1, 0xFF, 0xFF, 0xFF, tab);
		pilotageLED(30,0xE5, 0x00, 0x00, 0xFF, tab);
		pilotageLED(40,0xE5, 0x00, 0x00, 0xFF, tab);
		pilotageLED(31,0xE5, 0x00, 0x00, 0xFF, tab);
		pilotageLED(39,0xE5, 0x00, 0x00, 0xFF, tab);
		pilotageLED(32,0xE5, 0x00, 0x00, 0xFF, tab);
		tabSend(pilotageLED(38,0xE5, 0x00, 0x00, 0xFF, tab));
		osDelay(300);

		osSignalSet(ID_SIMU,0x0002);
		}
}

void ClignotantD(void const * argument)
{
	while(1)
	{
		osSignalWait(0x0001, osWaitForever);//attente activation par une autre fonction
		pilotageLED(12,0xFF, 0x00, 0x80, 0xFF, tab);
		pilotageLED(20,0xE1, 0xFF, 0xFF, 0xFF, tab);
		pilotageLED(19,0xE1, 0xFF, 0xFF, 0xFF, tab);
		pilotageLED(18,0xE1, 0xFF, 0xFF, 0xFF, tab);
		pilotageLED(30,0xE5, 0x00, 0x00, 0xFF, tab);
		pilotageLED(31,0xE5, 0x00, 0x00, 0xFF, tab);
		pilotageLED(32,0xE5, 0x00, 0x00, 0xFF, tab);
		tabSend(pilotageLED(38,0xFF, 0x00, 0x80, 0xFF, tab));
		osDelay(200);
		
		pilotageLED(11,0xFF, 0x00, 0x80, 0xFF, tab);		
		tabSend(pilotageLED(39,0xFF, 0x00, 0x80, 0xFF, tab));
		osDelay(200);
		
		pilotageLED(10,0xFF, 0x00, 0x80, 0xFF, tab);		
		tabSend(pilotageLED(40,0xFF, 0x00, 0x80, 0xFF, tab));
		osDelay(300);
		
		pilotageLED(12,0xE1, 0xFF, 0xFF, 0xFF, tab);
		pilotageLED(11,0xE1, 0xFF, 0xFF, 0xFF, tab);
		pilotageLED(10,0xE1, 0xFF, 0xFF, 0xFF, tab);
		pilotageLED(40,0xE5, 0x00, 0x00, 0xFF, tab);
		pilotageLED(39,0xE5, 0x00, 0x00, 0xFF, tab);
		tabSend(pilotageLED(38,0xE5, 0x00, 0x00, 0xFF, tab));
		osDelay(300);

		osSignalSet(ID_SIMU,0x0010);
		}
}

void ClignotantG(void const * argument)
{
	while(1)
	{
		osSignalWait(0x0001, osWaitForever);//attente activation par une autre fonction
		pilotageLED(18,0xFF, 0x00, 0x80, 0xFF, tab);
		pilotageLED(10,0xE1, 0xFF, 0xFF, 0xFF, tab);
		pilotageLED(11,0xE1, 0xFF, 0xFF, 0xFF, tab);
		pilotageLED(12,0xE1, 0xFF, 0xFF, 0xFF, tab);
		pilotageLED(40,0xE5, 0x00, 0x00, 0xFF, tab);
		pilotageLED(39,0xE5, 0x00, 0x00, 0xFF, tab);
		pilotageLED(38,0xE5, 0x00, 0x00, 0xFF, tab);
		tabSend(pilotageLED(32,0xFF, 0x00, 0x80, 0xFF, tab));
		osDelay(200);
		
		pilotageLED(19,0xFF, 0x00, 0x80, 0xFF, tab);		
		tabSend(pilotageLED(31,0xFF, 0x00, 0x80, 0xFF, tab));
		osDelay(200);
		
		pilotageLED(20,0xFF, 0x00, 0x80, 0xFF, tab);		
		tabSend(pilotageLED(30,0xFF, 0x00, 0x80, 0xFF, tab));
		osDelay(300);
		
		pilotageLED(18,0xE1, 0xFF, 0xFF, 0xFF, tab);
		pilotageLED(19,0xE1, 0xFF, 0xFF, 0xFF, tab);
		pilotageLED(20,0xE1, 0xFF, 0xFF, 0xFF, tab);
		pilotageLED(30,0xE5, 0x00, 0x00, 0xFF, tab);
		pilotageLED(31,0xE5, 0x00, 0x00, 0xFF, tab);
		tabSend(pilotageLED(32,0xE5, 0x00, 0x00, 0xFF, tab));
		osDelay(300);

		osSignalSet(ID_SIMU,0x0020);
		}
}



void Freinage(void const * argument)
{
	while(1)
	{
		osSignalWait(0x0001, osWaitForever);//attente début freinage
		pilotageLED(30,0xFF, 0x00, 0x00, 0xFF, tab);
		pilotageLED(40,0xFF, 0x00, 0x00, 0xFF, tab);
		pilotageLED(31,0xFF, 0x00, 0x00, 0xFF, tab);
		pilotageLED(39,0xFF, 0x00, 0x00, 0xFF, tab);
		pilotageLED(32,0xFF, 0x00, 0x00, 0xFF, tab);
		pilotageLED(38,0xFF, 0x00, 0x00, 0xFF, tab);
		
		pilotageLED(12,0xE1, 0xFF, 0xFF, 0xFF, tab);
		pilotageLED(18,0xE1, 0xFF, 0xFF, 0xFF, tab);
		pilotageLED(11,0xE1, 0xFF, 0xFF, 0xFF, tab);
		pilotageLED(19,0xE1, 0xFF, 0xFF, 0xFF, tab);
		pilotageLED(10,0xE1, 0xFF, 0xFF, 0xFF, tab);
		tabSend(pilotageLED(20,0xE1, 0xFF, 0xFF, 0xFF, tab));
		//osDelay(1000);
		osSignalWait(0x0002, osWaitForever);//fin freinage remise des phares arrières  
		pilotageLED(30,0xE5, 0x00, 0x00, 0xFF, tab);
		pilotageLED(40,0xE5, 0x00, 0x00, 0xFF, tab);
		pilotageLED(31,0xE5, 0x00, 0x00, 0xFF, tab);
		pilotageLED(39,0xE5, 0x00, 0x00, 0xFF, tab);
		pilotageLED(32,0xE5, 0x00, 0x00, 0xFF, tab);
		tabSend(pilotageLED(38,0xE5, 0x00, 0x00, 0xFF, tab));
//		osDelay(1000);
		osSignalSet(ID_SIMU,0x0004);
	}
	
}

void Recul(void const * argument)
{
	while(1)
	{
		osSignalWait(0x0001,osWaitForever);//attente activation par une autre tache

		pilotageLED(30,0xE5, 0x00, 0x00, 0xFF, tab);
		pilotageLED(40,0xE5, 0x00, 0x00, 0xFF, tab);
		pilotageLED(31,0xE5, 0x00, 0x00, 0xFF, tab);
		pilotageLED(39,0xE5, 0x00, 0x00, 0xFF, tab);
		pilotageLED(32,0xE5, 0xFF, 0xFF, 0xFF, tab);
		pilotageLED(38,0xE5, 0xFF, 0xFF, 0xFF, tab);
		
		
		pilotageLED(12,0xE1, 0xFF, 0xFF, 0xFF, tab);
		pilotageLED(18,0xE1, 0xFF, 0xFF, 0xFF, tab);
		pilotageLED(11,0xE1, 0xFF, 0xFF, 0xFF, tab);
		pilotageLED(19,0xE1, 0xFF, 0xFF, 0xFF, tab);
		pilotageLED(10,0xE1, 0xFF, 0xFF, 0xFF, tab);
		tabSend(pilotageLED(20,0xE1, 0xFF, 0xFF, 0xFF, tab));
		
		osSignalWait(0x0002,osWaitForever);//fin recul
		pilotageLED(30,0xE5, 0x00, 0x00, 0xFF, tab);
		pilotageLED(40,0xE5, 0x00, 0x00, 0xFF, tab);
		pilotageLED(31,0xE5, 0x00, 0x00, 0xFF, tab);
		pilotageLED(39,0xE5, 0x00, 0x00, 0xFF, tab);
		pilotageLED(32,0xE5, 0x00, 0x00, 0xFF, tab);
		tabSend(pilotageLED(38,0xE5, 0x00, 0x00, 0xFF, tab));
		
		//osDelay(1000);
		osSignalSet(ID_SIMU,0x0008);
	}
}

void Simu(void const * argument)
{
	while(1)
	{
		//osSignalWait(0x1000,osWaitForever);
		osSignalSet(ID_ALLUMAGE,0x0001);
		osSignalWait(0x0001,osWaitForever);
		osDelay(1000);
		osSignalSet(ID_CLIGNOTANT,0x0001);
		osSignalWait(0x0002,osWaitForever);
		osSignalSet(ID_CLIGNOTANT,0x0001);
		osSignalWait(0x0002,osWaitForever);
		osSignalSet(ID_CLIGNOTANT,0x0001);
		osSignalWait(0x0002,osWaitForever);
		osDelay(2000);
		osSignalSet(ID_FREINAGE,0x0001);
		osDelay(2000);
		osSignalSet(ID_FREINAGE,0x0002);
		osSignalWait(0x0004,osWaitForever);
		osDelay(2000);
		osSignalSet(ID_RECUL,0x0001);
		osDelay(2000);
		osSignalSet(ID_RECUL,0x0002);
		osDelay(2000);
		osSignalWait(0x0008,osWaitForever);
		osDelay(2000);
		osSignalSet(ID_CLIGNOTANTD,0x0001);
		osSignalWait(0x0010,osWaitForever);
		osSignalSet(ID_CLIGNOTANTD,0x0001);
		osSignalWait(0x0010,osWaitForever);
		osSignalSet(ID_CLIGNOTANTD,0x0001);
		osSignalWait(0x0010,osWaitForever);
		osDelay(3000);
		osSignalSet(ID_CLIGNOTANTG,0x0001);
		osSignalWait(0x0020,osWaitForever);
		osSignalSet(ID_CLIGNOTANTG,0x0001);
		osSignalWait(0x0020,osWaitForever);
		osSignalSet(ID_CLIGNOTANTG,0x0001);
		osSignalWait(0x0020,osWaitForever);
		osDelay(2000);
		osSignalSet(ID_APPELPHARE,0x0001);
		osSignalWait(0x0040,osWaitForever);
		osDelay(2000);
	}
}
	

void Arc(void const * argument)
{
	char tab[248];
	short val=0;
	short lav;
	short vla=127;
	int tour=0;
	pilotageLED(0,0xE0, 0x00, 0x00, 0x00, tab);
	while(1)
	{
		switch (tour){
		
		case 0:
			
				if (val == 0xFF)
				{
					val =0;
					tour =2;
				}
				else
				{
					val+=1;
					osDelay(10);
				}
				break;
				
		case 1:
			
				if (vla == 0xFF)
				{
					vla =0;
					tour = 0;
				}
				else
				{
					vla+=1;
					osDelay(10);
				}
		case 2:
				if (lav == 0xFF)
				{
					lav =0;
					tour = 1;
				}
				else
				{
					lav+=1;
					osDelay(10);
				}
			}
				
		tabSend(pilotageLED(0,0xFF,val,0x00,0x00,tab));
	}	
}
void AppelPhare(void const * argument)
{
	while(1)
	{
		osSignalWait(0x0001,osWaitForever);
		
		pilotageLED(30,0xE5, 0x00, 0x00, 0xFF, tab);
		pilotageLED(40,0xE5, 0x00, 0x00, 0xFF, tab);
		pilotageLED(31,0xE5, 0x00, 0x00, 0xFF, tab);
		pilotageLED(39,0xE5, 0x00, 0x00, 0xFF, tab);
		pilotageLED(32,0xE5, 0x00, 0x00, 0xFF, tab);
		pilotageLED(38,0xE5, 0x00, 0x00, 0xFF, tab);
		
		pilotageLED(12,0xE1, 0xFF, 0xFF, 0xFF, tab);
		pilotageLED(18,0xE1, 0xFF, 0xFF, 0xFF, tab);
		pilotageLED(11,0xE1, 0xFF, 0xFF, 0xFF, tab);
		pilotageLED(19,0xE1, 0xFF, 0xFF, 0xFF, tab);
		pilotageLED(10,0xE1, 0xFF, 0xFF, 0xFF, tab);
		tabSend(pilotageLED(20,0xE1, 0xFF, 0xFF, 0xFF, tab));
		osDelay(50);
		
		pilotageLED(12,0xFF, 0xFF, 0xFF, 0xFF, tab);
		pilotageLED(18,0xFF, 0xFF, 0xFF, 0xFF, tab);
		pilotageLED(11,0xFF, 0xFF, 0xFF, 0xFF, tab);
		pilotageLED(19,0xFF, 0xFF, 0xFF, 0xFF, tab);
		pilotageLED(10,0xFF, 0xFF, 0xFF, 0xFF, tab);
		tabSend(pilotageLED(20,0xFF, 0xFF, 0xFF, 0xFF, tab));
		osDelay(50);
		
		pilotageLED(12,0xE1, 0xFF, 0xFF, 0xFF, tab);
		pilotageLED(18,0xE1, 0xFF, 0xFF, 0xFF, tab);
		pilotageLED(11,0xE1, 0xFF, 0xFF, 0xFF, tab);
		pilotageLED(19,0xE1, 0xFF, 0xFF, 0xFF, tab);
		pilotageLED(10,0xE1, 0xFF, 0xFF, 0xFF, tab);
		tabSend(pilotageLED(20,0xE1, 0xFF, 0xFF, 0xFF, tab));
		osDelay(50);
		
		pilotageLED(12,0xFF, 0xFF, 0xFF, 0xFF, tab);
		pilotageLED(18,0xFF, 0xFF, 0xFF, 0xFF, tab);
		pilotageLED(11,0xFF, 0xFF, 0xFF, 0xFF, tab);
		pilotageLED(19,0xFF, 0xFF, 0xFF, 0xFF, tab);
		pilotageLED(10,0xFF, 0xFF, 0xFF, 0xFF, tab);
		tabSend(pilotageLED(20,0xFF, 0xFF, 0xFF, 0xFF, tab));
		osDelay(50);
		
		pilotageLED(12,0xE1, 0xFF, 0xFF, 0xFF, tab);
		pilotageLED(18,0xE1, 0xFF, 0xFF, 0xFF, tab);
		pilotageLED(11,0xE1, 0xFF, 0xFF, 0xFF, tab);
		pilotageLED(19,0xE1, 0xFF, 0xFF, 0xFF, tab);
		pilotageLED(10,0xE1, 0xFF, 0xFF, 0xFF, tab);
		tabSend(pilotageLED(20,0xE1, 0xFF, 0xFF, 0xFF, tab));
		osDelay(50);
		osSignalSet(ID_SIMU,0x0040);
	}
}

void CAN(void const * argument)
{
	char texte[10];
	int ID, data, taille;
	osEvent result;
	while(1)
	{
		result = osSignalWait(0x0001,5000);
		if (result.status == osEventTimeout){
			osSignalSet(ID_CLIGNOTANT,0x0001);
			osDelay(100);
		}
		else
			{
			ID = rx_msg_info.id;
			data = data_buf[0];
			taille = rx_msg_info.dlc;
			tabSend(pilotageLED(0,0xFF, 0xFF, 0x00, 0xFF, tab));
			if (data == 10)
			{
				osSignalSet(ID_ALLUMAGE,0x0001);
			}
			osDelay(10);
			}
	}	
}

