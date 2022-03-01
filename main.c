/** ****************************************************************************************

* 
* @author Capozziello Dennis
* @date 09/10/21 
* @version 1.0 09/10/21  

*/
//------------------------------------------------------------------------------------------
//=== INCLUDES =============================================================================
//------------------------------------------------------------------------------------------
#include "FreeRTOSConfig.h"             // ARM.FreeRTOS::RTOS:Config
#include "FreeRTOS.h"   
#include "task.h"                       // ARM.FreeRTOS::RTOS:Core
//#include "queue.h"                    // ARM.FreeRTOS::RTOS:Core
#include <stdio.h>
#include "Led.h"
#include "Puls.h"
#include "clock.h"
#include "Arduino.h"
#include "event_groups.h" 
#include "infrarossi.h"
#include "eventGroupRullo.h"

EventGroupHandle_t xEventGroupRullo;

int main(void) {
		
	pllInit(0,10,0);
	SwInit();
	LedInit();
	irInit();
	xEventGroupRullo=xEventGroupCreate();
	vTaskStartScheduler();
}