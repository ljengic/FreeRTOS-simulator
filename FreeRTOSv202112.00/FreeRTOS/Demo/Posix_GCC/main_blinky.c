#include <stdio.h>
#include <pthread.h>

/* Kernel includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "timers.h"
#include "semphr.h"

/* Local includes. */
#include "console.h"

/* Priorities at which the tasks are created. */
#define mainTASK_PRIORITY					 ( tskIDLE_PRIORITY + 1 )

/*-----------------------------------------------------------*/

/*
 * The tasks as described in the comments at the top of this file.
 */
static void prvTask( void * pvParameters );

TaskHandle_t handler_na_task;

/*-----------------------------------------------------------*/

/*** SEE THE COMMENTS AT THE TOP OF THIS FILE ***/
void  main_demo_periodic (void)
{

	 console_print( "usao sam u main \n" );

	/* Start the two tasks as described in the comments at the top of this
	 * file. */
	xTaskCreatePeriodic( prvTask,             /* The function that implements the task. */
					 "PeriodicTasks-Test",                            /* The text name assigned to the task - for debug only as it is not used by the kernel. */
					 configMINIMAL_STACK_SIZE,        /* The size of the stack to allocate to the task. */
					 NULL,                            /* The parameter passed to the task - not used in this simple case. */
					 mainTASK_PRIORITY, /* The priority assigned to the task. */
					 handler_na_task,						/* The task handle is not required, so NULL is passed. */
					 5000 / portTICK_RATE_MS,								/*period*/
					 1000 / portTICK_RATE_MS);                          /* duration */

	 //console_print( "prosao sam taskCreatePeriodic \n" );

	/* Start the tasks and timer running. */
	vTaskStartScheduler();

    /* If all is well, the scheduler will now be running, and the following
     * line will never be reached.  If the following line does execute, then
     * there was insufficient FreeRTOS heap memory available for the idle and/or
     * timer tasks	to be created.  See the memory management section on the
     * FreeRTOS web site for more details. */
    for( ; ; )
    {
    }
}
/*-----------------------------------------------------------*/


static void prvTask( void * pvParameters )
{

	//console_print( "usao sam u task \n" );

	  ( void ) pvParameters;

	  for( ; ; )
	  {
		  console_print( "Task period is %d\n",xTaskDurationGet(handler_na_task) );
		  UBaseType_t prioritet = uxTaskPriorityGet(handler_na_task);
		  console_print( "Task is running = %ld \n",prioritet);
		  vTaskDelay(1000 / portTICK_RATE_MS);
	  }

}
