#include "stm32f4xx_hal.h"
#include "cmsis_os.h" 
#include "detection_obstacle.h"
#include "Driver_I2C.h"
#include "Driver_CAN.h"

extern ARM_DRIVER_CAN Driver_CAN1;
extern osThreadId ID_US;
extern osThreadId ID_PARKING;

osThreadId ID_writeCAN;
osThreadId ID_readCAN;

uint8_t mesure(uint8_t capteur);
//void initialisation_CAN(void);
void initialisation_GPIO(void);

void tache_US(void const *arg);
void tache_PARKING(void const *arg);
void tache_writeCAN(void const *arg);
//void tache_readCAN(void const *arg);

osMailQId ID_BAL;
osMailQDef(BAL, 10, mailBox);

GPIO_InitTypeDef GPIOC_4;
GPIO_InitTypeDef GPIOC_5;

void ARM_CAN_CallbackEvent(uint32_t obj_idx, uint32_t event){
	if(event & ARM_CAN_EVENT_SEND_COMPLETE){
		osSignalSet(ID_writeCAN, CAN_EVENT);
	}
	if(event & ARM_CAN_EVENT_RECEIVE){
		osSignalSet(ID_readCAN, CAN_EVENT);
	}
}

void tache_US(void const *arg){	
	mailBox *envoi;
	uint8_t cotes;
	while(1){
		envoi = osMailAlloc(ID_BAL,osWaitForever);
		mesureUS_RTOSBis(envoi);
		cotes = envoi->gauche & envoi->droite;
		if((cotes < 0x19)||(envoi->centre < 0x19)){
			envoi->alerte = 0x01;
		}
		if((cotes < 0x19)&&(envoi->centre < 0x19)){
			envoi->alerte = 0x02;
		}
	  osMailPut(ID_BAL, envoi);
	}
}

void tache_PARKING(void const *arg){
	mailBox *envoiPark;
	uint8_t mesureGauche, mesureDroite, mesureCentre;
	while(1){
		osSignalWait(US_CAN_EVENT, osWaitForever);
		envoiPark = osMailAlloc(ID_BAL,osWaitForever);
		envoiPark->alerte = 0x00;
		mesureUS_RTOSBis(envoiPark);
		osMailPut(ID_BAL, envoiPark);
	}
}

void tache_writeCAN(void const *arg){
	ARM_CAN_MSG_INFO trame_envoi;
	osEvent eventBAL;
	mailBox *reception;
	uint8_t envoiCAN[4];
	
	trame_envoi.id = ARM_CAN_STANDARD_ID(ID_TRAME_US);
	trame_envoi.rtr = 0;

	while(1) {
		eventBAL = osMailGet(ID_BAL, osWaitForever);
		envoiCAN[0] = reception->alerte; //envoi du type d'alerte
		envoiCAN[1] = reception->gauche; //envoi de la valeur du capteur gauche
		envoiCAN[2] = reception->centre; //envoi de la valeur du capteur central
		envoiCAN[3] = reception->droite; //envoi de la valeur du capteur droite
		Driver_CAN1.MessageSend(1, &trame_envoi, envoiCAN, 4);
		osSignalWait(CAN_EVENT, osWaitForever);
		osMailFree(ID_BAL, reception);
		osDelay(500);
	}
}

int main(void){
	//définition des priorités
	osThreadDef(tache_US, osPriorityNormal, 1, 0);
	osThreadDef(tache_PARKING, osPriorityNormal, 1, 0);
	osThreadDef(tache_writeCAN, osPriorityNormal, 1, 0);
	//osThreadDef(tache_readCAN, osPriorityNormal, 1, 0);

	osKernelInitialize();
	//cr?ation des objets
	ID_US = osThreadCreate(osThread(tache_US), NULL);
	ID_PARKING = osThreadCreate(osThread(tache_PARKING), NULL);
	ID_writeCAN = osThreadCreate(osThread(tache_writeCAN), NULL);
	//ID_readCAN = osThreadCreate(osThread(tache_readCAN), NULL);

	ID_BAL = osMailCreate(osMailQ(BAL), NULL);
	
	//initialisation_GPIO();
	configI2C_RTOS();
	
	osKernelStart();
	osSignalSet(ID_US, 0x0001);
	osDelay(osWaitForever);
	return(0);
}

void initialisation_CAN(void){
	Driver_CAN1.Initialize(NULL, ARM_CAN_CallbackEvent);
	Driver_CAN1.PowerControl(ARM_POWER_FULL);
	Driver_CAN1.SetMode(ARM_CAN_MODE_INITIALIZATION);
	Driver_CAN1.SetBitrate( ARM_CAN_BITRATE_NOMINAL, 125000,
							ARM_CAN_BIT_PROP_SEG(5U) | 
							ARM_CAN_BIT_PHASE_SEG1(1U) | 
							ARM_CAN_BIT_PHASE_SEG2(1U) | 
							ARM_CAN_BIT_SJW(1U)
	);
	
}

/*
void tache_readCAN(void const *arg){
	ARM_CAN_MSG_INFO trame_recue;
	uint32_t id_trame;
	osEvent signalCAN;
	while(1){
		signalCAN = osSignalWait(CAN_EVENT, osWaitForever);
		id_trame = trame_recue.id;
		if(id_trame == ID_TRAME_PARK){
			osSignalClear(ID_PARKING, US_CAN_EVENT);
			osSignalSet(ID_PARKING, US_CAN_EVENT);
		}
	}
}

*/
