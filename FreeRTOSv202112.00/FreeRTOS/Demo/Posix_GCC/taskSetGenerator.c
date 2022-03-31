#include "taskSetGenerator.h"
#include "math.h"
#include "FreeRTOS.h"
#include "stdlib.h"
#include "stdio.h"

int TASK_CNT;

struct periodic{
	double u;
	TickType_t period;
	TickType_t duration;
} Task_Set[MAX_TASK_CNT];

double u[MAX_TASK_CNT];

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

	int hiperperiod = Task_Set[0].period;

	for(int i=1;i<TASK_CNT-1;i++){
		hiperperiod = LCM(Task_Set[i].period,hiperperiod);
	}
	return hiperperiod;
}

void calculateTaskDuration(){
	for(int i=0;i<TASK_CNT-1;i++){
		Task_Set[i].duration = Task_Set[i].period * Task_Set[i].u;
	}
}

void generateTaskPeriods(){
	Task_Set[0].period = 30;
	Task_Set[1].period = 35;
	Task_Set[2].period = 40;
	Task_Set[3].period = 50;
	Task_Set[4].period = 100;
}


void calculateUtilization(double utilization){

	double sum_u,next_sum_u;


	sum_u = utilization;

	for(int i=1;i<TASK_CNT-1;i++){
		double pot = (double) 1/ (double )(TASK_CNT-i);
		next_sum_u = sum_u*pow(rand_0_to_1(),pot);
		Task_Set[i-1].u = sum_u-next_sum_u;
		sum_u = next_sum_u;
	}

	Task_Set[TASK_CNT-1].u = sum_u;

}

void startTaskSetGenerator(double utilization,int n){

	TASK_CNT = n;

	calculateUtilization(utilization);

	generateTaskPeriods();

	int hiper=calculateHiperperiod();

	printf("hiperperiod je %d\n",hiper);

}


TickType_t getTaskPeriod(int i){
	TickType_t ret=Task_Set[i].period;
	return ret;
}

TickType_t getTaskDuration(int i){
	TickType_t ret=Task_Set[i].duration;
	return ret;
}

int getTaskCnt(){
	return TASK_CNT;
}

