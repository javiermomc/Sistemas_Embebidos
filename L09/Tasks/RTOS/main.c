/* Kernel includes. */
#include "sam.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "uart.h"
#include "myprintf.h"

/* Priorities at which the tasks are created. */
#define myTASK_TASK_PRIORITY         	( tskIDLE_PRIORITY + 1 )


xQueueHandle Global_Queue_Handle = 0;
void sender_task(void *p) {
	uint32_t i = 0;
	while(1) {
		myprintf("\nSend %d to receiver task\n", i);
		if(!xQueueSend(Global_Queue_Handle,&i,100))
		{
			myprintf("\nFailed to send to the Queue \n");
			
		}
		i++;
		
		vTaskDelay(300);
	}
}

void receiver_task(void *p) {
	int rx_int=0;
	while(1) {
		if(xQueueReceive(Global_Queue_Handle, &rx_int,100))
		{
			myprintf("\nReceived %d\n", rx_int);
		}
		else{
		myprintf("\nFailed to receive data from the Queue\n");
		}
		vTaskDelay(100);
	}
}


int main()
{
	SystemInit();
	/* Switch to 8MHz clock (disable prescaler) */
	SYSCTRL->OSC8M.bit.PRESC = 0;
	initUART();

	Global_Queue_Handle = xQueueCreate(3,sizeof(int));
	
	xTaskCreate(sender_task,(signed char* )"tx",512,NULL,1,NULL);
	xTaskCreate(receiver_task,(signed char* )"rx",512,NULL,1,NULL);
	/*xTaskCreate( my_task,        	    / The function that implements the task. 
	"my_task",              /* The text name assigned to the task. 
	512,            	    /* The size of the stack to allocate to the task. 
	NULL,           	    /* The parameter passed to the task  
	myTASK_TASK_PRIORITY,   /* The priority assigned to the task. 
	NULL );         	    /* The task handle is not required, so NULL is passed.                  */
    
	// Start the tasks and timer running. */
	vTaskStartScheduler();

	for( ;; );
	return(0);
}