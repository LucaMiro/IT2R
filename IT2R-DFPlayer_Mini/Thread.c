
#include "cmsis_os.h"                                           // CMSIS RTOS header file
#include "RTE_Device.h"
#include "lib_DFPlayer.h"

/*----------------------------------------------------------------------------
 *      Thread 1 'Thread_Name': Sample Thread
 *---------------------------------------------------------------------------*/
 
void Thread1 (void const *argument);                             // Thread function
osThreadId tid_Thread1;                                          // Thread id
osThreadDef (Thread1, osPriorityNormal, 1, 0);                   // Thread object

int main (void) {

  tid_Thread1 = osThreadCreate (osThread(Thread1), NULL);
  if (!tid_Thread1) return(-1);
  
  return(0);
}

void Thread1 (void const *argument) {

			init_UART2();
	
  while (1) {
		
			choisir_music(doc_ultrason,son_alerteGeneral);
			osDelay(6000);
    ; // Insert Thread code here...
    osThreadYield ();                                           // suspend Thread
  }
}
