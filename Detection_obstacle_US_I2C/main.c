/*---------------------------------------------------
* CAN 2 uniquement en TX 
* + réception CAN1 
* avec RTOS et utilisation des fonction CB
* pour test sur 1 carte -> relier CAN1 et CAN2
* 2017-04-02 - XM
---------------------------------------------------*/

#include "LPC17xx.h"                    // Device header
#include "Driver_CAN.h"                 // ::CMSIS Driver:CAN
#include "Board_GLCD.h"                 // ::Board Support:Graphic LCD
#include "GLCD_Config.h"                // Keil.MCB1700::Board Support:Graphic LCD
#include "stdio.h"
#include "cmsis_os.h"
#include "osObjects.h"

extern GLCD_FONT GLCD_Font_6x8;
extern GLCD_FONT GLCD_Font_16x24;

osThreadId id_CANthreadR;

extern ARM_DRIVER_CAN Driver_CAN1;


// CAN1 utilisé pour réception
void CAN1_callback(uint32_t obj_idx, uint32_t event){
    switch (event){
    case ARM_CAN_EVENT_RECEIVE:
        /*  Message was received successfully by the obj_idx object. */
       osSignalSet(id_CANthreadR, 0x01);
			break;
    }
}

// tache reception
void CANthreadR(void const *argument) {
	ARM_CAN_MSG_INFO   rx_msg_info;
	uint8_t data_buf[4];
	char capteurs[40];
	char alerte[20];
	while(1){		
		osSignalWait(0x01, osWaitForever);		// sommeil en attente réception
		Driver_CAN1.MessageRead(0, &rx_msg_info, data_buf, 4);
		sprintf(alerte, "alerte : %d", data_buf[0]);
		sprintf(capteurs, "capteurs : %d %d %d", data_buf[1], data_buf[2], data_buf[3]);
		GLCD_DrawString(10, 10, (char*)alerte);
		GLCD_DrawString(10, 40, (char*)capteurs);
		osDelay(500);
	}
}

// CAN1 utilisé pour réception
void InitCan1 (void) {
	Driver_CAN1.Initialize(NULL, CAN1_callback);
	Driver_CAN1.PowerControl(ARM_POWER_FULL);
	
	Driver_CAN1.SetMode(ARM_CAN_MODE_INITIALIZATION);
	Driver_CAN1.SetBitrate( ARM_CAN_BITRATE_NOMINAL,
													125000,
													ARM_CAN_BIT_PROP_SEG(5U)   |         // Set propagation segment to 5 time quanta
                          ARM_CAN_BIT_PHASE_SEG1(1U) |         // Set phase segment 1 to 1 time quantum (sample point at 87.5% of bit time)
                          ARM_CAN_BIT_PHASE_SEG2(1U) |         // Set phase segment 2 to 1 time quantum (total bit is 8 time quanta long)
                          ARM_CAN_BIT_SJW(1U));                // Resynchronization jump width is same as phase segment 2
                          
	// Mettre ici les filtres ID de réception sur objet 0
	Driver_CAN1.ObjectSetFilter(0, ARM_CAN_FILTER_ID_EXACT_ADD, ARM_CAN_STANDARD_ID(0x0002), 0);
	//....................................................
		
	Driver_CAN1.ObjectConfigure(0,ARM_CAN_OBJ_RX);				// Objet 0 du CAN1 pour réception
	
	Driver_CAN1.SetMode(ARM_CAN_MODE_NORMAL);					// fin init
}


osThreadDef(CANthreadR,osPriorityNormal, 1,0);

/*
 * main: initialize and start the system
 */
int main (void) {
  osKernelInitialize ();                    // initialize CMSIS-RTOS

  // initialize peripherals here
	GLCD_Initialize();
	GLCD_ClearScreen();
	GLCD_SetFont(&GLCD_Font_16x24);
	
	// Initialisation du périphérique CAN
	InitCan1();

  // create 'thread' functions that start executing,
  // example: tid_name = osThreadCreate (osThread(name), NULL);
	id_CANthreadR = osThreadCreate (osThread(CANthreadR), NULL);

  osKernelStart ();                         // start thread execution 
	osDelay(osWaitForever);
}





