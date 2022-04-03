#include "taskSetGenerator.h"
#include "math.h"
#include "FreeRTOS.h"
#include "stdlib.h"
#include "stdio.h"

int TASK_CNT;

struct periodic_task{
	char * name;
	double u;
	TickType_t period;
	TickType_t duration;
	int numOfPeriods;
//	int currentPeriod;          // cunter used for writing in report (ne treba ipak MOZE SE DOBITI KAO PROTEKLO VRIJEME/ PERIOD)
	bool report[MAX_PERIOD_CNT];
} Task_Set[MAX_TASK_CNT];

double u[MAX_TASK_CNT];
int hiperperiod;
char names[MAX_TASK_CNT][MAX_NAME_LENGHT];
char default_name[MAX_NAME_LENGHT] = "Task_00\0";

int GCD(int a,int b)
{
  if (b == 0)
    return a;
  return GCD(b, a % b);
}


int LCM(int a, int b)
{
    return (a / GCD(a, b)) * b;
}

double rand_0_to_1()
{
    return (double)rand() / (double)RAND_MAX ;
}

int calculateHiperperiod(){

	int temp = Task_Set[0].period;

	for(int i=1;i<TASK_CNT-1;i++){
		temp = LCM(getTaskPeriod(i),temp);
	}
	hiperperiod = temp;
}

void calculateTaskDuration(){
	for(int i=0;i<TASK_CNT;i++){
		Task_Set[i].duration = (int) ((double) getTaskPeriod(i) * getTaskUtilization(i));
	}
}

void generateTaskPeriods(){
	Task_Set[0].period = 30;
	Task_Set[1].period = 35;
	Task_Set[2].period = 40;
	Task_Set[3].period = 50;
	Task_Set[4].period = 100;
}

void makeName(int x){

	int cnt=x;

	for(int i=0;i<MAX_NAME_LENGHT;i++){
		names[x-1][i]=default_name[i];
	}

	names[x-1][6] = (cnt%10) + '0';
	cnt/=10;
	names[x-1][5] = (cnt%10) + '0';

}

void generateTaskNames(){

	for(int i=0;i<TASK_CNT;i++){

		makeName(i+1);

		Task_Set[i].name = names[i];
	}

}

void calculateUtilization(double utilization){

	double sum_u,next_sum_u;


	sum_u = utilization;

	for(int i=1;i<TASK_CNT;i++){
		double pot = (double) 1/ (double )(TASK_CNT-i);
		next_sum_u = (double) sum_u*pow(rand_0_to_1(),pot);
		Task_Set[i-1].u = (double) sum_u-next_sum_u;
		sum_u = next_sum_u;

	}

	Task_Set[TASK_CNT-1].u = sum_u;

}

void calculateNumOfPeriods(){
	for(int i=0;i<TASK_CNT;i++){
			Task_Set[i].numOfPeriods = getHiperPeriod() / Task_Set[i].period;
		}
}

void startTaskSetGenerator(double utilization,int n){

	TASK_CNT = n;

	calculateUtilization(utilization);

	generateTaskPeriods();

	calculateTaskDuration();

	calculateHiperperiod();

	calculateNumOfPeriods();

	generateTaskNames();

}

void printInfo(int x){
	printf("\n**TASK INFORMATION**\n");
	printf("Task name is %s\n",getTaskName(x));
	printf("Period = %d\n",getTaskPeriod(x));
	printf("Duration = %d\n",getTaskDuration(x));
	printf("Utilization = %lf\n",getTaskUtilization(x));
	printf("Number of periods = %d\n",getTaskNumberOfPeriods(x));
	printf("\n");
}

int getTaskPeriod(int i){
	int ret=Task_Set[i].period;
	return ret;
}

int getTaskDuration(int i){
	int ret=Task_Set[i].duration;
	return ret;
}

double getTaskUtilization(int i){
	double ret=Task_Set[i].u;
	return ret;
}

int getTaskNumberOfPeriods(int i){
	int ret=Task_Set[i].numOfPeriods;
	return ret;
}

char * getTaskName(int i){
	char * ret = Task_Set[i].name;
	return ret;
}

int getTaskCnt(){
	return TASK_CNT;
}

int getHiperPeriod(){
	return hiperperiod;
}


