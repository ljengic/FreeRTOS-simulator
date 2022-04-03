#include <stdio.h>
#include <pthread.h>

/* Kernel includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "timers.h"
#include "semphr.h"

/* Local includes. */
#include "console.h"
#include "taskSetGenerator.h"

/* Priorities at which the tasks are created. */
#define mainTASK_PRIORITY					 ( tskIDLE_PRIORITY + 1 )

/*-----------------------------------------------------------*/

/*
 * The tasks as described in the comments at the top of this file.
 */
static void prvTask( void * pvParameters );

TaskHandle_t handler[MAX_TASK_CNT];

/*-----------------------------------------------------------*/

/*** SEE THE COMMENTS AT THE TOP OF THIS FILE ***/
void  main_demo_periodic (void)
{

	 console_print( "usao sam u main \n" );



	 for(int i=0;i<getTaskCnt();i++){
		 xTaskCreatePeriodic( prvTask,             /* The function that implements the task. */
				 	 	 	 getTaskName(i),                            /* The text name assigned to the task - for debug only as it is not used by the kernel. */
		 					 configMINIMAL_STACK_SIZE,        /* The size of the stack to allocate to the task. */
		 					 NULL,                            /* The parameter passed to the task - not used in this simple case. */
		 					 mainTASK_PRIORITY, /* The priority assigned to the task. */
		 					 handler[i],						/* The task handle is not required, so NULL is passed. */
							 getTaskPeriod(i) ,								/*period*/
							 getTaskDuration(i));                          /* duration */


		 printInfo(i);
	 }

	 /*
	 TaskCreatePeriodic( prvTask,
					 "PeriodicTasks-Test",
					 configMINIMAL_STACK_SIZE,
					 NULL,
					 mainTASK_PRIORITY,
					 handler_na_task,
					 5000 / portTICK_RATE_MS,
					 1000 / portTICK_RATE_MS);
	*/
	 console_print( "prosao sam taskCreatePeriodic \n" );


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

	  TickType_t start,elapsed;

	  for( ; ; )
	  {

		  start = xTaskGetTickCount();

		  console_print( "Task is started\n");

		  while(1){

			  //console_print( "Some work...\n");

			  elapsed = xTaskGetTickCount() - start;

			  if(elapsed >= xTaskDurationGet(handler[0])) break;

		  }

		  console_print( "Task is bloked\n");

		  //console_print( "Task period is %d\n",xTaskDurationGet(handler_na_task) );
		  //UBaseType_t prioritet = uxTaskPriorityGet(handler_na_task);

		  elapsed = xTaskGetTickCount() - start;

		  vTaskDelay(xTaskPeriodGet(handler[0])-elapsed);
	  }

}
