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
	bool report[MAX_PERIOD_CNT]; // VRATI NA BOLL, OVO JE RADI TESTIRANJA SAMO !!!!!
	int missed_deadlines;
	int times_killed;
	int weakly_hard_broken;
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
		if(temp < 0){
			temp=MAX_HIPERPERIOD+1;
			break;
		}
	}
	hiperperiod = temp;
}

void calculateTaskDuration(){
	for(int i=0;i<getTaskCnt();i++){
		double tmp = ((double) getTaskPeriod(i) * getTaskUtilization(i));
		tmp+=0.5;
		//Task_Set[i].duration = (int) ((double) getTaskPeriod(i) * getTaskUtilization(i));
		Task_Set[i].duration = (int) tmp;
		if(Task_Set[i].duration == 0) Task_Set[i].duration++;
	}
}

void readTaskPeriods(){

	char * periods_path = "/home/luka/FreeRTOS-simulator/periods.txt";

	FILE *file;

	file = fopen(periods_path,"r");

    const unsigned MAX_LENGTH = 16;
    char buffer[MAX_LENGTH];
	int tmp;

   // while (fgets(buffer, MAX_LENGTH, file)){
	for(int i=0;i<getTaskCnt();i++){
		fgets(buffer, MAX_LENGTH, file);
		//printf("%s", buffer);
		sscanf(buffer, "%d", &tmp);
    	//printf("The value of tmp : %d\n", tmp);
		Task_Set[i].period = tmp;
	}

	fclose(file);
}

void readWeaklyHardConstraint(){
	// 0 is for task that must meet al deadlines
	// n means that task can be skipped once in every n period interval
	Task_Set[0].weakly_hard_constraint = 2;

	char * weakly_hard_path = "/home/luka/FreeRTOS-simulator/weakly_hard.txt";

	FILE *file;

	file = fopen(weakly_hard_path,"r");

    const unsigned MAX_LENGTH = 16;
    char buffer[MAX_LENGTH];
	int tmp;

   // while (fgets(buffer, MAX_LENGTH, file)){
	for(int i=0;i<getTaskCnt();i++){
		fgets(buffer, MAX_LENGTH, file);
		//printf("%s", buffer);
		sscanf(buffer, "%d", &tmp);
    	//printf("The value of tmp : %d\n", tmp);
		Task_Set[i].weakly_hard_constraint = tmp;
	}

	fclose(file);


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

void generatePeriods(){
	for(int i=0;i<getTaskCnt();i++){
		int range = HIGHEST_PERIOD - LOWEST_PERIOD + 1;
		int tmp = (rand_0_to_1() * range) + LOWEST_PERIOD;
		Task_Set[i].period = tmp;
	}
}

void generateWeaklyHard(){
	for(int i=0;i<getTaskCnt();i++){
		int range = HIGHEST_WH - LOWEST_WH + 1;
		int tmp = (rand_0_to_1() * range) + LOWEST_WH;
		Task_Set[i].weakly_hard_constraint = tmp;
	}
}

int check_nessery_condition(){
	double tmp = 0;
	for(int i=0;i<getTaskCnt();i++){
		if(getWeaklyHardConstraint(i) != 0)
			tmp += (double)(getTaskDuration(i)*(getWeaklyHardConstraint(i)-1)) / (getTaskPeriod(i)*getWeaklyHardConstraint(i));
		else
			tmp += (double)(getTaskDuration(i)) / (getTaskPeriod(i));
	}
	if(tmp<=1) return 1;
	return 0;
}

int check_sufficient_condition(){
	double tmp = 0;
	for(int i=1;i<MAX_L;i++){
		tmp=0;
		for(int j=0;j<getTaskCnt();j++){
			double D;
			if(getWeaklyHardConstraint(j) != 0)
				D = (double)(i)/(getTaskPeriod(j)) - (double)(i)/(getTaskPeriod(j)*getWeaklyHardConstraint(j));
			else
				D = (double)(i)/(getTaskPeriod(j));

			tmp+= ((int) (D))*getTaskDuration(j);
		}
		tmp/=i;
		if(tmp>1) return 1;
	}
	return 1;
}

int check_utilization(){
	for(int i=0;i<getTaskCnt();i++){
		if(getTaskUtilization(i) >= 0.7) return 0;
	}
	return 1;
}

void startTaskSetGenerator(double utilization,int n, char * report_file){

	srand(time(NULL));

	TASK_CNT = n;

	total_utilization = utilization;

	file_path = report_file;

	while(1){
		calculateUtilization(utilization);
		if(check_utilization()) break;
	}

	while(1){
		generatePeriods();
		if(calculateHiperperiod() < MAX_HIPERPERIOD) break;
	}

	while(1){
		generateWeaklyHard();
		if(check_nessery_condition() == 1 && check_sufficient_condition() == 1) break;
	}

	generateWeaklyHard();

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
	printf("Skip-over faktor = %d\n",getWeaklyHardConstraint(x));
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

void countWeaklyHardBroken(){
	for(int i=0;i<getTaskCnt();i++){
		bool * rep = getReport(i);
		int cnt=0;
		int prevoius_solved = Task_Set[i].weakly_hard_constraint;
		for(int j=0;j<getTaskNumberOfPeriods(i);j++){
			if(rep[j] == 1){
				prevoius_solved++;
			}
			else if(rep[j] == 0){
				if(prevoius_solved < (Task_Set[i].weakly_hard_constraint-1)){
					cnt++;
				}
				prevoius_solved=0;
			}
		}
		Task_Set[i].weakly_hard_broken = cnt;
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

int sumOfAllTaskNum(){
	int ret=0;
	for(int i=0;i<getTaskCnt();i++){
		ret+=getNumOfPeriods(i);
	}
	return ret;
}

//quality of service (tasks that met deadline/all tasks)
double calculateQoS(){
	double ret;
	double missed = sumOfMissedDeadlines();
	double all = sumOfAllTaskNum();
	ret = (all-missed)/all;
	return ret;
}

void writeReportInFile(){

	if(weakly_hard == 0) return;

	FILE *file;

	file = fopen(file_path,"a");

	fprintf(file,"%lf,%d,%d,%d,%lf",total_utilization,getTaskCnt(),sumOfMissedDeadlines(),getWeaklyHard(),calculateQoS());

	for(int i=0;i<getTaskCnt();i++){
		fprintf(file,",%d",getMissedDeadlines(i));
	}

	fprintf(file,"\n");

	fclose(file);

}

int getNumOfPeriods(int i){
	int ret=Task_Set[i].numOfPeriods;
	return ret;
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

bool * getReport(int i){
	return Task_Set[i].report;
}

void setReport(int i,int x){
	Task_Set[i].report[x] = 1;
}

TaskHandle_t * getPointerToHandler(int i){
	TaskHandle_t * ret = &Task_Set[i].handler;
	return ret;
}

int getWeaklyHardBroken(int i){
	int ret=Task_Set[i].weakly_hard_broken;
	return ret;
}
