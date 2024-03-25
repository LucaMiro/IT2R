#include "stm32f4xx_hal.h"
#include "Driver_SPI.h"                 // ::CMSIS Driver:SPI
#include "cmsis_os.h"                   // ARM::CMSIS:RTOS:Keil RTX
#include "Lumieres.h"
#include "Driver_CAN.h"                 // CMSIS Driver:CAN


extern ARM_DRIVER_SPI Driver_SPI1;
extern ARM_DRIVER_CAN Driver_CAN1;

ARM_CAN_MSG_INFO rx_msg_info;

uint8_t data_buf[8];

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
osThreadDef (Allumage, osPriorityHigh, 1, 0);
osThreadDef (Clignotant, osPriorityAboveNormal, 1, 0);
osThreadDef (ClignotantD, osPriorityAboveNormal, 1, 0);
osThreadDef (ClignotantG, osPriorityAboveNormal, 1, 0);
osThreadDef (Freinage, osPriorityNormal, 1, 0);
osThreadDef (AppelPhare, osPriorityNormal, 1, 0);
osThreadDef (Recul, osPriorityNormal, 1, 0);
osThreadDef (Simu, osPriorityHigh, 1, 0);
osThreadDef (Arc, osPriorityNormal, 1, 0);
osThreadDef (CAN, osPriorityHigh, 1, 0);

osMutexId ID_mut_GLCD;
osMutexDef(mut_GLCD);
/*
void CAN_callback(uint32_t obj_idx, uint32_t event)
{
	if (event & ARM_CAN_EVENT_RECEIVE)
	{
		Driver_CAN1.MessageRead(0, &rx_msg_info, data_buf,8);
		osSignalSet(ID_CAN, 0x0001);
}
}

void InitCAN(void)
{
	Driver_CAN1.Initialize(NULL,CAN_callback);
	Driver_CAN1.PowerControl(ARM_POWER_FULL);
	
	Driver_CAN1.SetMode(ARM_CAN_MODE_INITIALIZATION);
	Driver_CAN1.SetBitrate(ARM_CAN_BITRATE_NOMINAL,
											125000,
											ARM_CAN_BIT_PROP_SEG(5U)  |
											ARM_CAN_BIT_PHASE_SEG1(1U)|
											ARM_CAN_BIT_PHASE_SEG2(1U)|
											ARM_CAN_BIT_SJW(1U));
	
	Driver_CAN1.ObjectConfigure(0,ARM_CAN_OBJ_RX);
	Driver_CAN1.ObjectConfigure(2,ARM_CAN_OBJ_TX);
	Driver_CAN1.ObjectSetFilter(0, ARM_CAN_FILTER_ID_MASKABLE_ADD, ARM_CAN_STANDARD_ID(0xFFF),0xFFF);
	Driver_CAN1.SetMode(ARM_CAN_MODE_NORMAL);
}
*/
int main (void)
{
	osKernelInitialize ();                    // initialize CMSIS-RTOS

	Lum_init();
	//InitCAN();
	
	ID_mut_GLCD = osMutexCreate(osMutex(mut_GLCD));
	//ID_CAPTE = osThreadCreate ( osThread ( Capte ), NULL ) ;
	ID_ALLUMAGE = osThreadCreate ( osThread ( Allumage ), NULL ) ;
	ID_CLIGNOTANT = osThreadCreate ( osThread ( Clignotant ), NULL ) ;
	ID_CLIGNOTANTD = osThreadCreate ( osThread ( ClignotantD ), NULL ) ;
	ID_CLIGNOTANTG = osThreadCreate ( osThread ( ClignotantG ), NULL ) ;
	ID_FREINAGE = osThreadCreate ( osThread ( Freinage ), NULL ) ;
	ID_APPELPHARE = osThreadCreate ( osThread ( AppelPhare ), NULL ) ;
	ID_RECUL = osThreadCreate ( osThread ( Recul ), NULL ) ;
	ID_SIMU = osThreadCreate ( osThread ( Simu ), NULL ) ;
	//ID_ARC = osThreadCreate ( osThread ( Arc ), NULL ) ;
	//ID_CAN = osThreadCreate ( osThread ( CAN ), NULL ) ;
	
	osKernelStart ();
	osDelay(osWaitForever); 

}

void Capte (void const * argument)
{
	char tab[248];
	short val;
	pilotageLED(0,0xE0, 0x00, 0x00, 0x00, tab);
	while(1)
	{
		val = LectureADC();
		if (val >=3000)
		{
//			osMutexWait(ID_mut_GLCD,osWaitForever);
			///tabSend(pilotageLED(0,0xE1, 0xFF, 0x01, 0x80, tab));
			tabSend(pilotageLED(12,0xE1, 0xFF, 0xFF, 0xFF, tab));
			tabSend(pilotageLED(18,0xE1, 0xFF, 0xFF, 0xFF, tab));
			tabSend(pilotageLED(11,0xE1, 0xFF, 0xFF, 0xFF, tab));
			tabSend(pilotageLED(19,0xE1, 0xFF, 0xFF, 0xFF, tab));
			tabSend(pilotageLED(10,0xE1, 0xFF, 0xFF, 0xFF, tab));
			tabSend(pilotageLED(20,0xE1, 0xFF, 0xFF, 0xFF, tab));
			
			tabSend(pilotageLED(30,0xE5, 0x00, 0x00, 0xFF, tab));
			tabSend(pilotageLED(40,0xE5, 0x00, 0x00, 0xFF, tab));
			tabSend(pilotageLED(31,0xE5, 0x00, 0x00, 0xFF, tab));
			tabSend(pilotageLED(39,0xE5, 0x00, 0x00, 0xFF, tab));
			tabSend(pilotageLED(32,0xE5, 0x00, 0x00, 0xFF, tab));
			tabSend(pilotageLED(38,0xE5, 0x00, 0x00, 0xFF, tab));
//			osMutexRelease(ID_mut_GLCD);
		}
		else
		{
//			osMutexWait(ID_mut_GLCD,osWaitForever);
			//tabSend(pilotageLED(0,0xFF, 0x00, 0x40, 0xFF, tab));
			tabSend(pilotageLED(12,0xFF, 0xFF, 0xFF, 0xFF, tab));
			tabSend(pilotageLED(18,0xFF, 0xFF, 0xFF, 0xFF, tab));
			tabSend(pilotageLED(11,0xFF, 0xFF, 0xFF, 0xFF, tab));
			tabSend(pilotageLED(19,0xFF, 0xFF, 0xFF, 0xFF, tab));
			tabSend(pilotageLED(10,0xFF, 0xFF, 0xFF, 0xFF, tab));
			tabSend(pilotageLED(20,0xFF, 0xFF, 0xFF, 0xFF, tab));
			
			tabSend(pilotageLED(30,0xEB, 0x00, 0x00, 0xFF, tab));
			tabSend(pilotageLED(40,0xEB, 0x00, 0x00, 0xFF, tab));
			tabSend(pilotageLED(31,0xEB, 0x00, 0x00, 0xFF, tab));
			tabSend(pilotageLED(39,0xEB, 0x00, 0x00, 0xFF, tab));
			tabSend(pilotageLED(32,0xE5, 0x00, 0x00, 0xFF, tab));
			tabSend(pilotageLED(38,0xE5, 0x00, 0x00, 0xFF, tab));
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
		osSignalWait(0x0001, osWaitForever); // attente activation par le RFID (la c'est le simu)
		
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
		
		tabSend(pilotageLED(12,0xE1, 0xFF, 0xFF, 0xFF, tab));
		tabSend(pilotageLED(18,0xE1, 0xFF, 0xFF, 0xFF, tab));
		tabSend(pilotageLED(11,0xE1, 0xFF, 0xFF, 0xFF, tab));
		tabSend(pilotageLED(19,0xE1, 0xFF, 0xFF, 0xFF, tab));
		tabSend(pilotageLED(10,0xE1, 0xFF, 0xFF, 0xFF, tab));
		tabSend(pilotageLED(20,0xE1, 0xFF, 0xFF, 0xFF, tab));
		osSignalSet(ID_SIMU,0x0001);//Annonce fin
		//osDelay(osWaitForever);
		
	}
}

void Clignotant(void const * argument)
{
	char tab[248];
	pilotageLED(0,0xE0, 0x00, 0x00, 0x00, tab);
	while(1)
	{
		osSignalWait(0x0001, osWaitForever);//attente activation par une autre fonction
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

		tabSend(pilotageLED(12,0xE1, 0xFF, 0xFF, 0xFF, tab));
		tabSend(pilotageLED(18,0xE1, 0xFF, 0xFF, 0xFF, tab));
		tabSend(pilotageLED(11,0xE1, 0xFF, 0xFF, 0xFF, tab));
		tabSend(pilotageLED(19,0xE1, 0xFF, 0xFF, 0xFF, tab));
		tabSend(pilotageLED(10,0xE1, 0xFF, 0xFF, 0xFF, tab));
		tabSend(pilotageLED(20,0xE1, 0xFF, 0xFF, 0xFF, tab));
		tabSend(pilotageLED(30,0xE5, 0x00, 0x00, 0xFF, tab));
		tabSend(pilotageLED(40,0xE5, 0x00, 0x00, 0xFF, tab));
		tabSend(pilotageLED(31,0xE5, 0x00, 0x00, 0xFF, tab));
		tabSend(pilotageLED(39,0xE5, 0x00, 0x00, 0xFF, tab));
		tabSend(pilotageLED(32,0xE5, 0x00, 0x00, 0xFF, tab));
		tabSend(pilotageLED(38,0xE5, 0x00, 0x00, 0xFF, tab));
		osDelay(300);

		osSignalSet(ID_SIMU,0x0002);
		}
}

void ClignotantD(void const * argument)
{
	char tab[248];
	pilotageLED(0,0xE0, 0x00, 0x00, 0x00, tab);
	while(1)
	{
		osSignalWait(0x0001, osWaitForever);//attente activation par une autre fonction
		tabSend(pilotageLED(12,0xFF, 0x00, 0x80, 0xFF, tab));
		tabSend(pilotageLED(20,0xE1, 0xFF, 0xFF, 0xFF, tab));
		tabSend(pilotageLED(19,0xE1, 0xFF, 0xFF, 0xFF, tab));
		tabSend(pilotageLED(18,0xE1, 0xFF, 0xFF, 0xFF, tab));
		tabSend(pilotageLED(30,0xE5, 0x00, 0x00, 0xFF, tab));
		tabSend(pilotageLED(31,0xE5, 0x00, 0x00, 0xFF, tab));
		tabSend(pilotageLED(32,0xE5, 0x00, 0x00, 0xFF, tab));
		tabSend(pilotageLED(38,0xFF, 0x00, 0x80, 0xFF, tab));
		osDelay(200);
		
		tabSend(pilotageLED(11,0xFF, 0x00, 0x80, 0xFF, tab));		
		tabSend(pilotageLED(39,0xFF, 0x00, 0x80, 0xFF, tab));
		osDelay(200);
		
		tabSend(pilotageLED(10,0xFF, 0x00, 0x80, 0xFF, tab));		
		tabSend(pilotageLED(40,0xFF, 0x00, 0x80, 0xFF, tab));
		osDelay(300);
		
		tabSend(pilotageLED(12,0xE1, 0xFF, 0xFF, 0xFF, tab));
		tabSend(pilotageLED(11,0xE1, 0xFF, 0xFF, 0xFF, tab));
		tabSend(pilotageLED(10,0xE1, 0xFF, 0xFF, 0xFF, tab));
		tabSend(pilotageLED(40,0xE5, 0x00, 0x00, 0xFF, tab));
		tabSend(pilotageLED(39,0xE5, 0x00, 0x00, 0xFF, tab));
		tabSend(pilotageLED(38,0xE5, 0x00, 0x00, 0xFF, tab));
		osDelay(300);

		osSignalSet(ID_SIMU,0x0010);
		}
}

void ClignotantG(void const * argument)
{
	char tab[248];
	pilotageLED(0,0xE0, 0x00, 0x00, 0x00, tab);
	while(1)
	{
		osSignalWait(0x0001, osWaitForever);//attente activation par une autre fonction
		tabSend(pilotageLED(18,0xFF, 0x00, 0x80, 0xFF, tab));
		tabSend(pilotageLED(10,0xE1, 0xFF, 0xFF, 0xFF, tab));
		tabSend(pilotageLED(11,0xE1, 0xFF, 0xFF, 0xFF, tab));
		tabSend(pilotageLED(12,0xE1, 0xFF, 0xFF, 0xFF, tab));
		tabSend(pilotageLED(40,0xE5, 0x00, 0x00, 0xFF, tab));
		tabSend(pilotageLED(39,0xE5, 0x00, 0x00, 0xFF, tab));
		tabSend(pilotageLED(38,0xE5, 0x00, 0x00, 0xFF, tab));
		tabSend(pilotageLED(32,0xFF, 0x00, 0x80, 0xFF, tab));
		osDelay(200);
		
		tabSend(pilotageLED(19,0xFF, 0x00, 0x80, 0xFF, tab));		
		tabSend(pilotageLED(31,0xFF, 0x00, 0x80, 0xFF, tab));
		osDelay(200);
		
		tabSend(pilotageLED(20,0xFF, 0x00, 0x80, 0xFF, tab));		
		tabSend(pilotageLED(30,0xFF, 0x00, 0x80, 0xFF, tab));
		osDelay(300);
		
		tabSend(pilotageLED(18,0xE1, 0xFF, 0xFF, 0xFF, tab));
		tabSend(pilotageLED(19,0xE1, 0xFF, 0xFF, 0xFF, tab));
		tabSend(pilotageLED(20,0xE1, 0xFF, 0xFF, 0xFF, tab));
		tabSend(pilotageLED(30,0xE5, 0x00, 0x00, 0xFF, tab));
		tabSend(pilotageLED(31,0xE5, 0x00, 0x00, 0xFF, tab));
		tabSend(pilotageLED(32,0xE5, 0x00, 0x00, 0xFF, tab));
		osDelay(300);

		osSignalSet(ID_SIMU,0x0020);
		}
}



void Freinage(void const * argument)
{
	char tab[248];
	pilotageLED(0,0xE0, 0x00, 0x00, 0x00, tab);
	while(1)
	{
		osSignalWait(0x0001, osWaitForever);//attente d�but freinage
		tabSend(pilotageLED(30,0xFF, 0x00, 0x00, 0xFF, tab));
		tabSend(pilotageLED(40,0xFF, 0x00, 0x00, 0xFF, tab));
		tabSend(pilotageLED(31,0xFF, 0x00, 0x00, 0xFF, tab));
		tabSend(pilotageLED(39,0xFF, 0x00, 0x00, 0xFF, tab));
		tabSend(pilotageLED(32,0xFF, 0x00, 0x00, 0xFF, tab));
		tabSend(pilotageLED(38,0xFF, 0x00, 0x00, 0xFF, tab));
		
		tabSend(pilotageLED(12,0xE1, 0xFF, 0xFF, 0xFF, tab));
		tabSend(pilotageLED(18,0xE1, 0xFF, 0xFF, 0xFF, tab));
		tabSend(pilotageLED(11,0xE1, 0xFF, 0xFF, 0xFF, tab));
		tabSend(pilotageLED(19,0xE1, 0xFF, 0xFF, 0xFF, tab));
		tabSend(pilotageLED(10,0xE1, 0xFF, 0xFF, 0xFF, tab));
		tabSend(pilotageLED(20,0xE1, 0xFF, 0xFF, 0xFF, tab));
		//osDelay(1000);
		osSignalWait(0x0002, osWaitForever);//fin freinage remise des phares arri�res  
		tabSend(pilotageLED(30,0xE5, 0x00, 0x00, 0xFF, tab));
		tabSend(pilotageLED(40,0xE5, 0x00, 0x00, 0xFF, tab));
		tabSend(pilotageLED(31,0xE5, 0x00, 0x00, 0xFF, tab));
		tabSend(pilotageLED(39,0xE5, 0x00, 0x00, 0xFF, tab));
		tabSend(pilotageLED(32,0xE5, 0x00, 0x00, 0xFF, tab));
		tabSend(pilotageLED(38,0xE5, 0x00, 0x00, 0xFF, tab));
//		osDelay(1000);
		osSignalSet(ID_SIMU,0x0004);
	}
	
}

void Recul(void const * argument)
{
	char tab[248];
	pilotageLED(0,0xE0, 0x00, 0x00, 0x00, tab);
	while(1)
	{
		osSignalWait(0x0001,osWaitForever);//attente activation par une autre tache

		tabSend(pilotageLED(30,0xE5, 0x00, 0x00, 0xFF, tab));
		tabSend(pilotageLED(40,0xE5, 0x00, 0x00, 0xFF, tab));
		tabSend(pilotageLED(31,0xE5, 0x00, 0x00, 0xFF, tab));
		tabSend(pilotageLED(39,0xE5, 0x00, 0x00, 0xFF, tab));
		tabSend(pilotageLED(32,0xE5, 0xFF, 0xFF, 0xFF, tab));
		tabSend(pilotageLED(38,0xE5, 0xFF, 0xFF, 0xFF, tab));
		
		
		tabSend(pilotageLED(12,0xE1, 0xFF, 0xFF, 0xFF, tab));
		tabSend(pilotageLED(18,0xE1, 0xFF, 0xFF, 0xFF, tab));
		tabSend(pilotageLED(11,0xE1, 0xFF, 0xFF, 0xFF, tab));
		tabSend(pilotageLED(19,0xE1, 0xFF, 0xFF, 0xFF, tab));
		tabSend(pilotageLED(10,0xE1, 0xFF, 0xFF, 0xFF, tab));
		tabSend(pilotageLED(20,0xE1, 0xFF, 0xFF, 0xFF, tab));
		
		osSignalWait(0x0002,osWaitForever);//fin recul
		tabSend(pilotageLED(30,0xE5, 0x00, 0x00, 0xFF, tab));
		tabSend(pilotageLED(40,0xE5, 0x00, 0x00, 0xFF, tab));
		tabSend(pilotageLED(31,0xE5, 0x00, 0x00, 0xFF, tab));
		tabSend(pilotageLED(39,0xE5, 0x00, 0x00, 0xFF, tab));
		tabSend(pilotageLED(32,0xE5, 0x00, 0x00, 0xFF, tab));
		tabSend(pilotageLED(38,0xE5, 0x00, 0x00, 0xFF, tab));
		
		//osDelay(1000);
		osSignalSet(ID_SIMU,0x0008);
	}
}

void Simu(void const * argument)
{
	char tab[248];
	pilotageLED(0,0xE0, 0x00, 0x00, 0x00, tab);
	
	while(1)
	{
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
		osDelay(5000);
	
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
	char tab[248];
	pilotageLED(0,0xE0, 0x00, 0x00, 0x00, tab);
	while(1)
	{
		osSignalWait(0x0001,osWaitForever);
		
		tabSend(pilotageLED(30,0xE5, 0x00, 0x00, 0xFF, tab));
		tabSend(pilotageLED(40,0xE5, 0x00, 0x00, 0xFF, tab));
		tabSend(pilotageLED(31,0xE5, 0x00, 0x00, 0xFF, tab));
		tabSend(pilotageLED(39,0xE5, 0x00, 0x00, 0xFF, tab));
		tabSend(pilotageLED(32,0xE5, 0x00, 0x00, 0xFF, tab));
		tabSend(pilotageLED(38,0xE5, 0x00, 0x00, 0xFF, tab));
		
		tabSend(pilotageLED(12,0xE1, 0xFF, 0xFF, 0xFF, tab));
		tabSend(pilotageLED(18,0xE1, 0xFF, 0xFF, 0xFF, tab));
		tabSend(pilotageLED(11,0xE1, 0xFF, 0xFF, 0xFF, tab));
		tabSend(pilotageLED(19,0xE1, 0xFF, 0xFF, 0xFF, tab));
		tabSend(pilotageLED(10,0xE1, 0xFF, 0xFF, 0xFF, tab));
		tabSend(pilotageLED(20,0xE1, 0xFF, 0xFF, 0xFF, tab));
		osDelay(50);
		
		tabSend(pilotageLED(12,0xFF, 0xFF, 0xFF, 0xFF, tab));
		tabSend(pilotageLED(18,0xFF, 0xFF, 0xFF, 0xFF, tab));
		tabSend(pilotageLED(11,0xFF, 0xFF, 0xFF, 0xFF, tab));
		tabSend(pilotageLED(19,0xFF, 0xFF, 0xFF, 0xFF, tab));
		tabSend(pilotageLED(10,0xFF, 0xFF, 0xFF, 0xFF, tab));
		tabSend(pilotageLED(20,0xFF, 0xFF, 0xFF, 0xFF, tab));
		osDelay(50);
		
		tabSend(pilotageLED(12,0xE1, 0xFF, 0xFF, 0xFF, tab));
		tabSend(pilotageLED(18,0xE1, 0xFF, 0xFF, 0xFF, tab));
		tabSend(pilotageLED(11,0xE1, 0xFF, 0xFF, 0xFF, tab));
		tabSend(pilotageLED(19,0xE1, 0xFF, 0xFF, 0xFF, tab));
		tabSend(pilotageLED(10,0xE1, 0xFF, 0xFF, 0xFF, tab));
		tabSend(pilotageLED(20,0xE1, 0xFF, 0xFF, 0xFF, tab));
		osDelay(50);
		
		tabSend(pilotageLED(12,0xFF, 0xFF, 0xFF, 0xFF, tab));
		tabSend(pilotageLED(18,0xFF, 0xFF, 0xFF, 0xFF, tab));
		tabSend(pilotageLED(11,0xFF, 0xFF, 0xFF, 0xFF, tab));
		tabSend(pilotageLED(19,0xFF, 0xFF, 0xFF, 0xFF, tab));
		tabSend(pilotageLED(10,0xFF, 0xFF, 0xFF, 0xFF, tab));
		tabSend(pilotageLED(20,0xFF, 0xFF, 0xFF, 0xFF, tab));
		osDelay(50);
		
		tabSend(pilotageLED(12,0xE1, 0xFF, 0xFF, 0xFF, tab));
		tabSend(pilotageLED(18,0xE1, 0xFF, 0xFF, 0xFF, tab));
		tabSend(pilotageLED(11,0xE1, 0xFF, 0xFF, 0xFF, tab));
		tabSend(pilotageLED(19,0xE1, 0xFF, 0xFF, 0xFF, tab));
		tabSend(pilotageLED(10,0xE1, 0xFF, 0xFF, 0xFF, tab));
		tabSend(pilotageLED(20,0xE1, 0xFF, 0xFF, 0xFF, tab));
		osDelay(50);
		osSignalSet(ID_SIMU,0x0040);
	}
}

void CAN(void const * argument)
{
	char texte[10];
	int ID, data;
	while(1)
	{
		osSignalWait(0x0001,osWaitForever);
		ID = rx_msg_info.id;
		data = data_buf[0];
		//taille = rx_msg_info.dlc;
		
	}
	
}