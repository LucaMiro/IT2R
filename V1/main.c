#include "stm32f4xx_hal.h"
#include "Driver_SPI.h"                 // ::CMSIS Driver:SPI
#include "cmsis_os.h"                   // ARM::CMSIS:RTOS:Keil RTX
#include "Lumieres.h"


extern ARM_DRIVER_SPI Driver_SPI1;

void Capte (void const * argument);
void Allumage(void const * argument);
void Clignotant(void const * argument);
void Freinage(void const * argument);
void Recul(void const * argument);

osThreadId  ID_CAPTE;
osThreadId  ID_ALLUMAGE;
osThreadId  ID_CLIGNOTANT;
osThreadId  ID_FREINAGE;
osThreadId  ID_RECUL;

osThreadDef (Capte, osPriorityNormal, 1, 0);
osThreadDef (Allumage, osPriorityNormal, 1, 0);
osThreadDef (Clignotant, osPriorityNormal, 1, 0);
osThreadDef (Freinage, osPriorityNormal, 1, 0);
osThreadDef (Recul, osPriorityNormal, 1, 0);

osMutexId ID_mut_GLCD;
osMutexDef(mut_GLCD);

int main (void)
{
	osKernelInitialize ();                    // initialize CMSIS-RTOS

	Lum_init();
	
	ID_mut_GLCD = osMutexCreate(osMutex(mut_GLCD));
	//ID_CAPTE = osThreadCreate ( osThread ( Capte ), NULL ) ;
	ID_ALLUMAGE = osThreadCreate ( osThread ( Allumage ), NULL ) ;
	//ID_CLIGNOTANT = osThreadCreate ( osThread ( Clignotant ), NULL ) ;
	//ID_FREINAGE = osThreadCreate ( osThread ( Freinage ), NULL ) ;
	//ID_RECUL = osThreadCreate ( osThread ( Recul ), NULL ) ;
	
	osKernelStart ();
	osDelay(osWaitForever); 

}

void Capte (void const * argument)
{
	char tab[248];
	short val;
	while(1)
	{
		val = LectureADC();
		if (val >=3000)
		{
//			osMutexWait(ID_mut_GLCD,osWaitForever);
			tabSend(pilotageLED(0,0xE1, 0xFF, 0x01, 0x80, tab));
//			osMutexRelease(ID_mut_GLCD);
		}
		else
		{
//			osMutexWait(ID_mut_GLCD,osWaitForever);
			tabSend(pilotageLED(0,0xFF, 0x00, 0x40, 0xFF, tab));
//			osMutexRelease(ID_mut_GLCD);
		}
	}
}

void Allumage(void const * argument)
{
	char tab[248];
	pilotageLED(0,0xE0, 0x00, 0x00, 0x00, tab);
	while(1)
	{
		tabSend(pilotageLED(10,0xFF, 0x00, 0x80, 0xFF, tab));
		tabSend(pilotageLED(20,0xFF, 0x00, 0x80, 0xFF, tab));
		tabSend(pilotageLED(30,0xFF, 0x00, 0x80, 0xFF, tab));
		tabSend(pilotageLED(40,0xFF, 0x00, 0x80, 0xFF, tab));
		osDelay(200);
		tabSend(pilotageLED(11,0xFF, 0x00, 0x80, 0xFF, tab));
		tabSend(pilotageLED(19,0xFF, 0x00, 0x80, 0xFF, tab));
		tabSend(pilotageLED(31,0xFF, 0x00, 0x80, 0xFF, tab));
		tabSend(pilotageLED(39,0xFF, 0x00, 0x80, 0xFF, tab));
		osDelay(200);
		tabSend(pilotageLED(12,0xFF, 0x00, 0x80, 0xFF, tab));
		tabSend(pilotageLED(18,0xFF, 0x00, 0x80, 0xFF, tab));
		tabSend(pilotageLED(32,0xFF, 0x00, 0x80, 0xFF, tab));
		tabSend(pilotageLED(38,0xFF, 0x00, 0x80, 0xFF, tab));
		osDelay(300);
		tabSend(pilotageLED(10,0xE0, 0x00, 0x00, 0x00, tab));
		tabSend(pilotageLED(20,0xE0, 0x00, 0x00, 0x00, tab));
		tabSend(pilotageLED(11,0xE0, 0x00, 0x00, 0x00, tab));
		tabSend(pilotageLED(19,0xE0, 0x00, 0x00, 0x00, tab));
		tabSend(pilotageLED(12,0xE0, 0x00, 0x00, 0x00, tab));
		tabSend(pilotageLED(18,0xE0, 0x00, 0x00, 0x00, tab));
		tabSend(pilotageLED(30,0xE0, 0x00, 0x00, 0x00, tab));
		tabSend(pilotageLED(40,0xE0, 0x00, 0x00, 0x00, tab));
		tabSend(pilotageLED(31,0xE0, 0x00, 0x00, 0x00, tab));
		tabSend(pilotageLED(39,0xE0, 0x00, 0x00, 0x00, tab));
		tabSend(pilotageLED(32,0xE0, 0x00, 0x00, 0x00, tab));
		tabSend(pilotageLED(38,0xE0, 0x00, 0x00, 0x00, tab));
		osDelay(300);
		
		tabSend(pilotageLED(10,0xFF, 0x00, 0x80, 0xFF, tab));
		tabSend(pilotageLED(20,0xFF, 0x00, 0x80, 0xFF, tab));
		tabSend(pilotageLED(30,0xFF, 0x00, 0x80, 0xFF, tab));
		tabSend(pilotageLED(40,0xFF, 0x00, 0x80, 0xFF, tab));
		osDelay(200);
		tabSend(pilotageLED(11,0xFF, 0x00, 0x80, 0xFF, tab));
		tabSend(pilotageLED(19,0xFF, 0x00, 0x80, 0xFF, tab));
		tabSend(pilotageLED(31,0xFF, 0x00, 0x80, 0xFF, tab));
		tabSend(pilotageLED(39,0xFF, 0x00, 0x80, 0xFF, tab));
		osDelay(200);
		tabSend(pilotageLED(12,0xFF, 0x00, 0x80, 0xFF, tab));
		tabSend(pilotageLED(18,0xFF, 0x00, 0x80, 0xFF, tab));
		tabSend(pilotageLED(32,0xFF, 0x00, 0x80, 0xFF, tab));
		tabSend(pilotageLED(38,0xFF, 0x00, 0x80, 0xFF, tab));
		osDelay(300);
		tabSend(pilotageLED(10,0xE0, 0x00, 0x00, 0x00, tab));
		tabSend(pilotageLED(20,0xE0, 0x00, 0x00, 0x00, tab));
		tabSend(pilotageLED(11,0xE0, 0x00, 0x00, 0x00, tab));
		tabSend(pilotageLED(19,0xE0, 0x00, 0x00, 0x00, tab));
		tabSend(pilotageLED(12,0xE0, 0x00, 0x00, 0x00, tab));
		tabSend(pilotageLED(18,0xE0, 0x00, 0x00, 0x00, tab));
		tabSend(pilotageLED(30,0xE0, 0x00, 0x00, 0x00, tab));
		tabSend(pilotageLED(40,0xE0, 0x00, 0x00, 0x00, tab));
		tabSend(pilotageLED(31,0xE0, 0x00, 0x00, 0x00, tab));
		tabSend(pilotageLED(39,0xE0, 0x00, 0x00, 0x00, tab));
		tabSend(pilotageLED(32,0xE0, 0x00, 0x00, 0x00, tab));
		tabSend(pilotageLED(38,0xE0, 0x00, 0x00, 0x00, tab));
		osDelay(300);
		
		tabSend(pilotageLED(10,0xFF, 0xFF, 0xFF, 0xFF, tab));
		tabSend(pilotageLED(20,0xFF, 0xFF, 0xFF, 0xFF, tab));
		tabSend(pilotageLED(30,0xE5, 0x00, 0x00, 0xFF, tab));
		tabSend(pilotageLED(40,0xE5, 0x00, 0x00, 0xFF, tab));
		osDelay(5);
		
		tabSend(pilotageLED(11,0xFF, 0xFF, 0xFF, 0xFF, tab));
		tabSend(pilotageLED(19,0xFF, 0xFF, 0xFF, 0xFF, tab));
		tabSend(pilotageLED(31,0xE5, 0x00, 0x00, 0xFF, tab));
		tabSend(pilotageLED(39,0xE5, 0x00, 0x00, 0xFF, tab));
		
		tabSend(pilotageLED(10,0xE0, 0x00, 0x00, 0x00, tab));
		tabSend(pilotageLED(20,0xE0, 0x00, 0x00, 0x00, tab));
		tabSend(pilotageLED(30,0xE0, 0x00, 0x00, 0x00, tab));
		tabSend(pilotageLED(40,0xE0, 0x00, 0x00, 0x00, tab));
		osDelay(5);
		
		tabSend(pilotageLED(12,0xFF, 0xFF, 0xFF, 0xFF, tab));
		tabSend(pilotageLED(18,0xFF, 0xFF, 0xFF, 0xFF, tab));
		tabSend(pilotageLED(32,0xE5, 0x00, 0x00, 0xFF, tab));
		tabSend(pilotageLED(38,0xE5, 0x00, 0x00, 0xFF, tab));
		
		tabSend(pilotageLED(11,0xE0, 0x00, 0x00, 0x00, tab));
		tabSend(pilotageLED(19,0xE0, 0x00, 0x00, 0x00, tab));
		tabSend(pilotageLED(31,0xE0, 0x00, 0x00, 0x00, tab));
		tabSend(pilotageLED(39,0xE0, 0x00, 0x00, 0x00, tab));
		osDelay(1000);
		
		tabSend(pilotageLED(10,0xFF, 0xFF, 0xFF, 0xFF, tab));
		tabSend(pilotageLED(20,0xFF, 0xFF, 0xFF, 0xFF, tab));
		tabSend(pilotageLED(30,0xFF, 0x00, 0x00, 0xFF, tab));
		tabSend(pilotageLED(40,0xFF, 0x00, 0x00, 0xFF, tab));
		osDelay(5);
		
		tabSend(pilotageLED(11,0xFF, 0xFF, 0xFF, 0xFF, tab));
		tabSend(pilotageLED(19,0xFF, 0xFF, 0xFF, 0xFF, tab));
		tabSend(pilotageLED(10,0xE0, 0x00, 0x00, 0x00, tab));
		tabSend(pilotageLED(20,0xE0, 0x00, 0x00, 0x00, tab));
		tabSend(pilotageLED(31,0xE5, 0x00, 0x00, 0xFF, tab));
		tabSend(pilotageLED(39,0xE5, 0x00, 0x00, 0xFF, tab));
		tabSend(pilotageLED(30,0xE0, 0x00, 0x00, 0x00, tab));
		tabSend(pilotageLED(40,0xE0, 0x00, 0x00, 0x00, tab));
		
		osDelay(1000);
		
		tabSend(pilotageLED(10,0xFF, 0xFF, 0xFF, 0xFF, tab));
		tabSend(pilotageLED(20,0xFF, 0xFF, 0xFF, 0xFF, tab));
		tabSend(pilotageLED(30,0xE5, 0x00, 0x00, 0xFF, tab));
		tabSend(pilotageLED(40,0xE5, 0x00, 0x00, 0xFF, tab));
		osDelay(1500);
		
		tabSend(pilotageLED(12,0xE5, 0xFF, 0xFF, 0xFF, tab));
		tabSend(pilotageLED(18,0xE5, 0xFF, 0xFF, 0xFF, tab));
		tabSend(pilotageLED(11,0xE5, 0xFF, 0xFF, 0xFF, tab));
		tabSend(pilotageLED(19,0xE5, 0xFF, 0xFF, 0xFF, tab));
		tabSend(pilotageLED(10,0xE5, 0xFF, 0xFF, 0xFF, tab));
		tabSend(pilotageLED(20,0xE5, 0xFF, 0xFF, 0xFF, tab));
		osDelay(osWaitForever);
		
	}
}

void Clignotant(void const * argument)
{
	char tab[248];
	pilotageLED(0,0xE0, 0x00, 0x00, 0x00, tab);
	while(1)
	{
		tabSend(pilotageLED(10,0xFF, 0x00, 0x80, 0xFF, tab));
		tabSend(pilotageLED(20,0xFF, 0x00, 0x80, 0xFF, tab));
		tabSend(pilotageLED(30,0xFF, 0x00, 0x80, 0xFF, tab));
		tabSend(pilotageLED(40,0xFF, 0x00, 0x80, 0xFF, tab));
		osDelay(200);
		tabSend(pilotageLED(11,0xFF, 0x00, 0x80, 0xFF, tab));
		tabSend(pilotageLED(19,0xFF, 0x00, 0x80, 0xFF, tab));
		tabSend(pilotageLED(31,0xFF, 0x00, 0x80, 0xFF, tab));
		tabSend(pilotageLED(39,0xFF, 0x00, 0x80, 0xFF, tab));
		osDelay(200);
		tabSend(pilotageLED(12,0xFF, 0x00, 0x80, 0xFF, tab));
		tabSend(pilotageLED(18,0xFF, 0x00, 0x80, 0xFF, tab));
		tabSend(pilotageLED(32,0xFF, 0x00, 0x80, 0xFF, tab));
		tabSend(pilotageLED(38,0xFF, 0x00, 0x80, 0xFF, tab));
		osDelay(300);
		tabSend(pilotageLED(10,0xE0, 0x00, 0x00, 0x00, tab));
		tabSend(pilotageLED(20,0xE0, 0x00, 0x00, 0x00, tab));
		tabSend(pilotageLED(11,0xE0, 0x00, 0x00, 0x00, tab));
		tabSend(pilotageLED(19,0xE0, 0x00, 0x00, 0x00, tab));
		tabSend(pilotageLED(12,0xE0, 0x00, 0x00, 0x00, tab));
		tabSend(pilotageLED(18,0xE0, 0x00, 0x00, 0x00, tab));
		tabSend(pilotageLED(30,0xE0, 0x00, 0x00, 0x00, tab));
		tabSend(pilotageLED(40,0xE0, 0x00, 0x00, 0x00, tab));
		tabSend(pilotageLED(31,0xE0, 0x00, 0x00, 0x00, tab));
		tabSend(pilotageLED(39,0xE0, 0x00, 0x00, 0x00, tab));
		tabSend(pilotageLED(32,0xE0, 0x00, 0x00, 0x00, tab));
		tabSend(pilotageLED(38,0xE0, 0x00, 0x00, 0x00, tab));
		osDelay(300);
		}
}

void Freinage(void const * argument)
{
	char tab[248];
	pilotageLED(0,0xE0, 0x00, 0x00, 0x00, tab);
	tabSend(pilotageLED(30,0xE5, 0x00, 0x00, 0xFF, tab));
	tabSend(pilotageLED(40,0xE5, 0x00, 0x00, 0xFF, tab));
	tabSend(pilotageLED(31,0xE5, 0x00, 0x00, 0xFF, tab));
	tabSend(pilotageLED(39,0xE5, 0x00, 0x00, 0xFF, tab));
	tabSend(pilotageLED(32,0xE5, 0x00, 0x00, 0xFF, tab));
	tabSend(pilotageLED(38,0xE5, 0x00, 0x00, 0xFF, tab));
	osDelay(1000);
	
	tabSend(pilotageLED(30,0xFF, 0x00, 0x00, 0xFF, tab));
	tabSend(pilotageLED(40,0xFF, 0x00, 0x00, 0xFF, tab));
	tabSend(pilotageLED(31,0xFF, 0x00, 0x00, 0xFF, tab));
	tabSend(pilotageLED(39,0xFF, 0x00, 0x00, 0xFF, tab));
	tabSend(pilotageLED(32,0xFF, 0x00, 0x00, 0xFF, tab));
	tabSend(pilotageLED(38,0xFF, 0x00, 0x00, 0xFF, tab));
	
}

void Recul(void const * argument)
{
	char tab[248];
	pilotageLED(0,0xE0, 0x00, 0x00, 0x00, tab);
	tabSend(pilotageLED(30,0xE5, 0x00, 0x00, 0xFF, tab));
	tabSend(pilotageLED(40,0xE5, 0x00, 0x00, 0xFF, tab));
	tabSend(pilotageLED(31,0xE5, 0x00, 0x00, 0xFF, tab));
	tabSend(pilotageLED(39,0xE5, 0x00, 0x00, 0xFF, tab));
	tabSend(pilotageLED(32,0xE5, 0x00, 0x00, 0xFF, tab));
	tabSend(pilotageLED(38,0xE5, 0x00, 0x00, 0xFF, tab));
	osDelay(1000);
	
	tabSend(pilotageLED(30,0xE5, 0x00, 0x00, 0xFF, tab));
	tabSend(pilotageLED(40,0xE5, 0x00, 0x00, 0xFF, tab));
	tabSend(pilotageLED(31,0xE5, 0x00, 0x00, 0xFF, tab));
	tabSend(pilotageLED(39,0xE5, 0x00, 0x00, 0xFF, tab));
	tabSend(pilotageLED(32,0xE5, 0xFF, 0xFF, 0xFF, tab));
	tabSend(pilotageLED(38,0xE5, 0xFF, 0xFF, 0xFF, tab));
}

