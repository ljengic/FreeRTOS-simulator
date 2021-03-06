#include <stdio.h>
#include <stdlib.h>
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

/*-----------------------------------------------------------*/

/*** SEE THE COMMENTS AT THE TOP OF THIS FILE ***/
void  main_demo_periodic (void)
{

	// console_print( "usao sam u main \n" );



	setWeaklyHard();

	 for(int i=0;i<getTaskCnt();i++){
		 xTaskCreatePeriodic( prvTask,             /* The function that implements the task. */
				 	 	 	 i,  					// task identificator
				 	 	 	 getTaskName(i),                            /* The text name assigned to the task - for debug only as it is not used by the kernel. */
		 					 configMINIMAL_STACK_SIZE,        /* The size of the stack to allocate to the task. */
		 					 NULL,                            /* The parameter passed to the task - not used in this simple case. */
		 					 mainTASK_PRIORITY, /* The priority assigned to the task. */
		 					 *(getPointerToHandler(i)),						/* The task handle is not required, so NULL is passed. */
							 getTaskPeriod(i),								/*period*/
							 getTaskDuration(i),
							 getWeaklyHardConstraint(i));                          /* duration */


		printInfo(i);
		//setTaskHandler(i,handler[i]);
	 }

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

	  ( void ) pvParameters;

	  int id = xGetCurrentId();
	  int period = getTaskPeriod(id);
	  int duration = getTaskDuration(id);
	  char * name = getTaskName(id);

	  for( ; ; )
	  {

		  while(1){

		  }

	  }

}

void print_report(){

	for(int i=0;i<getTaskCnt();i++){
		bool * rep = getReport(i);
		printf("Task_%d : ",i+1);
		for(int j=0;j<getTaskNumberOfPeriods(i);j++){
			printf("%d",rep[j]);
		}
		printf("\n");
	}
	
	printf("%d\n",getWeaklyHard());

	for(int i=0;i<getTaskCnt();i++){
		printf("Weakly_hard_broken TASK_%d : %d\n",i+1,getWeaklyHardBroken(i));
	}


}

void exit_function(){

	countMissedDeadlines();

	countWeaklyHardBroken();

	writeReportInFile();

	print_report();

	exit(0);
}
