/* Kernel includes. */
#include "sam.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "uart.h"
#include "myprintf.h"

#define wait_send 300
#define wait_recieve 100

/* Priorities at which the tasks are created. */
#define myTASK_TASK_PRIORITY         	( tskIDLE_PRIORITY + 1 )

int states = 0;

xQueueHandle Global_Queue_Handle = 0;
void sender_task_1(void *p) {
	uint32_t i = 0;
	while(1) {
		//myprintf("\nSend %d to receiver task\n", i);
		
		if( ( PORT->Group[ 0 ].IN.reg & PORT_IN_IN( PORT_PA14 ) ) == _U_( 0x00000000 ) )
		{
			i = 1;
		} else
		 i= 0;
		xQueueSend(Global_Queue_Handle,&i,100);
		
	
		
		vTaskDelay(wait_send);
	}
}

void sender_task_2(void *p) {
	uint32_t i = 0;
	while(1) {
		if( ( PORT->Group[ 0 ].IN.reg & PORT_IN_IN( PORT_PA09 ) ) == _U_( 0x00000000 ) )
		{
			i = 2;
		} else
		 i= 0;
		xQueueSend(Global_Queue_Handle,&i,100);
		//if(!xQueueSend(Global_Queue_Handle,&i,100))
			//{
				//myprintf("\nFailed to send to the Queue \n");
			//}
		
		vTaskDelay(wait_send);
	}
}

void sender_task_3(void *p) {
	uint32_t i = 0;
	while(1) {
		//myprintf("\nSend %d to receiver task\n", i);
		
		if( ( PORT->Group[ 0 ].IN.reg & PORT_IN_IN( PORT_PA08 ) ) == _U_( 0x00000000 ) )
		{
			i = 3;
		} else
		 i= 0;
		xQueueSend(Global_Queue_Handle,&i,100);
		vTaskDelay(wait_send);
	}
}

void sender_task_4(void *p) {
	uint32_t i = 0;
	while(1) {
		//myprintf("\nSend %d to receiver task\n", i);
		
		if( ( PORT->Group[ 0 ].IN.reg & PORT_IN_IN( PORT_PA15 ) ) == _U_( 0x00000000 ) )
		{
			i = 4;
		} else
		 i= 0;
		xQueueSend(Global_Queue_Handle,&i,100);
		
		
		vTaskDelay(wait_send);
	}
}


void receiver_task(void *p) {
	int rx_int=0;
	while(1) {
		if(xQueueReceive(Global_Queue_Handle, &rx_int,100))
		{
			if (rx_int>0)
			{
				myprintf("\nState on in port %d\n", rx_int);
			}
		}
		vTaskDelay(wait_recieve);
	}
}



int main()
{
	SystemInit();
	/* Switch to 8MHz clock (disable prescaler) */
	SYSCTRL->OSC8M.bit.PRESC = 0;
	initUART();

	Global_Queue_Handle = xQueueCreate(3,sizeof(int));
	
	PORT->Group[ 0 ].PINCFG[ PIN_PA16 ].reg = 0x2; //bit INEN must be set for input pins
	PORT->Group[ 0 ].PINCFG[ PIN_PA27 ].reg = 0x0; //bit PMUXEN must be clear for GPIOs

	PORT->Group[ 0 ].DIRCLR.reg = PORT_PA16; //pin 16 declared as data input
	PORT->Group[ 0 ].DIRSET.reg = PORT_PA27; //pin 27 TX LED declared as data output
	
	PORT->Group[ 0 ].PINCFG[ PIN_PA09 ].reg = 0x2; //bit INEN must be set for input pins
	PORT->Group[ 0 ].DIRCLR.reg = PORT_PA09; //pin 16 declared as data input
	PORT->Group[ 0 ].PINCFG[ PIN_PA08 ].reg = 0x2; //bit INEN must be set for input pins
	PORT->Group[ 0 ].DIRCLR.reg = PORT_PA08; //pin 16 declared as data input
	PORT->Group[ 0 ].PINCFG[ PIN_PA15 ].reg = 0x2; //bit INEN must be set for input pins
	PORT->Group[ 0 ].DIRCLR.reg = PORT_PA15; //pin 16 declared as data input
	

	xTaskCreate(sender_task_1,(signed char* )"Button 1",512,NULL,2,NULL);
	xTaskCreate(sender_task_2,(signed char* )"Button 2",512,NULL,2,NULL);
	xTaskCreate(sender_task_3,(signed char* )"Button 3",512,NULL,2,NULL);
	xTaskCreate(sender_task_4,(signed char* )"Button 4",512,NULL,2,NULL);
	
	/*xTaskCreate( my_task,        	    / The function that implements the task. 
	"my_task",              /* The text name assigned to the task. 
	512,            	    /* The size of the stack to allocate to the task. 
	NULL,           	    /* The parameter passed to the task  
	myTASK_TASK_PRIORITY,   /* The priority assigned to the task. 
	NULL );         	    /* The task handle is not required, so NULL is passed.                  */
    xTaskCreate(receiver_task,(signed char* )"Ack",512,NULL,1,NULL);
	// Start the tasks and timer running. */
	vTaskStartScheduler();

	for( ;; );
	return(0);
}