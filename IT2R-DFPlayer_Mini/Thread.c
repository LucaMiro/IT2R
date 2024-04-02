
#include "cmsis_os.h"                                           // CMSIS RTOS header file
#include "RTE_Device.h"
#include "lib_DFPlayer.h"
#include "Driver_CAN.h"                 // ::CMSIS Driver:CAN

/*----------------------------------------------------------------------------
 *      Thread 1 'Thread_Name': Sample Thread
 *---------------------------------------------------------------------------*/
 
extern   ARM_DRIVER_CAN         Driver_CAN1;
extern   ARM_DRIVER_CAN         Driver_CAN2;

void Thread1 (void const *argument);  
void CANthreadR (void const *argument);
void CANthreadT (void const *argument);


// Thread function
osThreadId tid_Thread1;                                          // Thread id
osThreadDef (Thread1, osPriorityNormal, 1, 0);                   // Thread object
osThreadId id_CANthreadR;
osThreadDef (CANthreadR, osPriorityNormal, 1, 0);                   // Thread object
osThreadId id_CANthreadT;
osThreadDef (CANthreadT, osPriorityNormal, 1, 0);




// CAN1 utilisé pour réception
void myCAN1_callback(uint32_t obj_idx, uint32_t event)
{
    switch (event)
    {
    case ARM_CAN_EVENT_RECEIVE:
        /*  Message was received successfully by the obj_idx object. */
       osSignalSet(id_CANthreadR, 0x01);
        break;
    }
}

// CAN2 utilisé pour émission
void myCAN2_callback(uint32_t obj_idx, uint32_t event)
{
    switch (event)
    {
    case ARM_CAN_EVENT_SEND_COMPLETE:
        /* 	Message was sent successfully by the obj_idx object.  */
        osSignalSet(id_CANthreadT, 0x01);
        break;
    }
}







// CAN1 utilisé pour réception
void InitCan1 (void) {
	
	Driver_CAN1.Initialize(NULL,myCAN1_callback);
	Driver_CAN1.PowerControl(ARM_POWER_FULL);
	
	Driver_CAN1.SetMode(ARM_CAN_MODE_INITIALIZATION);
	Driver_CAN1.SetBitrate( ARM_CAN_BITRATE_NOMINAL,
													125000,
													ARM_CAN_BIT_PROP_SEG(5U)   |         // Set propagation segment to 5 time quanta
                          ARM_CAN_BIT_PHASE_SEG1(1U) |         // Set phase segment 1 to 1 time quantum (sample point at 87.5% of bit time)
                          ARM_CAN_BIT_PHASE_SEG2(1U) |         // Set phase segment 2 to 1 time quantum (total bit is 8 time quanta long)
                          ARM_CAN_BIT_SJW(1U));                // Resynchronization jump width is same as phase segment 2
                          
	// Mettre ici les filtres ID de réception sur objet 0
	//....................................................
	Driver_CAN1.ObjectSetFilter(0,ARM_CAN_FILTER_ID_MASKABLE_ADD, ARM_CAN_STANDARD_ID(0xFFF),0x000);
		
	Driver_CAN1.ObjectConfigure(0,ARM_CAN_OBJ_RX);				// Objet 0 du CAN1 pour réception
	
	Driver_CAN1.SetMode(ARM_CAN_MODE_NORMAL);					// fin init
	
}





// CAN2 utilisé pour émission
void InitCan2 (void) {
	Driver_CAN2.Initialize(NULL,myCAN2_callback);
	Driver_CAN2.PowerControl(ARM_POWER_FULL);
	
	Driver_CAN2.SetMode(ARM_CAN_MODE_INITIALIZATION);
	Driver_CAN2.SetBitrate( ARM_CAN_BITRATE_NOMINAL,
													125000,
													ARM_CAN_BIT_PROP_SEG(5U)   |         // Set propagation segment to 5 time quanta
                          ARM_CAN_BIT_PHASE_SEG1(1U) |         // Set phase segment 1 to 1 time quantum (sample point at 87.5% of bit time)
                          ARM_CAN_BIT_PHASE_SEG2(1U) |         // Set phase segment 2 to 1 time quantum (total bit is 8 time quanta long)
                          ARM_CAN_BIT_SJW(1U));                // Resynchronization jump width is same as phase segment 2
	
	Driver_CAN2.ObjectConfigure(1,ARM_CAN_OBJ_TX);				// Objet 1 du CAN2 pour emission
	
	Driver_CAN2.SetMode(ARM_CAN_MODE_NORMAL);					// fin init
	
}






int main (void) {

	  osKernelInitialize ();                    // initialize CMSIS-RTOS
	

	
	// Initialisation des 2 périphériques CAN
	InitCan1();

	LPC_GPIO0->FIODIR2&=0x01;
	init_UART2();
	choisir_music(doc_voiture,son_klaxon3);

  // create 'thread' functions that start executing,
  // example: tid_name = osThreadCreate (osThread(name), NULL);
	id_CANthreadR = osThreadCreate (osThread(CANthreadR), NULL);
	id_CANthreadT = osThreadCreate (osThread(CANthreadT), NULL);
  tid_Thread1 = osThreadCreate (osThread(Thread1), NULL);
  if (!tid_Thread1) return(-1);
	
  osKernelStart ();                         // start thread execution 
	osDelay(osWaitForever);

  
  return(0);
}


void CANthreadT(void const *argument)
{
	ARM_CAN_MSG_INFO                tx_msg_info;
	uint8_t data_buf[2];
	
	osDelay(100);
	while (1) {

		
		tx_msg_info.id = ARM_CAN_STANDARD_ID(0xFA1);// Code pour envoyer trame Id 0x128 
		tx_msg_info.rtr = 0;
		data_buf[0] = 254;//(LPC_GPIO0->FIOPIN2&=0xFE);
		Driver_CAN2.MessageSend(1,&tx_msg_info,data_buf,1);
		//.............

		osSignalWait(0x01, osWaitForever);		// sommeil en attente fin emission

	}		
}


// tache reception
void CANthreadR(void const *argument)
{
	ARM_CAN_MSG_INFO   rx_msg_info;
	int identifiant, retour,retour2, taille;
	uint8_t data_buf[2];
	char texte[2];
	init_UART2();
	
		osDelay(100);
	while(1)
	{		
		
		osSignalWait(0x01, osWaitForever);		// sommeil en attente réception
		Driver_CAN1.MessageRead(0,&rx_msg_info,data_buf,8);
		identifiant = rx_msg_info.id;
		retour=data_buf[0];
		retour=data_buf[1];
		taille=rx_msg_info.dlc;
		
		sprintf(texte,"%d   %d ",retour,retour2);
		
		choisir_music(retour,retour2);

		// Code pour reception trame + affichage Id et Data sur LCD
	}
}


void Thread1 (void const *argument) {

			init_UART2();
			osDelay(100);
  while (1) {
		
			choisir_music(doc_ultrason,son_alerteGeneral);
			osDelay(10);
    ; // Insert Thread code here...
    osThreadYield ();                                           // suspend Thread
  }
}


