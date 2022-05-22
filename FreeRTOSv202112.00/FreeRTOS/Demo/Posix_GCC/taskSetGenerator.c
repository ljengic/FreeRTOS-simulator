#include "taskSetGenerator.h"
#include "math.h"
#include "FreeRTOS.h"
#include "task.h"
#include "stdlib.h"
#include "stdio.h"

int TASK_CNT;

struct periodic_task{
	TaskHandle_t handler;
	char * name;
	double u;
	TickType_t period;
	TickType_t duration;
	int weakly_hard_constraint;
	int numOfPeriods;
//	int currentPeriod;          // cunter used for writing in report (ne treba ipak, MOZE SE DOBITI KAO PROTEKLO VRIJEME/ PERIOD)
	int report[MAX_PERIOD_CNT]; // VRATI NA BOLL, OVO JE RADI TESTIRANJA SAMO !!!!!
	int missed_deadlines;
	int times_killed;
} Task_Set[MAX_TASK_CNT];

int weakly_hard = 1;  // if this varijable is 1 then weakly hard conditions are OK 
double u[MAX_TASK_CNT];
int hiperperiod;
double total_utilization;
char * file_path;
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

	int temp = getTaskPeriod(0);

	for(int i=1;i<getTaskCnt();i++){
		temp = LCM(getTaskPeriod(i),temp);
	}
	hiperperiod = temp;
}

void calculateTaskDuration(){
	for(int i=0;i<getTaskCnt();i++){
		Task_Set[i].duration = (int) ((double) getTaskPeriod(i) * getTaskUtilization(i));
	}
}

void generateTaskPeriods(){
	/*
	Task_Set[0].period = 30;
	Task_Set[1].period = 35;
	Task_Set[2].period = 40;
	Task_Set[3].period = 50;
	Task_Set[4].period = 100;
	*/
	Task_Set[0].period = 30;
	Task_Set[1].period = 35;
	Task_Set[2].period = 40;
	Task_Set[3].period = 50;
	Task_Set[4].period = 100;
}

void generateWeaklyHardConstraint(){
	// 0 is for task that must meet al deadlines
	// n means that task can be skipped once in every n period interval
	Task_Set[0].weakly_hard_constraint = 10;
	Task_Set[1].weakly_hard_constraint = 2;
	Task_Set[2].weakly_hard_constraint = 5;
	Task_Set[3].weakly_hard_constraint = 2;
	Task_Set[4].weakly_hard_constraint = 2;
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

	for(int i=0;i<getTaskCnt();i++){

		makeName(i+1);

		Task_Set[i].name = names[i];
	}

}

void calculateUtilization(double utilization){

	double sum_u,next_sum_u;


	sum_u = utilization;

	for(int i=1;i<getTaskCnt();i++){
		double pot = (double) 1/ (double )(getTaskCnt()-i);
		next_sum_u = (double) sum_u*pow(rand_0_to_1(),pot);
		Task_Set[i-1].u = (double) sum_u-next_sum_u;
		sum_u = next_sum_u;

	}

	Task_Set[getTaskCnt()-1].u = sum_u;

}

void calculateNumOfPeriods(){
	for(int i=0;i<getTaskCnt();i++){
			Task_Set[i].numOfPeriods = getHiperPeriod() / getTaskPeriod(i);
		}
}

void resetTimesKilled(){
		for(int i=0;i<getTaskCnt();i++){
			Task_Set[i].times_killed=0;
		}
}

void resetReports(){
		for(int i=0;i<getTaskCnt();i++){
			for(int j=0;j<MAX_PERIOD_CNT;j++){
				Task_Set[i].report[j]=0;
			}
		}
}

void startTaskSetGenerator(double utilization,int n, char * report_file){

	TASK_CNT = n;

	total_utilization = utilization;

	file_path = report_file;

	calculateUtilization(utilization);

	generateTaskPeriods();

	generateWeaklyHardConstraint();

	calculateTaskDuration();

	calculateHiperperiod();

	calculateNumOfPeriods();

	generateTaskNames();

	resetTimesKilled();

	resetReports();

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

void countMissedDeadlines(){
	for(int i=0;i<getTaskCnt();i++){
		bool * rep = getReport(i);
		int cnt=0;
		for(int j=0;j<getTaskNumberOfPeriods(i);j++){
			if(rep[j] == 0){
				cnt++;
			}
		}
		Task_Set[i].missed_deadlines = cnt;
	}
}

int sumOfMissedDeadlines(){
	int ret=0;
	for(int i=0;i<getTaskCnt();i++){
		ret+=getMissedDeadlines(i);
	}
	return ret;
}

int sumOfKilledTasks(){
	int ret=0;
	for(int i=0;i<getTaskCnt();i++){
		ret+=getTimesKilled(i);
	}
	return ret;
}

void writeReportInFile(){

	FILE *file;

	file = fopen(file_path,"a");

	fprintf(file,"%lf,%d,%d,%d,%d",total_utilization,getTaskCnt(),sumOfMissedDeadlines(),sumOfKilledTasks(),getWeaklyHard());

	for(int i=0;i<getTaskCnt();i++){
		fprintf(file,",%d",getMissedDeadlines(i));
	}

	for(int i=0;i<getTaskCnt();i++){
		fprintf(file,",%d",getTimesKilled(i));
	}

	fprintf(file,"\n");

	fclose(file);

}

void weaklyHardBroken(){
	weakly_hard = 0;
}

int getWeaklyHard(){
	int ret=weakly_hard;
	return ret;
}

int setWeaklyHard(){
	weakly_hard=1;
}

int getWeaklyHardConstraint(int i){
	int ret=Task_Set[i].weakly_hard_constraint;
	return ret;
}

void incrementTimesKilled(int i){
	Task_Set[i].times_killed++;
}

int getTimesKilled(int i){
	int ret=Task_Set[i].times_killed;
	return ret;
}

TaskHandle_t getTaskHandler(int i){
	TaskHandle_t ret=Task_Set[i].handler;
	return ret;
}

void setTaskHandler(int i,TaskHandle_t x){
	Task_Set[i].handler =x ;
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

int getMissedDeadlines(int i){
	int ret=Task_Set[i].missed_deadlines;
	return ret;
}

int * getReport(int i){
	return Task_Set[i].report;
}

int setReport(int i,int x){
	return Task_Set[i].report[x] = 1;
}

TaskHandle_t * getPointerToHandler(int i){
	TaskHandle_t * ret = &Task_Set[i].handler;
	return ret;
}
