#include "stm32f4xx_hal.h"
#include "Driver_SPI.h"                 // ::CMSIS Driver:SPI
#include "cmsis_os.h"                   // ARM::CMSIS:RTOS:Keil RTX
#include "Lumieres.h"

char* pilotageLED(char nbLED, char lumi, char bleu, char vert, char rouge, char tab[]);

extern ARM_DRIVER_SPI Driver_SPI1;

void Capte (void const * argument);

osThreadId  ID_CAPTE;

osThreadDef (Capte, osPriorityNormal, 1, 0);

osMutexId ID_mut_GLCD;
osMutexDef(mut_GLCD);

int main (void)
{
	int i=0;
	char tab[248];
	short lesho=0;
	osKernelInitialize ();                    // initialize CMSIS-RTOS

	Lum_init();
	
	ID_mut_GLCD = osMutexCreate(osMutex(mut_GLCD));
	ID_CAPTE = osThreadCreate ( osThread ( Capte ), NULL ) ;
	
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